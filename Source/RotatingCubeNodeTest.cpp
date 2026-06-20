#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Resource/ResourceCache.h>


#include "RotatingCubeNodeTest.h"


RotatingCubeNodeTest::RotatingCubeNodeTest(Urho3D::Context* context) : LogicComponent(context) {

	SetUpdateEventMask(Urho3D::USE_UPDATE);
}

void RotatingCubeNodeTest::RegisterObject(Urho3D::Context* context) {

	context->RegisterFactory<RotatingCubeNodeTest>();

}

void RotatingCubeNodeTest::StartInit() {

	Urho3D::ResourceCache* cache_ = GetSubsystem<Urho3D::ResourceCache>();
	cubeNode_ = node_->CreateChild("CubeNode");
	Urho3D::StaticModel* model_ = cubeNode_->CreateComponent<Urho3D::StaticModel>();
	model_->SetModel(cache_->GetResource<Urho3D::Model>("Models/Box.mdl"));

	Urho3D::Node* moonCubeNode_ = cubeNode_->CreateChild("MoonNode");
	Urho3D::StaticModel* moonModel_ = moonCubeNode_->CreateComponent<Urho3D::StaticModel>();
	moonModel_->SetModel(cache_->GetResource<Urho3D::Model>("Models/Box.mdl"));
	moonCubeNode_->Scale(Urho3D::Vector3(0.5f, 0.5f, 0.5f));
	moonCubeNode_->SetPosition(Urho3D::Vector3(0.0f, 0.0f, 1.3f));

}

void RotatingCubeNodeTest::HandleUpdate(float timeStep) {

	cubeNode_->Rotate(Urho3D::Quaternion(0.0f, 45.0f * timeStep, 0.0f));
}

void RotatingCubeNodeTest::Update(float timeStep) {

	HandleUpdate(timeStep);

}