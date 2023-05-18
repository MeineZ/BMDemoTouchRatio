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

void GameStats::OnBallHit(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	// We care about ball hits from everyone, when A player touches a ball it seems like a global
	// timer is used to delay 2000 continious balltouches.
	int ballHitKey = ballHitData.RegisterEvent(
		DemoTouchRatio::Instance().ShouldMatchAccolades()
		? Util::GetCurrentPing()
		: DemoTouchRatio::Instance().GetCustomBallHitDelay()
	);

	if (!Util::IsLocalPlayer(carWrapper))
		return;

	ballHitData.Bump(ballHitKey);
}

int GameStats::GetBallHits() const {
	return ballHitData.Count();
}

