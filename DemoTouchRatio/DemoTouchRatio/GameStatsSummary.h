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

#include "FieldSidesData.h"

#include <vector>
#include <sstream>

class GameStats;

// [STAT_ADD] 18. Add arguments
#define DEFAULT_SUMMARY_ARGS 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,FieldSidesData(0,0,0),FieldSidesData(0,0,0),FieldSidesData(0,0,0),FieldSidesData(0,0,0)

class GameStatsSummary {
public:
	struct SummarizedStats
	{
	public:
		float bumps = 0;
		float teamBumps = 0;
		float demos = 0;
		float ballHits = 0;
		float boostPMinute = 0;
		float totalBoostUsed = 0;
		float inAirPercentage = 0;
		float powerslideCount = 0;
		float powerslideDuration = 0;
		float powerslideDurationPerUse = 0;
		float shots = 0;
		float goals = 0;
		float saves = 0;
		float teamDemos = 0;
		float deaths = 0;
		float assists = 0;
		FieldSidesData boostCollected = FieldSidesData( 0, 0, 0 );
		FieldSidesData boostCollectedPMinute = FieldSidesData( 0, 0, 0 );
		FieldSidesData boostOverfill = FieldSidesData( 0, 0, 0 );
		FieldSidesData boostOverfillPMinute = FieldSidesData( 0, 0, 0 );

		// [STAT_ADD] 15. Add arguments
		SummarizedStats(int bumps, int teamBumps, int demos, int ballHits, float totalBoostUsed, float boostPMinute, float inAirPercentage, int powerslideCount, float powerslideDuration, int shots, int goals, int saves, int teamDemos, int deaths, int assists, FieldSidesData boostCollected, FieldSidesData boostCollectedPMinute, FieldSidesData boostOverfill, FieldSidesData boostOverfillPMinute)
		{

			this->bumps = bumps;
			this->teamBumps = teamBumps;
			this->demos = demos;
			this->ballHits = ballHits;
			this->totalBoostUsed = totalBoostUsed;
			this->boostPMinute = boostPMinute;
			this->inAirPercentage = inAirPercentage;
			this->powerslideCount = powerslideCount;
			this->powerslideDuration = powerslideDuration;
			this->powerslideDurationPerUse = this->powerslideCount != 0.f
				? (this->powerslideDuration / this->powerslideCount)
				: 0.f;
			this->shots = shots;
			this->goals = goals;
			this->saves = saves;
			this->teamDemos = teamDemos;
			this->deaths = deaths;
			this->assists = assists;
			this->boostCollected = boostCollected;
			this->boostCollectedPMinute = boostCollectedPMinute;
			this->boostOverfill = boostOverfill;
			this->boostOverfillPMinute = boostOverfillPMinute;
		}

		// [STAT_ADD] 16. Add arguments
		SummarizedStats(float bumps, float teamBumps, float demos, float ballHits, float totalBoostUsed, float boostPMinute, float inAirPercentage, float powerslideCount, float powerslideDuration, float shots, float goals, float saves, float teamDemos, float deaths, float assists, FieldSidesData boostCollected, FieldSidesData boostCollectedPMinute, FieldSidesData boostOverfill, FieldSidesData boostOverfillPMinute )
		{
			this->bumps = bumps;
			this->teamBumps = teamBumps;
			this->demos = demos;
			this->ballHits = ballHits;
			this->totalBoostUsed = totalBoostUsed;
			this->boostPMinute = boostPMinute;
			this->inAirPercentage = inAirPercentage;
			this->powerslideCount = powerslideCount;
			this->powerslideDuration = powerslideDuration;
			this->powerslideDurationPerUse = this->powerslideCount != 0.f
				? (this->powerslideDuration / this->powerslideCount)
				: 0.f;
			this->shots = shots;
			this->goals = goals;
			this->saves = saves;
			this->teamDemos = teamDemos;
			this->deaths = deaths;
			this->assists = assists;
			this->boostCollected = boostCollected;
			this->boostCollectedPMinute = boostCollectedPMinute;
			this->boostOverfill = boostOverfill;
			this->boostOverfillPMinute = boostOverfillPMinute;
		}

