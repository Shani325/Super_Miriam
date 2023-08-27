#include "Observer.h"
#include <iostream>
#include <algorithm>
using namespace std;

void Subject::Register(IObserverPtr o)
{
	if(_observers.end() == find(_observers.begin(), _observers.end() ,o))
		_observers.push_back(o);
}

void Subject::Notify(Event const& e)
{
	for (auto& o : _observers)
	{
		o->onNotify(e);
	}
}

#define COMPARE_EXPLICIT(a,b) if ((a)<(b)) return true; else if ((a)>(b)) return false;
bool Event::operator<(Event const& other) const
{
	COMPARE_EXPLICIT(sender, other.sender);
	COMPARE_EXPLICIT(code, other.code);
	COMPARE_EXPLICIT(type, other.type);
	return false;
}
