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

int EventData::RegisterEvent() 
{
	DEBUGLOG("MS {} ,, LAST {}", Util::TimestampInMS(), lastRegisteredEvent);
	if (lastRegisteredEvent + TIMEOUT_DURATION > Util::TimestampInMS())
		return INVALID_KEY;

	lastRegisteredEvent = Util::TimestampInMS();

	++expectingKey;
	if (INVALID_KEY == expectingKey)
	{
		++expectingKey;
	}

	DEBUGLOG("EXPECTING KEY {}", expectingKey);
	return expectingKey;
}

void EventData::Bump(int key)
{
	DEBUGLOG("RECEIVED KEY {} ,, EXPECTING {}", key, expectingKey);
	if (key == INVALID_KEY || expectingKey != key)
		return;

	++count;
}

int EventData::Count()
{
	return count;
}