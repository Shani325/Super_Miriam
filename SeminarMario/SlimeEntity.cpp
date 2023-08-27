#include "SlimeEntity.h"
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

EntityStatePtr CreateSlimeEnemy(std::string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr PhysicsPtr = make_shared<FixedWidgetPhysics>();

	return make_shared<EntityState>(graphicsPtr, PhysicsPtr);
}

EntityPtr CreateSlime(std::string const& rootAnimationsFolder)
{
	auto state = CreateSlimeEnemy(rootAnimationsFolder);

	EntityPtr slime(new Entity(state));

	return slime;
}