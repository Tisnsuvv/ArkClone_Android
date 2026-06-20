// Copyright (c) 2023-2023 the rbfx project.
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT> or the accompanying LICENSE file.

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsWorld.h>

#include "Main.h"
#include "PlayerController.h"
#include "RotatingCubeNodeTest.h"
#include "GametestTexts.h"

//#include <Core.SamplePlugin/SampleComponent.h>

using namespace Urho3D;


// Define entry point.
URHO3D_DEFINE_APPLICATION_MAIN(MyApplication);

MyApplication::MyApplication(Context *context)
    : Application(context)
{
}

void MyApplication::Setup()
{
    engineParameters_[EP_ORGANIZATION_NAME] = "Minzuww Studio";
    engineParameters_[EP_APPLICATION_NAME] = "MovementProjectUrho3D";
    engineParameters_[EP_LOG_NAME] = "conf://MyApplication.log";
    engineParameters_[EP_WINDOW_TITLE] = "Movement — Urho3D";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_BORDERLESS] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = true;
    engineParameters_[EP_RENDER_BACKEND] = 3 ;
}

void MyApplication::Start()
{
    //SampleComponent::RegisterObject(context_);

    auto cache = GetSubsystem<ResourceCache>();
    auto renderer = GetSubsystem<Renderer>();

    RotatingCubeNodeTest::RegisterObject(context_);
    PlayerController::RegisterObject(context_);
    GameplayPrototypeTexts::RegisterObject(context_);

    // Create scene.
    scene_ = MakeShared<Scene>(context_);
    scene_->CreateComponent<Octree>();
    physicsWorld_ = scene_->CreateComponent<PhysicsWorld>();
    physicsWorld_->SetGravity(Vector3{0.0f,-9.8f,0.0f});


    // Create camera.
    Node* cameraNode = scene_->CreateChild("Camera");
    Camera* camera = cameraNode->CreateComponent<Camera>();
    camera->SetFov(90.0f);


    // Create zone.
    Zone* zone = scene_->CreateComponent<Zone>();
    zone->SetFogColor(0xC9C0BB_rgb);
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetAmbientColor(Color(0.2f, 0.2f, 0.25f));
    
    floorNode = scene_->CreateChild("Floor");
    StaticModel* floorModel = floorNode->CreateComponent<StaticModel>();
    floorModel->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    floorModel->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

    RigidBody* floorBody = floorNode->CreateComponent<RigidBody>();
    floorBody->SetMass(0);
    CollisionShape* floorShape = floorNode->CreateComponent<CollisionShape>();
    floorShape->SetBox(Vector3::ONE);

    floorNode->SetPosition(Vector3{ 0.0f, -5.0f, 0.0f });
    floorNode->SetScale(Vector3{ 50.0f, 1.0f, 50.0f });


    ////### Customs Inits
    Node* CubeNode_ = scene_->CreateChild("CubeNode");
    RotatingCubeNodeTest* LCubeNode_ = CubeNode_->CreateComponent<RotatingCubeNodeTest>();
    LCubeNode_->StartInit();

    playerCharacter_ = scene_->CreateChild("PlayerCharacter");
    PlayerController* playerLogic = playerCharacter_->CreateComponent<PlayerController>();
    playerLogic->ConstructPlayerNode();
    playerLogic->SetUpCamera(camera);

    myPrototypeText = MakeShared<GameplayPrototypeTexts>(context_);
    //SharedPtr<GameplayPrototypeTexts> myPrototypeText(new GameplayPrototypeTexts(context_));
    myPrototypeText->StartInit();


    // Create light.
    Node* lightNode = scene_->CreateChild("Light");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);

    // Create viewport.
    const auto viewport = MakeShared<Viewport>(context_, scene_, camera);
    renderer->SetViewport(0, viewport);

    SubscribeToEvent(E_UPDATE, &MyApplication::Update);


}

void MyApplication::Stop()
{

}

void MyApplication::Update(VariantMap& eventData)
{

    float dT = eventData[Update::P_TIMESTEP].GetFloat();
    auto input = GetSubsystem<Input>();
    if (input->GetKeyPress(KEY_ESCAPE))
        SendEvent(E_EXITREQUESTED);
    myPrototypeText->HandleUpdate(dT);
    
}
