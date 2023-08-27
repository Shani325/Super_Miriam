#pragma once
#include "Entities.h"
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

class AppleEntity : public Entity
{
	bool _isActive;
	int _countSteps;
public:
	AppleEntity(EntityStatePtr state, cv::Mat background) :Entity(state), _isActive(false), _countSteps(0){}
	
	virtual void onNotify(Event const& e) override
	{
		if (e.sender == EventSenders::SENDER_KEYBOARD
			&&
			e.type == EventTypes::EVENT_KEY_PRESSED
			&&
			e.code == EventCodes::KEY_THROW)
		{
			_isActive = true;
		}		
	}

	bool getIsActive() {
		return _isActive;
	}
	
	void setIsActive(bool isActive) {
		_isActive = isActive;
	}

	int getCountSteps() {
		return _countSteps;
	}

	void setCountSteps(int countSteps) {
		_countSteps = countSteps;
	}
};

typedef std::shared_ptr<AppleEntity> AppleEntityPtr;

EntityStatePtr createAppleState(std::string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(new SingleAnimationGraphics(animation, isCyclic));

	Size frameSize = graphicsPtr->getCollisionMask().size();

	IPhysicsComponentPtr PhysicsPtr = make_shared<BoundedPhysicsDecorator>(
		make_shared<ConstVelocityPhysics>(cv::Point(frameSize.width / 5, 0)));

	return make_shared<EntityState>(graphicsPtr, PhysicsPtr);
}

AppleEntityPtr createApple(std::string const& rootAnimationsFolder, cv::Mat background)
{
	EntityStatePtr state = createAppleState(rootAnimationsFolder);

	AppleEntityPtr apple(new AppleEntity(state, background));

	return apple;
}