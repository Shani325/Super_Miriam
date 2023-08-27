#include "Physics.h"
//#include "Sizes.h"
#include <algorithm>
#define SCREEN_WIDTH 1200

using namespace cv;
using namespace std;
/// //////////////////////////////////////////

// @2: Read this function and try to understand what it does. Use Image Watch !
bool checkPixelLevelCollision(IPhysicsComponent const* first, IPhysicsComponentPtr const& other)
{
	cv::Mat const& mask_First = first->getCollisionMask();
	cv::Mat const& mask_Other = other->getCollisionMask();
	if (mask_Other.empty() || mask_First.empty())
	{
		//cout << "empty:" <<mask_First.empty()<<"|"<<mask_Other.empty()<< endl;
		return false;
	}


	Point TL_first = first->getTL();
	Point TL_other = other->getTL();

	// ROI = Reagion of Interest
	Rect firstROI(TL_first, TL_first + Point(mask_First.size()));
	Rect othersROI(TL_other, TL_other + Point(mask_Other.size()));
	Rect intersection = firstROI & othersROI;
	if (intersection.empty())
		return false;

	Rect intersection_RelativeToMe(
		intersection.tl() - TL_first, intersection.br() - TL_first);

	Rect intersection_RelativeToOther(
		intersection.tl() - TL_other, intersection.br() - TL_other);

	Mat myMaskROI = mask_First(intersection_RelativeToMe);
	Mat othersMaskROI = mask_Other(intersection_RelativeToOther);
	Mat whereMeAndOtherIntersect = myMaskROI.mul(othersMaskROI);

	int flag = countNonZero(whereMeAndOtherIntersect);
	//cout << "flag:" << flag << endl;
	return flag;
}

/// //////////////////////////////////////////

FixedWidgetPhysics::FixedWidgetPhysics()
	:_topLeft(0, 0), _mask()
{
}

void FixedWidgetPhysics::reset(cv::Point const& tl)
{
	_topLeft = tl;
}

bool FixedWidgetPhysics::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	return false;
}

bool FixedWidgetPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& FixedWidgetPhysics::getTL() const
{
	return _topLeft;
}

cv::Mat const& FixedWidgetPhysics::getCollisionMask() const
{
	return _mask;
}

//Const Velocity Physics

ConstVelocityPhysics::ConstVelocityPhysics(cv::Point const& velocity) :_velocity(velocity), _currTL(0, 0), _mask()
{
}

void ConstVelocityPhysics::reset(cv::Point const& tl)
{
	_currTL = tl;
}

bool ConstVelocityPhysics::update(cv::Mat const& collisionMask)//the physic doesn't change, just from outside.
{
	_mask = collisionMask;
	_currTL += _velocity;
	return false;
}

cv::Mat const& ConstVelocityPhysics::getCollisionMask() const
{
	return _mask;
}

bool ConstVelocityPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& ConstVelocityPhysics::getTL() const
{
	return _currTL;
}

//Jump Physics

JumpPhysics::JumpPhysics(int horizontalVelocity, int initialVerticalVelocity, int gravity) :_mask(), _currTL(0, 0)//?????
{
	if (gravity < 0)
		throw ("Error! Gravity must be initial positive value!");
	_gravity = gravity;
	if (initialVerticalVelocity <= 0)
		throw ("Error! initial vertical velocity must be initial positive value!");
	_initialJumpVelocity = Point(horizontalVelocity, -initialVerticalVelocity);
}

void JumpPhysics::reset(cv::Point const& tl)
{
	_currTL = _jumpStartTL = tl;
	_currVelocity = _initialJumpVelocity;
}

bool JumpPhysics::update(cv::Mat const& collisionMask)
{
	bool isChange = false;
	_mask = collisionMask;
	_currTL += _currVelocity;
	_currVelocity.y += _gravity;
	
	if (_currTL.y >= _jumpStartTL.y)//if change physic
	{
		_currTL.y = _jumpStartTL.y;
		isChange = true;
	}
	return isChange;
}

cv::Mat const& JumpPhysics::getCollisionMask() const
{
	return _mask;
}

bool JumpPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& JumpPhysics::getTL() const
{
	return _currTL;
}

//Non Colliding Physics Decorator

NonCollidingPhysicsDecorator::NonCollidingPhysicsDecorator(IPhysicsComponentPtr base)
	:_base(base)
{
}

void NonCollidingPhysicsDecorator::reset(cv::Point const& tl)
{
	_base->reset(tl);
}

bool NonCollidingPhysicsDecorator::update(cv::Mat const& collisionMask)
{
	return _base->update(collisionMask);
}

cv::Mat const& NonCollidingPhysicsDecorator::getCollisionMask() const
{
	return Mat();
}

bool NonCollidingPhysicsDecorator::checkCollision(std::shared_ptr<IPhysicsComponent> const& other) const
{
	return false;
}

cv::Point const& NonCollidingPhysicsDecorator::getTL() const
{
	return _base->getTL();
}

//Bounded Physics Decorator

BoundedPhysicsDecorator::BoundedPhysicsDecorator(IPhysicsComponentPtr base) :_base(base)
{
	_bounds.width = 1200;
	_bounds.height = 700;
}

bool BoundedPhysicsDecorator::test()
{
	_currTL.y = _base->getTL().y;
	if (_base->getTL().x > _bounds.x + _bounds.width - _base->getCollisionMask().size().width)//if its go out to the right side
	{
		_currTL.x = _base->getTL().x % _bounds.width;
		return true;
	}
	if (_base->getTL().x <= 0)//if its go out to the left side
	{
		_currTL.x = _bounds.width + _base->getTL().x % _bounds.width;//the base TL go down to minus
		return true;
	}
	return false;
}

void BoundedPhysicsDecorator::reset(cv::Point const& tl)
{
	_currTL = tl;
	_base->reset(tl);
}

bool BoundedPhysicsDecorator::update(cv::Mat const& collisionMask)
{
	bool is = _base->update(collisionMask);
	if (!test())
		_currTL = _base->getTL();
	return is;
}

cv::Mat const& BoundedPhysicsDecorator::getCollisionMask() const
{
	return _base->getCollisionMask();
}

bool BoundedPhysicsDecorator::checkCollision(std::shared_ptr<IPhysicsComponent> const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& BoundedPhysicsDecorator::getTL() const
{
	return _currTL;
}
//throw physic
/*

ThrowPhysics::ThrowPhysics(int horizontalVelocity, int initialVerticalVelocity, int gravity)
	:_gravity(gravity), _initialThrowVelocity(horizontalVelocity, -initialVerticalVelocity)
														,_currVelocity(_initialThrowVelocity)
{
	if (gravity < 0 || initialVerticalVelocity < 0)
		throw exception("A positive number was expected");
}
void ThrowPhysics::reset(cv::Point const& tl)
{
	_currTL = tl;
	_currVelocity = _initialThrowVelocity;
}
bool ThrowPhysics::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	_currTL += _currVelocity;
	_currVelocity.y += _gravity;

	return _currTL.y >= _bottom;//return true if the throw is over and false otherwise
}
cv::Mat const& ThrowPhysics::getCollisionMask() const
{
	return _mask;
}
bool ThrowPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}
cv::Point const& ThrowPhysics::getTL() const
{
	return _currTL;
}
cv::Point const& ThrowPhysics::getVelocity() const
{
	return _currVelocity;
}

*/