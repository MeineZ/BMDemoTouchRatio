#pragma once

#include <string>

class EventData
{
private:
	int count;
	uint64_t lastRegisteredEvent;

	int expectingKey;

public:
	EventData();

	int RegisterEvent();
	void Bump(int key);
	int Count();
};