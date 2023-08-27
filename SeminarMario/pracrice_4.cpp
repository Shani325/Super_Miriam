#include "Animation.h"
#include <opencv2/opencv.hpp>
#include "Timer.h"
#include "Sizes.h"
#include "Observer.h"
#include "Config.h"
#include "HeroEntity.h"
#include "EntitiesPool.h"
#include "SlimeEntity.h"
#include "LivesEntity.h"
#include "ScoreEntity.h"
#include "Apple.h"
#include <ctime>
#include "WallEntity.h"
#include "CoinEntity.h"

#include <chrono>
#include <thread>

using namespace cv;
using namespace std;

//Global Variables
cv::Mat background = cv::imread(R"(../Animations/background.png)", cv::IMREAD_UNCHANGED);
cv::Mat canvas = background.clone();
cv::Point topLeft(canvas.size().width / 45, canvas.size().height * 2 / 4.8);

int main()
{
	srand((unsigned)time(NULL));

	cv::resize(background, background, cv::Size(screen_width, screen_height));

	int floor = background.size().height * 2 / 3;

	HeroEntityPtr hero = createHero(R"(../Animations/Hero)");
	hero->reset(Point(background.size().width, background.size().height * 2 / 3));

	int countHearts = 4;
	HeartsEntityPtr hearts = createHearts(R"(../Animations/heart.png)", countHearts);
	hearts->reset(Point(850, 20));

	EntityPtr score = createScore(500);
	score->reset(Point(50, 50));

	AppleEntityPtr apple = createApple(R"(../Animations/apple)", canvas);

	EntitiesPool slimesPool;
	for (int i = 0; i < 1; i++) {
		slimesPool.insert(CreateSlime(R"(../Animations/SlimeOrange)"));
	}

	EntityPtr slime = slimesPool.getNext();
	slime->reset(Point(background.size().width * 2 / 3, background.size().height * 4 / 5));

	Timer timer(/*freq. ms=*/100);

	vector<WallEntityPtr> wallsVector;
	for (int i = 0; i < 3; i++)
	{
		wallsVector.push_back(createWall(R"(../Animations/Wall)"));
		timer.Register(wallsVector[i]);
	}

	vector<CoinEntityPtr> coinsVector;
	for (int i = 0; i < 3; i++)
	{
		coinsVector.push_back(createCoin(R"(../Animations/Coin)"));
		timer.Register(coinsVector[i]);
	}

	timer.Register(hero);
	timer.Register(slime);
	timer.Register(apple);

	int currentKey = 0;

	while (true)
	{
		Mat canvas = background.clone();

		int tempKey = timer.tick();
		if (tempKey != -1)
			currentKey = tempKey;

		//draw walls
		cv::Point position(700, 300);

		for (int i = 0; i < wallsVector.size(); i++)
		{
			wallsVector[i]->reset(position);
			wallsVector[i]->draw(canvas);
			position.x -= 200;
			position.y += 100;
		}

		//draw coins
		position.x = 750;
		position.y = 250;
		for (int i = 0; i < coinsVector.size(); i++)
		{
			if (!coinsVector[i]->isEated)
			{
				coinsVector[i]->reset(position);
				coinsVector[i]->draw(canvas);
			}
			position.x -= 200;
			position.y += 100;
		}

		hero->draw(canvas);
		score->draw(canvas);
		hearts->draw(canvas);
		
		//Exit from game
		if (hearts->_numHearts == 0)
		{
			exampleOfPutText(canvas);
			waitKey();
			exit(0);
		}

		//draw slimes
		for (int i = 0; i < slimesPool.getPool().size(); i++)
		{
			if (slimesPool.getIsInUse()[i])
				slimesPool.getPool()[i]->draw(canvas);
		}

		//Check collision with enemies
		for (int i = 0; i < slimesPool.getPool().size(); i++)
		{
			if (slimesPool.getIsInUse()[i] && hero->getState()->getPhysics()->checkCollision(slimesPool.getPool()[i]->getState()->getPhysics()))
			{
				hero->getState()->Register(hearts);
				hero->getState()->Register(score);
				hero->reset(Point(slimesPool.getPool()[i]->getState()->getPhysics()->getTL().x - 200, background.size().height * 2 / 3));
				hero->getState()->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY });
				break;
			}
		}

		//draw apple and check collision
		if (apple->getIsActive())
		{
			if (apple->getCountSteps() == 0)
				apple->reset(cv::Point(hero->getState()->getPhysics()->getTL().x + 50, screen_height - 150));
			apple->setCountSteps(apple->getCountSteps() + 1);
			apple->draw(canvas);
			apple->getState()->update();
			if (apple->getCountSteps() >= 20)
			{
				apple->setIsActive(false);
				apple->setCountSteps(0);
			}
			for (int i = 0; i < slimesPool.getPool().size(); i++)
			{
				if (slimesPool.getIsInUse()[i] && apple->getState()->getPhysics()->checkCollision(slimesPool.getPool()[i]->getState()->getPhysics()))
				{
					apple->setIsActive(false);
					apple->setCountSteps(0);
					slimesPool.returnEntity(slimesPool.getPool()[i]);
					apple->getState()->Register(hearts);
					apple->getState()->Register(score);
					apple->getState()->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_FOOD_LIFE });
					apple->getState()->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_FOOD_SCORE });
					slimesPool.getNext();
					slime->reset(Point(rand() % screen_width, background.size().height * 4 / 5));
				}
			}
		}

		//check collision with walls
		for (int i = 0; i < wallsVector.size(); i++)
		{
			if (!wallsVector[i]->isStandOnMe
				&&
				wallsVector[i]->getState()->getPhysics()->getTL().y <=
				hero->getState()->getPhysics()->getTL().y +
				hero->getState()->getPhysics()->getCollisionMask().size().height
				&&
				wallsVector[i]->getState()->getPhysics()->getTL().y + 50 >=
				hero->getState()->getPhysics()->getTL().y +
				hero->getState()->getPhysics()->getCollisionMask().size().height
				&&
				wallsVector[i]->getState()->getPhysics()->checkCollision(
					hero->getState()->getPhysics()))
			{
				for (int j = 0; j < wallsVector.size(); j++)
				{
					if (i != j)
						wallsVector[j]->isStandOnMe = false;
				}
				wallsVector[i]->isStandOnMe = true;
				hero->reset(Point(hero->getState()->getPhysics()->getTL().x + 2, hero->getState()->getPhysics()->getTL().y));
				hero->getState()->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_WALL });
				break;
			}

			if (wallsVector[i]->isStandOnMe
				&&
				!wallsVector[i]->getState()->getPhysics()->checkCollision(hero->getState()->getPhysics()))

			{
				if (currentKey == KEY_RIGHT || currentKey == KEY_LEFT ||(currentKey == KEY_UP && hero->_isFinishedPhysics))
				{
					wallsVector[i]->isStandOnMe = false;
					hero->reset(Point(hero->getState()->getPhysics()->getTL().x,
						floor));
				}
			}
		}

		//check collision with coins
		for (int i = 0; i < coinsVector.size(); i++)
		{
			if (hero->getState()->getPhysics()->checkCollision(coinsVector[i]->getState()->getPhysics()))
			{
				coinsVector[i]->isEated = true;
				coinsVector[i]->getState()->Register(score);
				coinsVector[i]->getState()->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_FOOD_SCORE });
				coinsVector[i]->reset(Point(-1, -1));
			}
		}		

		imshow("test", canvas);
	}
	return 0;
}