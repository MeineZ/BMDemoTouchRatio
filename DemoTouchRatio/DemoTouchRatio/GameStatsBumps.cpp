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

