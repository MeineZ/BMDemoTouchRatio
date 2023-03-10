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
	EventData(int initialCount);

	int RegisterEvent(int timeoutInMS);
	void Bump(int key);
	int Count() const;
};