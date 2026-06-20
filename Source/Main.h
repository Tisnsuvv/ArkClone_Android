
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Physics/PhysicsWorld.h>

#include "GametestTexts.h"

using namespace Urho3D;

class MyApplication : public Application
{
    // Enable type information.
    URHO3D_OBJECT(MyApplication, Application);

public:
    /// Construct.
    explicit MyApplication(Context* context);

    /// Setup before engine initialization. Modifies the engine parameters.
    void Setup() override;
    /// Setup after engine initialization. Creates the logo, console & debug HUD.
    void Start() override;
    /// Cleanup after the main loop. Called by Application.
    void Stop() override;

private:
    /// Update event handler.
    void Update(VariantMap& eventData);

    /// Scene to be rendered.
    SharedPtr<Scene> scene_;
    /// Viewport that renders the scene.
    SharedPtr<Viewport> viewport_;
    SharedPtr<GameplayPrototypeTexts> myPrototypeText;

    WeakPtr<PhysicsWorld> physicsWorld_;


    /// Geometry in the scene.
    WeakPtr<Node> geometryNode_;
    
    WeakPtr<Node> floorNode;
    WeakPtr<Node> playerCharacter_;
};