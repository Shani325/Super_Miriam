#pragma once
#include "Entities.h"

class CoinEntity : public Entity
{
public:
	bool isEated;
	CoinEntity(EntityStatePtr state) :Entity(state),isEated(false) {}
};

typedef std::shared_ptr<CoinEntity> CoinEntityPtr;

EntityStatePtr createCoinState(std::string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));

	IGraphicsComponentPtr graphicsPtr(new SingleAnimationGraphics(animation, true));

	IPhysicsComponentPtr PhysicsPtr = (make_shared<FixedWidgetPhysics>());

	return make_shared<EntityState>(graphicsPtr, PhysicsPtr);
}

CoinEntityPtr createCoin(std::string const& rootAnimationsFolder)
{
	EntityStatePtr state = createCoinState(rootAnimationsFolder);

	CoinEntityPtr coin(new CoinEntity(state));

	return coin;
}
