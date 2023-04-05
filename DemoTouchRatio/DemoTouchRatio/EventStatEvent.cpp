#include <pch.h>
#include <EventStatEvent.h>

#include <Util.h>

EventStatEvent::EventStatEvent() :
	totalShots(0),
	totalGoals(0),
	totalSaves(0)
{}

EventStatEvent::EventStatEvent(int totalShots, int totalGoals, int totalSaves) :
	totalShots(totalShots),
	totalGoals(totalGoals),
	totalSaves(totalSaves)
{}

void EventStatEvent::RegisterStatTicker(std::string eventName)
{ /* Stat ticker contains the receiver and victim. May be useful for demos or other stats. */ }

void EventStatEvent::RegisterStatEvent(std::string eventName)
{
	if (eventName.compare("Shot") == 0)
	{
		IncreaseShots();
	}
	else if (eventName.compare("Goal") == 0)
	{
		IncreaseGoals();
	}
	else if (eventName.compare("Save") == 0 || eventName.compare("EpicSave") == 0)
	{
		IncreaseSaves();
	}
}

void EventStatEvent::IncreaseShots()
{
	++totalShots;
}

void EventStatEvent::IncreaseGoals()
{
	++totalGoals;
}

void EventStatEvent::IncreaseSaves()
{
	++totalSaves;
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