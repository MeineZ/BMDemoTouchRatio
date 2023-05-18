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
#include <GameStats.h>

#include <Util.h>

void GameStats::HandleInAir(uint64_t deltaTime)
{
	// Do not track boost if delta time is over 200ms (5fps). This means there is some kind of gap between the tracking,
	// allbeit insane amount of lagg a goal that has been scored, you've been demo'd. In all these moments, the physics tick method isn't called,
	// which is extremely useful in this case.
	if (!Util::CanTrack() || deltaTime > 200)
		return;

	inAirData.RegisterAirTime(deltaTime);
}

void GameStats::OnCarWorldHit(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack() || !Util::IsLocalPlayer(carWrapper))
		return;

	inAirData.OnCarHitWorld();
}

float GameStats::GetTimeInAir() const
{
	return inAirData.GetTimeInAir();
}

float GameStats::GetInAirPercentage() const
{
	return inAirData.GetInAirPercentage(totalPlayedTime);
}