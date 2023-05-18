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
#include <EventStatEvent.h>

#include <Util.h>

// https://github.com/ubelhj/OBSCounter/blob/main/OBSCounter/Maps.h#L341

// Found Events:
//  - Center
//  - Clear
//  - HatTrick
//  - AerialHit
//  - LongGoal
//  - FirstTouch

EventStatEvent::EventStatEvent() :
	demos(0),
	teamDemos(0),
	deaths(0),
	totalShots(0),
	totalGoals(0),
	totalSaves(0),
	totalAssists(0)
{}

EventStatEvent::EventStatEvent(int demos, int teamDemos, int deaths, int totalShots, int totalGoals, int totalSaves, int totalAssists) :
	demos(demos),
	teamDemos(teamDemos),
	deaths(deaths),
	totalShots(totalShots),
	totalGoals(totalGoals),
	totalSaves(totalSaves),
	totalAssists(totalAssists)
{}

void EventStatEvent::RegisterStatTicker(std::string eventName, CarWrapper receiver, PriWrapper victim)
{
	if (eventName.compare("Demolish") == 0)
	{
		HandleDemolition(receiver, victim);
	}
}

void EventStatEvent::RegisterStatEvent(std::string eventName)
{
	if (eventName.compare("Shot") == 0)
	{
		HandleShots();
	}
	else if (eventName.compare("Goal") == 0)
	{
		HandleGoals();
	}
	else if (eventName.compare("Save") == 0 || eventName.compare("EpicSave") == 0)
	{
		HandleSaves();
	}
	else if( eventName.compare( "Assist" ) == 0 )
	{
		HandleAssists();
	}
}

void EventStatEvent::HandleShots()
{
	++totalShots;
}

void EventStatEvent::HandleGoals()
{
	++totalGoals;
}

void EventStatEvent::HandleSaves()
{
	++totalSaves;
}

void EventStatEvent::HandleAssists()
{
	++totalAssists;
}

int EventStatEvent::GetShots() const
{
	return totalShots;
}

int EventStatEvent::GetGoals() const
{
	return totalGoals;
}

int EventStatEvent::GetSaves() const
{
	return totalSaves;
}

int EventStatEvent::GetAssists() const
{
	return totalAssists;
}