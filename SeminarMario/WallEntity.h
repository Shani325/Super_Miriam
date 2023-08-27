#pragma once
#include "Entities.h"

class WallEntity : public Entity
{
public:
	bool isStandOnMe;
	WallEntity(EntityStatePtr state):Entity(state),isStandOnMe(false){}
};

typedef std::shared_ptr<WallEntity> WallEntityPtr;

EntityStatePtr createWallState(std::string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));

	IGraphicsComponentPtr graphicsPtr(new SingleAnimationGraphics(animation,true));

	IPhysicsComponentPtr PhysicsPtr = (make_shared<FixedWidgetPhysics>());

	return make_shared<EntityState>(graphicsPtr, PhysicsPtr);
}

WallEntityPtr createWall(std::string const& rootAnimationsFolder)
{
	EntityStatePtr state = createWallState(rootAnimationsFolder);

	WallEntityPtr wall(new WallEntity(state));

	return wall;
}
