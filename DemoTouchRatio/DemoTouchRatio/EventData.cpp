#include <pch.h>
#include <EventData.h>
#include <Util.h>

#define INVALID_KEY 0
#define TIMEOUT_DURATION 1000

EventData::EventData():
	count(0),
	lastRegisteredEvent(Util::TimestampInMS() - TIMEOUT_DURATION),
	expectingKey(INVALID_KEY)
{ }

int EventData::RegisterEvent() 
{
	if (lastRegisteredEvent + TIMEOUT_DURATION > Util::TimestampInMS())
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

int EventData::Count()
{
	return count;
}