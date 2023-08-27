#pragma once
#include "Observer.h"
#include "Entities.h"

class Timer : public Subject
{
private:
	size_t _frequency_ms;

public:
	Timer(size_t frequency_ms);

	int tick();
};

