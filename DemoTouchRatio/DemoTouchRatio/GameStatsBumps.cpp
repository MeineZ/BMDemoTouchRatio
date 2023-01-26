#include "pch.h"
#include <GameStats.h>
#include <EventData.h>
#include <DemoTouchRatio.h>

#include "Util.h"
#include "Params.h"

void GameStats::OnBump(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	ACar_TA_execBumpCar_Params* castedParams = (ACar_TA_execBumpCar_Params*)args;
	CarWrapper bumpedCar = CarWrapper(castedParams->bumpedCar);

	// We only register bumps from the player
	if (!Util::IsLocalPlayer(carWrapper) && !Util::IsLocalPlayer(bumpedCar))
		return;

	float delay =
		DemoTouchRatio::Instance().ShouldMatchAccolades()
		? Util::GetCurrentPing()
		: DemoTouchRatio::Instance().GetCustomBumpDelay();

	// Check whether we're allowed to track it and if it's a team bump
	bool canTrackTeamBumps = DemoTouchRatio::Instance().CanTrackTeamBumps();
	bool sameTeam = Util::AreInSameTeam(carWrapper, bumpedCar);
	if (canTrackTeamBumps && sameTeam) {
		int teamBumpKey = teamBumpData.RegisterEvent(delay);
		teamBumpData.Bump(teamBumpKey);
		return;
	}
	// Return if it was a team bump but we're not allowed to track it
	else if (!canTrackTeamBumps && sameTeam)
	{
		return;
	}

	int bumpKey = bumpData.RegisterEvent(delay);
	bumpData.Bump(bumpKey);
}

int GameStats::GetBumps() const {
	return bumpData.Count();
}
int GameStats::GetTeamBumps() const {
	return teamBumpData.Count();
}

