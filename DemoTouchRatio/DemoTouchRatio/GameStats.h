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

#pragma once

#include <EventData.h>
#include <EventBoost.h>
#include <EventInAir.h>
#include <EventPowerslide.h>
#include <EventStatEvent.h>

class GameStats
{
private:

	uint64_t lastTimeStamp;
	float totalPlayedTime;

	// [STAT_ADD] 5. Add data tracking property
	EventData bumpData;
	EventData teamBumpData;
	EventData ballHitData;
	EventBoost boostData;
	EventInAir inAirData;
	EventPowerslide powerslideData;
	EventStatEvent statEventData;

	// [STAT_ADD] 6. Add data tracking method
	void OnBump(CarWrapper carWrapper, void* args, std::string eventName);
	void OnBallHit(CarWrapper carWrapper, void* args, std::string eventName);
	void OnCarWorldHit(CarWrapper carWrapper, void* args, std::string eventName);
	void OnStatTicker(ServerWrapper serverWrapper, void* args, std::string eventName);
	void OnStatEvent(ServerWrapper serverWrapper, void* args, std::string eventName);
	void OnEventPickedUp(ActorWrapper actorWrapper, void* args, std::string eventName);

	void OnPhysicsTick(std::string eventName);
	void HandleBoost(uint64_t deltaTime);
	void HandleInAir(uint64_t deltaTime);
	void HandlePowerslide(uint64_t deltaTime);

public:
	GameStats();
	const GameStats(int bumps, int teamBumps, int demos, int ballHits, float totalTime, float totalBoost, float airTimeInMinutes, int powerslideCount, float powerslideDuration, int shots, int goals, int saves, int teamDemos, int deaths, int assists, FieldSidesData boostCollected, FieldSidesData boostOverfill );

	void BindEvents();
	void UnbindEvents();

	float GetTotalPlayedTime() const;

	// [STAT_ADD] 7. Add data getter
	int GetBumps() const;
	int GetTeamBumps() const;
	int GetDemos() const;
	int GetTeamDemos() const;
	int GetDeaths() const;
	int GetBallHits() const;
	float GetBoostUsed() const;
	float GetBoostPMinute() const;
	FieldSidesData GetBoostCollected() const;
	FieldSidesData GetBoostCollectedPMinute() const;
	FieldSidesData GetBoostOverfill() const;
	FieldSidesData GetBoostOverfillPMinute() const;
	float GetTimeInAir() const;
	float GetInAirPercentage() const;
	int GetPowerslideCount() const;
	float GetPowerslideTimeInMinutes() const;
	float GetPowerslideTimeInSeconds() const;
	float GetPowerslideTimePerUseInSeconds() const;
	int GetShots() const;
	int GetGoals() const;
	int GetSaves() const;
	int GetAssists() const;
};