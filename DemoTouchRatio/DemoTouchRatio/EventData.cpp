#include <pch.h>
#include <EventData.h>
#include <Util.h>

#define INVALID_KEY 0
#define TIMEOUT_DURATION 1000

EventData::EventData():
	count(0),
	lastRegisteredEvent(0),
	expectingKey(INVALID_KEY)
{ }

EventData::EventData(int initialCount) :
	count(initialCount),
	lastRegisteredEvent(0),
	expectingKey(INVALID_KEY)
{ }

int EventData::RegisterEvent(int timeoutInMS) 
{
	if (lastRegisteredEvent + timeoutInMS > Util::TimestampInMS())
		return INVALID_KEY;

	lastRegisteredEvent = Util::TimestampInMS();

	++expectingKey;
	if (INVALID_KEY == expectingKey)
	{
		++expectingKey;
	}

	return expectingKey;
}

void EventData::Bump(int key)
{
	if (key == INVALID_KEY || expectingKey != key)
		return;

	++count;
}

int EventData::Count() const
{
	return count;
}