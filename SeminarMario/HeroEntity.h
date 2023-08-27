#pragma once

#include "Entities.h"
#include "Config.h"

class HeroEntity : public Entity
{
public:
	bool _isFinishedPhysics;
	HeroEntity(EntityStatePtr state) :Entity(state), _isFinishedPhysics(false) {}
	virtual void onNotify(Event const& e) override
	{
		if (e.sender == EventSenders::SENDER_ENTITY_STATE
			&&
			e.type == EventTypes::EVENT_PHYSICS
			&&
			e.code == EventCodes::ENTITY_PHYSICS_FINISHED)
		{
			_isFinishedPhysics = true;
		}
		else if (e.sender == EventSenders::SENDER_ENTITY_STATE
			&&
			e.type == EventTypes::EVENT_PHYSICS
			&&
			e.code == EventCodes::ENTITY_PHYSICS_NOT_FINISHED)
		{
			_isFinishedPhysics = false;
		}
		Entity::onNotify(e);
	}
};

typedef std::shared_ptr<HeroEntity> HeroEntityPtr;

HeroEntityPtr createHero(std::string const& rootAnimationsFolder);

//bool checkCollisionWithEntitiesPool(EntityPtr hero, vector<EntityPtr> pool, vector<bool> isInUse);