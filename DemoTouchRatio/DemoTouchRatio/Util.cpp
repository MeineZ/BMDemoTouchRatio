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

#include <chrono>
#include <cstdint>
#include <iostream>
#include <pch.h>

#include "Util.h"
#include "Constants.h"
#include "DemoTouchRatio.h"

CarWrapper Util::GetLocalPlayer()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return CarWrapper(0);

	return gameWrapper->GetLocalCar();
}

bool Util::IsLocalPlayer(CarWrapper& carWrapper)
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return false;

	if (carWrapper.IsNull())
		return false;

	if (gameWrapper->GetLocalCar().IsNull())
		return false;

	return carWrapper.memory_address == gameWrapper->GetLocalCar().memory_address;
}

float Util::GetLocalBoost()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return 0.0f;

	CarWrapper carWrapper = gameWrapper->GetLocalCar();
	if (carWrapper.IsNull())
		return 0.0f;

	BoostWrapper boostWrapper = carWrapper.GetBoostComponent();
	if (boostWrapper.IsNull())
		return 0.0f;

	return boostWrapper.GetCurrentBoostAmount();
}

bool Util::CanTrack()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return false;

	if (gameWrapper->IsInReplay() || gameWrapper->IsInFreeplay())
		return false;

	if (gameWrapper->IsInOnlineGame())
	{
		ServerWrapper serverWrapper = gameWrapper->GetOnlineGame();
		return serverWrapper.IsOnlineMultiplayer();
	}
	else if(gameWrapper->IsInGame())
	{
		return true;
	}
	return false;
}

int Util::GetCurrentPing()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	CarWrapper localCar = gameWrapper->GetLocalCar();
	if (localCar.IsNull())
		return 0;

	PriWrapper priWrapper = localCar.GetPRI();
	if (priWrapper.IsNull())
		return 0;

	return ((int)priWrapper.GetPing()) * 4;
}

uint64_t Util::TimestampInMS()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

bool Util::AreInSameTeam(CarWrapper carA, CarWrapper carB) {
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return false;

	if (gameWrapper->GetLocalCar().IsNull())
		return false;

	if (carA.IsNull() || carB.IsNull())
		return false;

	return carA.GetTeamNum2() == carB.GetTeamNum2();
}

PlaylistType Util::CurrentPlaylist()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return PlaylistType::PLAYLIST_UNKNOWN;

	MMRWrapper mmrWrapper = gameWrapper->GetMMRWrapper();
	int id = mmrWrapper.GetCurrentPlaylist();

	return PlaylistHelpers::ConvertToPluginPlaylist(PlaylistHelpers::GetPlaylistFromInt(id));
}