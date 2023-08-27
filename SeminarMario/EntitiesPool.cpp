#include "EntitiesPool.h"
#include <algorithm>
using namespace std;

EntitiesPool::EntitiesPool()
{
}

void EntitiesPool::insert(EntityPtr whom)
{
	_pool.push_back(whom);
	_isInUse.push_back(false);
}

EntityPtr EntitiesPool::getNext()
{
	vector<bool>::iterator unUsed = find(_isInUse.begin(), _isInUse.end(), false);//search for the first element that not in use
	if (unUsed == _isInUse.end())//if all in use throw exception
		return nullptr;
		//throw "ERROR! There isn't unused entity!";
	_isInUse[distance(_isInUse.begin(), unUsed)] = true;//do it in use
	return _pool[distance(_isInUse.begin(), unUsed)];
}

void EntitiesPool::returnEntity(EntityPtr toFree)
{
	auto entityToFree = find(_pool.begin(), _pool.end(), toFree);
	if(entityToFree == _pool.end())
		throw "ERROR! Entity not found!";
	_isInUse[distance(_pool.begin(), entityToFree)] = false;
}
