#pragma once
#include "Entities.h"

class HeartsEntity : public Entity
{
public:
	int _numHearts;
	HeartsEntity(EntityStatePtr state, int numHearts) :Entity(state), _numHearts(numHearts) {}

	virtual void onNotify(Event const& e) override
	{
		if (e.sender == EventSenders::SENDER_ENTITY_STATE
			&&
			e.type == EventTypes::EVENT_PHYSICS
			&&
			e.code == EventCodes::COLLISION_WITH_ENEMY)
		{
			_state->getGraphics()->reset(--_numHearts);
		}
		else if (e.sender == EventSenders::SENDER_ENTITY_STATE
			&&
			e.type == EventTypes::EVENT_PHYSICS
			&&
			e.code == EventCodes::COLLISION_WITH_FOOD_LIFE)
		{
			if (_numHearts <= 10)
				_state->getGraphics()->reset(++_numHearts);
		}
	}
};

typedef std::shared_ptr<HeartsEntity> HeartsEntityPtr;

EntityStatePtr createHeartsState(std::string const& animationFolder, int numHearts)
{
	Frame animation(animationFolder);

	IGraphicsComponentPtr graphicsPtr(new LivesGraphics(animation, numHearts));

	IPhysicsComponentPtr PhysicsPtr = (make_shared<FixedWidgetPhysics>());

	return make_shared<EntityState>(graphicsPtr, PhysicsPtr);
}

HeartsEntityPtr createHearts(std::string const& rootAnimationsFolder, int numHearts)
{
	EntityStatePtr state = createHeartsState(rootAnimationsFolder, numHearts);

	HeartsEntityPtr heart(new HeartsEntity(state, numHearts));

	return heart;
}

