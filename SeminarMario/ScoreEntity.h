#pragma once
#include "Entities.h"

//Score


class Score : public Entity
{
	int _score;
public:
	Score(EntityStatePtr state, int score) :Entity(state), _score(score) {}

	virtual void onNotify(Event const& e) override
	{
		if (e.sender == EventSenders::SENDER_ENTITY_STATE
			&&
			e.type == EventTypes::EVENT_PHYSICS
			&&
			e.code == EventCodes::COLLISION_WITH_ENEMY)
		{
			if (_score >= 50) {
				_score -= 50;
				_state->getGraphics()->reset(_score);
			}
		}
		else if (e.sender == EventSenders::SENDER_ENTITY_STATE
			&&
			e.type == EventTypes::EVENT_PHYSICS
			&&
			e.code == EventCodes::COLLISION_WITH_FOOD_SCORE)
		{
			_score += 20;
			_state->getGraphics()->reset(_score);
		}
	}
};

EntityStatePtr createScoreState(int score)
{
	IGraphicsComponentPtr graphicsPtr(new ScoresGraphics(score));

	IPhysicsComponentPtr nonCollidingPhysicsPtr = (make_shared<FixedWidgetPhysics>());

	return make_shared<EntityState>(graphicsPtr, nonCollidingPhysicsPtr);
}

EntityPtr createScore(int score)
{
	auto state = createScoreState(score);

	EntityPtr scoreEntity(new Score(state,score));

	return scoreEntity;
}