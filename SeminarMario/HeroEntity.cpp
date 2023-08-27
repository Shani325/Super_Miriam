#include "HeroEntity.h"
#include "opencv2/opencv.hpp"
#include "Config.h"

#include <memory>
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

enum HeroStates {
	HERO_IDLE,
	HERO_RUN_RIGHT,
	HERO_RUN_LEFT,
	HERO_DUCK, 
	HERO_STAY_DUCK, 
	HERO_STAND_AFTER_DUCK, 
	HERO_JUMP ,
	HERO_THROW_APPLE
};

EntityStatePtr createHeroState(	fs::path const& animationFolder, HeroStates state)
{
	AnimationPtr animation(new Animation(animationFolder.string()));
	bool isNotCyclic =
		state == HeroStates::HERO_DUCK ||
		state == HeroStates::HERO_STAND_AFTER_DUCK ||
		state == HeroStates::HERO_JUMP;
	bool isCyclic = !isNotCyclic;

	IGraphicsComponentPtr graphicsPtr(new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr boundedPhysicsPtr = nullptr;
	Size frameSize = graphicsPtr->getCollisionMask().size();
	switch (state)
	{
	case HERO_IDLE:
		boundedPhysicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<FixedWidgetPhysics>());
		break;	
	case HERO_RUN_RIGHT:
		boundedPhysicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<ConstVelocityPhysics>(Point(frameSize.width / 10, 0)));
		break;
	case HERO_RUN_LEFT:
		boundedPhysicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<ConstVelocityPhysics>(Point(frameSize.width / -10, 0)));
		break;
	case HERO_DUCK:
		boundedPhysicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<FixedWidgetPhysics>());
		break;
	case HERO_STAY_DUCK:
		boundedPhysicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<FixedWidgetPhysics>());
		break;
	case HERO_STAND_AFTER_DUCK:
		boundedPhysicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<FixedWidgetPhysics>());
		break;
	case HERO_JUMP:
		boundedPhysicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<JumpPhysics>(35,50,10));;//maybe change value
		break;
	default:
		throw std::exception("Unknown physics state!");
	}

	return make_shared<EntityState>(graphicsPtr, boundedPhysicsPtr);
}

HeroEntityPtr createHero( std::string const & rootAnimationsFolder)
{
	fs::path root = rootAnimationsFolder;
	auto idleFolder = root / "idle";

	auto idle = createHeroState(root / "idle", HeroStates::HERO_IDLE);
	auto runRight = createHeroState(root / "runRight", HeroStates::HERO_RUN_RIGHT);
	auto jump = createHeroState(root / "jump", HeroStates::HERO_JUMP);
	auto runLeft = createHeroState(root / "runLeft", HeroStates::HERO_RUN_LEFT);
	auto duck = createHeroState(root / "duckDown", HeroStates::HERO_DUCK);
	auto stayDuck = createHeroState(root / "duckStay", HeroStates::HERO_STAY_DUCK);
	auto standAfterDuck = createHeroState(root / "standAfterDuck", HeroStates::HERO_STAND_AFTER_DUCK);

	// @4: TOOD: add states: runLeft, duck, stayDuck, standAfterDuck

	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, runRight);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, runLeft);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);

	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, idle);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);
	runRight->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, idle);
	runRight->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_WALL }, idle);

	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, idle);
	runLeft->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, idle);
	runLeft->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_WALL }, idle);

	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED }, idle);
	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, idle);
	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_WALL }, idle);

	duck->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION }, stayDuck);

	stayDuck->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, standAfterDuck);

	standAfterDuck->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION }, idle);

	//  @4: TOOD: add all events here... 
	HeroEntityPtr hero(new HeroEntity(idle));

	idle->Register(hero);
	runRight->Register(hero);
	runLeft->Register(hero);
	jump->Register(hero);
	duck->Register(hero);
	stayDuck->Register(hero);
	standAfterDuck->Register(hero);
	//  @4: TOOD: add all states here... 

	return hero;
}