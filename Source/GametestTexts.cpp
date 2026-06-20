
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/BorderImage.h>
#include <EASTL/string.h>
#include <fmt/format.h>
#include <cmath>

#include "GametestTexts.h"

using namespace Urho3D;


GameplayPrototypeTexts::GameplayPrototypeTexts(Context* context) : Object(context) {

} 

void GameplayPrototypeTexts::RegisterObject(Context* context) {

	context->RegisterFactory<GameplayPrototypeTexts>();

}

void GameplayPrototypeTexts::StartInit() {

	CreatePrototype();
	ShowActiveRenderer();
}

void GameplayPrototypeTexts::CreatePrototype() {
	
	auto* cache = GetSubsystem<ResourceCache>();

	Font* UsedFont = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");

	myText = new Text(context_);

	auto* UISubsys = GetSubsystem<UI>()->GetRoot();
	UISubsys->AddChild(myText);

	myText->SetFont(UsedFont);
	myText->SetText("AMBATUKAM");

	myText->SetSize(300, 200);
	myText->SetFontSize(30);
	myText->SetColor(Color(0.7f, 1.0f, 1.0f));
	myText->SetHorizontalAlignment(HA_LEFT);
	myText->SetVerticalAlignment(VA_TOP);
	



	myFrame = new BorderImage(context_);
	UISubsys->AddChild(myFrame);

	myFrame->SetVerticalAlignment(VA_TOP);
	myFrame->SetHorizontalAlignment(HA_CENTER);


	myFPSCounter = new Text(context_);
	myFrame->AddChild(myFPSCounter);

	myFPSCounter->SetFont(UsedFont);
	myFPSCounter->SetColor(Color::RED);
	myFPSCounter->SetVerticalAlignment(VA_CENTER);
	myFPSCounter->SetHorizontalAlignment(HA_CENTER);
	myFPSCounter->SetText("FPS : 1000");
	myFPSCounter->SetFontSize(30);
	myFPSCounter->SetPosition(00, 40);

	







	
	
}

void GameplayPrototypeTexts::CreatePrototype1() {


}

void GameplayPrototypeTexts::ApplyUpdate(float dT) {
	int FPSValue = static_cast<int>(std::round(1.0f / dT));
	ea::string CurrentFps = fmt::format("FPS : {}", FPSValue).c_str();
	myFPSCounter->SetText(CurrentFps);
}

void GameplayPrototypeTexts::HandleUpdate(float dT) {
	ApplyUpdate(dT);

}

void GameplayPrototypeTexts::ShowActiveRenderer()
{
	// Retrieve the Graphics subsystem
	auto* graphics = GetSubsystem<Graphics>();

	if (graphics)
	{
		// Get the active graphics backend enum
		auto shi = graphics->GetApiName();
		//auto* renderDevice = GetSubsystem<RenderDevice>();
		//auto devicename = renderDevice->

		myRenderingBackend = new Text(context_);
		myFrame->AddChild(myRenderingBackend);


		auto* cache = GetSubsystem<ResourceCache>();
		Font* UsedFont = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");

		myRenderingBackend->SetFont(UsedFont);
		myRenderingBackend->SetColor(Color::GREEN);
		myRenderingBackend->SetFontSize(30);
		myRenderingBackend->SetVerticalAlignment(VA_CENTER);
		myRenderingBackend->SetHorizontalAlignment(HA_CENTER);
		myRenderingBackend->SetPosition(00, 70);
		
		myRenderingBackend->SetText(shi + "# NVIDIA GeForce RTX 3060");


		
		
	}
}