		void Add(SummarizedStats& other, bool isAverage, int oldTotalGames, int totalGames)
		{
			if (totalGames == 0)
				return;

			float otherAddsNOfGames = static_cast<float>(totalGames - oldTotalGames);

			this->bumps = (this->bumps * static_cast<float>(oldTotalGames)) + (other.bumps * otherAddsNOfGames);
			this->teamBumps = (this->teamBumps * static_cast<float>(oldTotalGames)) + (other.teamBumps * otherAddsNOfGames);
			this->demos = (this->demos * static_cast<float>(oldTotalGames)) + (other.demos * otherAddsNOfGames);
			this->ballHits = (this->ballHits * static_cast<float>(oldTotalGames)) + (other.ballHits * otherAddsNOfGames);
			this->totalBoostUsed = (this->totalBoostUsed * static_cast<float>(oldTotalGames)) + (other.totalBoostUsed * otherAddsNOfGames);
			this->boostPMinute = (this->boostPMinute * static_cast<float>(oldTotalGames)) + (other.boostPMinute * otherAddsNOfGames);
			this->inAirPercentage = (this->inAirPercentage * static_cast<float>(oldTotalGames)) + (other.inAirPercentage * otherAddsNOfGames);
			this->powerslideCount = (this->powerslideCount * static_cast<float>(oldTotalGames)) + (other.powerslideCount * otherAddsNOfGames);
			this->powerslideDuration = (this->powerslideDuration * static_cast<float>(oldTotalGames)) + (other.powerslideDuration * otherAddsNOfGames);
			this->powerslideDurationPerUse = ( this->powerslideDurationPerUse * static_cast<float>( oldTotalGames ) ) + ( other.powerslideDurationPerUse * otherAddsNOfGames );
			this->shots = (this->shots * static_cast<float>(oldTotalGames)) + (other.shots * otherAddsNOfGames);
			this->goals = (this->goals * static_cast<float>(oldTotalGames)) + (other.goals * otherAddsNOfGames);
			this->saves = (this->saves * static_cast<float>(oldTotalGames)) + (other.saves * otherAddsNOfGames);
			this->teamDemos = ( this->teamDemos * static_cast<float>( oldTotalGames ) ) + ( other.teamDemos * otherAddsNOfGames );
			this->deaths = ( this->deaths * static_cast<float>( oldTotalGames ) ) + ( other.deaths * otherAddsNOfGames );
			this->assists = (this->assists * static_cast<float>(oldTotalGames)) + (other.assists * otherAddsNOfGames);
			this->boostCollected = ( this->boostCollected * static_cast<float>( oldTotalGames ) ) + ( other.boostCollected * otherAddsNOfGames );
			this->boostCollectedPMinute = ( this->boostCollectedPMinute * static_cast<float>( oldTotalGames ) ) + ( other.boostCollectedPMinute * otherAddsNOfGames );
			this->boostOverfill = ( this->boostOverfill * static_cast<float>( oldTotalGames ) ) + ( other.boostOverfill * otherAddsNOfGames );
			this->boostOverfillPMinute = ( this->boostOverfillPMinute * static_cast<float>( oldTotalGames ) ) + ( other.boostOverfillPMinute * otherAddsNOfGames );

			if (isAverage)
			{
				this->bumps = this->bumps / static_cast<float>(totalGames);
				this->teamBumps = this->teamBumps / static_cast<float>(totalGames);
				this->demos = this->demos / static_cast<float>(totalGames);
				this->ballHits = this->ballHits / static_cast<float>(totalGames);
				this->totalBoostUsed = this->totalBoostUsed / static_cast<float>(totalGames);
				this->powerslideCount = this->powerslideCount / static_cast<float>(totalGames);
				this->powerslideDuration = this->powerslideDuration / static_cast<float>(totalGames);
				this->powerslideDurationPerUse = this->powerslideDurationPerUse / static_cast<float>( totalGames );
				this->shots = this->shots / static_cast<float>(totalGames);
				this->goals = this->goals / static_cast<float>(totalGames);
				this->saves = this->saves / static_cast<float>(totalGames);
				this->teamDemos = this->teamDemos / static_cast<float>( totalGames );
				this->deaths = this->deaths / static_cast<float>( totalGames );
				this->assists = this->assists / static_cast<float>(totalGames);
				this->boostCollected = this->boostCollected / static_cast<float>(totalGames);
				this->boostOverfill = this->boostOverfill / static_cast<float>(totalGames);
			}
			// These are always averages
			this->boostPMinute = this->boostPMinute / static_cast<float>(totalGames);
			this->inAirPercentage = this->inAirPercentage / static_cast<float>(totalGames);
			this->boostCollectedPMinute = this->boostCollectedPMinute / static_cast<float>( totalGames );
			this->boostOverfillPMinute = this->boostOverfillPMinute / static_cast<float>( totalGames );
		}
	};

private:

	int numberOfGames;
	SummarizedStats currentStats;
	SummarizedStats lastStats;
	SummarizedStats totalStats;
	SummarizedStats averageStats;

	SummarizedStats CreateSummaryFrom(GameStats* currentGameStats, float& totalTime, float& totalTimeInAir);

public:
	GameStatsSummary();
	GameStatsSummary(GameStats* currentGameStats, GameStats* lastGameStats, std::vector<GameStats*> previousGameStats);

	void AddTotalAndAverage(GameStatsSummary& other);

	const SummarizedStats& GetCurrent();
	const SummarizedStats& GetLast();
	const SummarizedStats& GetTotal();
	const SummarizedStats& GetAverage();
	int GetNumberOfGames();
};