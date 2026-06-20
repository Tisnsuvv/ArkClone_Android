#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Scene/Node.h>

class RotatingCubeNodeTest final : public Urho3D::LogicComponent {

	URHO3D_OBJECT(RotatingCubeNodeTest, Urho3D::LogicComponent)

public:
	static void RegisterObject(Urho3D::Context* context);
	explicit RotatingCubeNodeTest(Urho3D::Context* context);
	~RotatingCubeNodeTest() = default;
	void StartInit();
	void Update(float timeStep)override;
	void HandleUpdate(float timeStep);
private:
	Urho3D::WeakPtr<Urho3D::Node> cubeNode_;
};