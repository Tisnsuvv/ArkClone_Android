#pragma once

#include <Urho3D/UI/Text.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Scene/Scene.h>
//#include <Urho3D/UI/Text.h>
#include <vector>


using namespace Urho3D;

class GameplayPrototypeTexts : public Object {

	URHO3D_OBJECT(GameplayPrototypeTexts, Object)

public:
	explicit GameplayPrototypeTexts(Context* context);
	static void RegisterObject(Context* context_);
	
	~GameplayPrototypeTexts() = default;
	void StartInit();
	void HandleUpdate(float dT);

private:

	void ApplyUpdate(float dT);
	void CreatePrototype1();
	void CreatePrototype();
	void ShowActiveRenderer();
	SharedPtr<Text> myText; 
	WeakPtr<Text> myFPSCounter;
	WeakPtr<Text> myRenderingBackend;
	WeakPtr<BorderImage> myFrame;
	
	//std::vector<WeakPtr<Node>> Nodes;
};