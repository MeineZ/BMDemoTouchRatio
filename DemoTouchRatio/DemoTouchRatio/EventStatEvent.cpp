#include <pch.h>
#include <EventStatEvent.h>

#include <Util.h>

// https://github.com/ubelhj/OBSCounter/blob/main/OBSCounter/Maps.h#L341

EventStatEvent::EventStatEvent() :
	demos(0),
	teamDemos(0),
	deaths(0),
	totalShots(0),
	totalGoals(0),
	totalSaves(0)
{}

EventStatEvent::EventStatEvent(int demos, int teamDemos, int deaths, int totalShots, int totalGoals, int totalSaves) :
	demos(demos),
	teamDemos(teamDemos),
	deaths(deaths),
	totalShots(totalShots),
	totalGoals(totalGoals),
	totalSaves(totalSaves)
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