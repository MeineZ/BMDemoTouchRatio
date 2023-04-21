#include "pch.h"
#include <GameStats.h>

int GameStats::GetDemos() const {
	return statEventData.GetDemos();
}

int GameStats::GetTeamDemos() const
{
	return statEventData.GetTeamDemos();
}

int GameStats::GetDeaths() const
{
	return statEventData.GetDeaths();
}

