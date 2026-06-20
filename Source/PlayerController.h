#pragma once

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Container/Ptr.h>       // WeakPtr
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Graphics/Camera.h>



namespace Urho3D
{
    class RigidBody;
    class Node;
}


enum class MovementState : unsigned char
{
    Grounded = 0,  
    Jumping,     
    Falling,      
};


class PlayerController final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerController, LogicComponent)
 
public:
 
    static void RegisterObject(Urho3D::Context* context);
    explicit PlayerController(Urho3D::Context* context);
    ~PlayerController() override = default;
    void ConstructPlayerNode();
    void SetUpCamera(Urho3D::Camera* camera_);

    float characterMass_{ 56.0f };
    float moveSpeed_{ 6.0f }; 
    float jumpImpulse_{ characterMass_ * 7.0f };
    float yawSpeed_{ 0.15f };  
    float pitchSpeed_{ 0.15f };  
    float pitchMin_{ -60.0f };  
    float pitchMax_{ 60.0f };  
    float groundRayLen_{ 0.25f };  

    
 

    MovementState GetMovementState() const { return movementState_; }

protected:
 
    void Start() override;
    void Update(float timeStep) override;
    void FixedUpdate(float timeStep) override;


private:

   
    bool IsGrounded() const;
    Urho3D::WeakPtr<Urho3D::RigidBody> rigidBody_;       
    Urho3D::WeakPtr<Urho3D::Node>      cameraPivotNode_; 
    Urho3D::WeakPtr<Urho3D::Node>      endPointCamNode_;
    float         yaw_{ 0.0f }; 
    float         pitch_{ 0.0f }; 
    MovementState movementState_{ MovementState::Grounded };
    Urho3D::Vector3 pendingMoveDir_{ Urho3D::Vector3::ZERO };
    bool pendingJump_{ false };
};