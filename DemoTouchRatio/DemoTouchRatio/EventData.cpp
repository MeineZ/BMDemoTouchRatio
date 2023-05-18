/*
 * Copyright 2023 MeineZ@GitHub

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

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