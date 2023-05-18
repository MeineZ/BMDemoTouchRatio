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

void GameStats::OnEventPickedUp(ActorWrapper actorWrapper, void* args, std::string eventName)
{
	BoostPickupWrapper boostPickupWrapper(actorWrapper.memory_address);
	boostData.RegisterBoostPickup(boostPickupWrapper);
}

void GameStats::HandleBoost(uint64_t deltaTime)
{
	// Do not track boost if delta time is over 200ms (5fps). This means there is some kind of gap between the tracking,
	// allbeit insane amount of lagg a goal that has been scored, you've been demo'd. In all these moments, the physics tick method isn't called,
	// which is extremely useful in this case.
	if (!Util::CanTrack() || deltaTime > 200)
		return;

	CarWrapper car = DemoTouchRatio::GetGameWrapper()->GetLocalCar();
	if (car.IsNull())
		return;

	BoostWrapper boostWrapper = car.GetBoostComponent();
	if (boostWrapper.IsNull())
		return;

	boostData.RegisterBoost(boostWrapper.GetCurrentBoostAmount());
}

float GameStats::GetBoostUsed() const
{
	return boostData.GetTotalBoostUsed();
}

float GameStats::GetBoostPMinute() const
{
	return boostData.GetBoostPMinute(totalPlayedTime);
}

FieldSidesData GameStats::GetBoostCollected() const
{
	return boostData.GetTotalBoostCollected();
}

FieldSidesData GameStats::GetBoostCollectedPMinute() const
{
	return boostData.GetBoostCollectedPMinute(totalPlayedTime);
}

FieldSidesData GameStats::GetBoostOverfill() const
{
	return boostData.GetTotalBoostOverfill();
}

FieldSidesData GameStats::GetBoostOverfillPMinute() const
{
	return boostData.GetBoostOverfillPMinute(totalPlayedTime);
}