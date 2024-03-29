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

void GameStats::OnStatTicker(ServerWrapper serverWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	AGFxHUD_TA_execStatTicker_Params* castedParams = (AGFxHUD_TA_execStatTicker_Params*)args;
	PriWrapper statReceiver = PriWrapper(castedParams->Receiver);
	PriWrapper statVictim = PriWrapper(castedParams->Victim);
	if (statReceiver.IsNull())
		return;

	CarWrapper receiverCar = statReceiver.GetCar();
	if (receiverCar.IsNull())
		return;

	// We allow any stat to be processed. Each stat should check if its the local player on their own.
	StatEventWrapper statEvent = StatEventWrapper(castedParams->StatEvent);
	if( statEvent.IsNull() )
		return;

	statEventData.RegisterStatTicker(statEvent.GetEventName(), receiverCar, statVictim);
}

void GameStats::OnStatEvent(ServerWrapper serverWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	AGFxHUD_TA_execStatEvent_Params* castedParams = (AGFxHUD_TA_execStatEvent_Params*)args;
	PriWrapper statReceiver = PriWrapper(castedParams->PRI);
	if (statReceiver.IsNull())
		return;

	CarWrapper receiverCar = statReceiver.GetCar();
	if (receiverCar.IsNull())
		return;

	// We only register demos from the player
	if (!Util::IsLocalPlayer(receiverCar))
		return;

	StatEventWrapper statEvent = StatEventWrapper(castedParams->StatEvent);
	if (statEvent.IsNull())
		return;

	statEventData.RegisterStatEvent(statEvent.GetEventName());
}

int GameStats::GetShots() const
{
	return statEventData.GetShots();
}

int GameStats::GetGoals() const
{
	return statEventData.GetGoals();
}

int GameStats::GetSaves() const
{
	return statEventData.GetSaves();
}

int GameStats::GetAssists() const
{
	return statEventData.GetAssists();
}