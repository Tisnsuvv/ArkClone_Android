#include "PlayerController.h"



#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Node.h>

using namespace Urho3D;



static constexpr unsigned PLAYER_COLLISION_LAYER = 1u << 1u; 
static constexpr unsigned GROUND_RAYCAST_MASK = ~PLAYER_COLLISION_LAYER;
static constexpr float    EYE_HEIGHT = 1.6f;
static constexpr float    CAPSULE_HEIGHT = 1.8f;
static constexpr float    CAPSULE_DIAMETER = 0.7f;   
static const Vector3      CAPSULE_OFFSET{ 0.0f, CAPSULE_HEIGHT * 0.5f, 0.0f };
static constexpr float    GROUND_RAY_ORIGIN_LIFT = 0.05f;
static constexpr float    CAMERA_DISTANCE = -3.0f;



void PlayerController::RegisterObject(Context* context)
{
   
    context->RegisterFactory<PlayerController>("Player");

    
 
}



PlayerController::PlayerController(Context* context)
    : LogicComponent(context)
{
    
    SetUpdateEventMask(USE_UPDATE | USE_FIXEDUPDATE);
}




void PlayerController::ConstructPlayerNode()
{
    Node* playerNode = GetNode();
    if (!playerNode)
    {
        URHO3D_LOGERROR("PlayerController::ConstructPlayerNode — "
            "component is not attached to a node!");
        return;
    }

    auto* cache = GetSubsystem<ResourceCache>();
    auto* body = playerNode->CreateComponent<RigidBody>();
    body->SetMass(56.0f);
    body->SetAngularFactor(Vector3::ZERO);
    body->SetLinearDamping(0.2f);
    body->SetCollisionEventMode(COLLISION_ALWAYS);
    body->SetCollisionLayer(PLAYER_COLLISION_LAYER);
    body->SetCollisionMask(GROUND_RAYCAST_MASK);
    
  
    rigidBody_ = WeakPtr<RigidBody>(body);


    auto* shape = playerNode->CreateComponent<CollisionShape>();
    shape->SetCapsule(CAPSULE_DIAMETER, CAPSULE_HEIGHT,
        CAPSULE_OFFSET, Quaternion::IDENTITY);

  
    auto* model = playerNode->CreateComponent<AnimatedModel>();  
    model->SetModel(cache->GetResource<Model>("Models/Ninja.mdl"));
    model->SetMaterial(cache->GetResource<Material>("Materials/Ninja.xml"));
    model->SetCastShadows(true);
    
    
    Node* camPivot = playerNode->CreateChild("CameraPivot");
    Node* endPointCam = camPivot->CreateChild("CameraEndPoint");

    camPivot->SetPosition(Vector3(0.0f, EYE_HEIGHT, 0.0f));
    endPointCam->SetPosition(Vector3(0.8f, 0.0f, CAMERA_DISTANCE));
    endPointCamNode_ = WeakPtr<Node>(endPointCam);
    cameraPivotNode_ = WeakPtr<Node>(camPivot);
    

    


    yaw_ = playerNode->GetRotation().YawAngle();
    URHO3D_LOGINFO("PlayerController: player node constructed successfully.");
}



void PlayerController::Start()
{
  
    auto* input = GetSubsystem<Input>();
    input->SetMouseMode(MM_RELATIVE);
}

void PlayerController::SetUpCamera(Urho3D::Camera* camera_) {
    
    endPointCamNode_->AddComponent(camera_, 0);
    Urho3D::Node* cameraNode_ = camera_->GetNode();
    
}

void PlayerController::Update(float timeStep)
{
    if (!cameraPivotNode_)
        return;

    auto* input = GetSubsystem<Input>();
    const IntVector2 mouseDelta = input->GetMouseMove();

    yaw_ += static_cast<float>(mouseDelta.x_) * yawSpeed_;
    pitch_ += static_cast<float>(mouseDelta.y_) * pitchSpeed_;
    pitch_ = Clamp(pitch_, pitchMin_, pitchMax_);

   
    //cameraPivotNode_->SetRotation(Quaternion(yaw_, Vector3::UP));
    GetNode()->SetRotation(Quaternion(yaw_, Vector3::UP));


    if (!cameraPivotNode_.Expired())
        cameraPivotNode_->SetRotation(Quaternion(pitch_, Vector3::RIGHT));

    pendingMoveDir_ = Vector3::ZERO;

    if (input->GetKeyDown(KEY_W))  pendingMoveDir_ += Vector3::FORWARD;
    if (input->GetKeyDown(KEY_S))  pendingMoveDir_ += Vector3::BACK;
    if (input->GetKeyDown(KEY_A))  pendingMoveDir_ += Vector3::LEFT;
    if (input->GetKeyDown(KEY_D))  pendingMoveDir_ += Vector3::RIGHT;


    if (pendingMoveDir_.LengthSquared() > M_EPSILON)
        pendingMoveDir_.Normalize();

    if (input->GetKeyPress(KEY_SPACE) && movementState_ == MovementState::Grounded)
        pendingJump_ = true;
}



void PlayerController::FixedUpdate(float timeStep)
{
    
    if (rigidBody_.Expired())
        return;

    RigidBody* body = rigidBody_.Get();
    const bool grounded = IsGrounded();


    switch (movementState_)
    {
    case MovementState::Grounded:
       
        if (!grounded)
            movementState_ = MovementState::Falling;
        break;

    case MovementState::Jumping:
        
        if (body->GetLinearVelocity().y_ <= 0.0f)
            movementState_ = MovementState::Falling;
        break;

    case MovementState::Falling:
        
        if (grounded)
            movementState_ = MovementState::Grounded;
        break;
    }

   
    if (pendingJump_)
    {
        if (movementState_ == MovementState::Grounded)
        {
            body->ApplyImpulse(Vector3::UP * jumpImpulse_);
            movementState_ = MovementState::Jumping;
        }
        pendingJump_ = false;   
    }


    const Quaternion& worldYaw = GetNode()->GetRotation();
    const Vector3     worldMoveDir = worldYaw * pendingMoveDir_;

    Vector3 velocity = body->GetLinearVelocity();
    velocity.x_ = worldMoveDir.x_ * moveSpeed_;
    velocity.z_ = worldMoveDir.z_ * moveSpeed_;
    

    body->SetLinearVelocity(velocity);
}



bool PlayerController::IsGrounded() const
{
    Scene* scene = GetScene();
    if (!scene)
        return false;

    auto* physicsWorld = scene->GetComponent<PhysicsWorld>();
    if (!physicsWorld)
        return false;

   
    const Vector3 origin = GetNode()->GetWorldPosition()
        + Vector3(0.0f, GROUND_RAY_ORIGIN_LIFT, 0.0f);

    PhysicsRaycastResult result;
    physicsWorld->RaycastSingle(
        result,
        Ray(origin, Vector3::DOWN),
        groundRayLen_,           
        GROUND_RAYCAST_MASK);    

    return result.body_ != nullptr;
}