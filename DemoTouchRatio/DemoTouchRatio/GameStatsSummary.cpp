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
#include "GameStatsSummary.h"

#include <GameStats.h>

GameStatsSummary::GameStatsSummary() :
	currentStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	lastStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	totalStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	averageStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	numberOfGames(0)
{}

GameStatsSummary::GameStatsSummary(GameStats* currentGameStats, GameStats* lastGameStats, std::vector<GameStats*> previousGameStats):
	currentStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	lastStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	totalStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	averageStats(SummarizedStats(DEFAULT_SUMMARY_ARGS))
{
	numberOfGames = 0;
	// Current
	float totalTimePlayed = 0.f;
	float totalTimeInAir = 0.f;
	currentStats = CreateSummaryFrom(currentGameStats, totalTimePlayed, totalTimeInAir);
	// Last
	lastStats = CreateSummaryFrom(lastGameStats, totalTimePlayed, totalTimeInAir);

	// Total
	// [STAT_ADD] 19. Add data
	totalStats = CreateSummaryFrom(nullptr, totalTimePlayed, totalTimeInAir);
	totalStats.bumps += currentStats.bumps + lastStats.bumps;
	totalStats.teamBumps += currentStats.teamBumps + lastStats.teamBumps;
	totalStats.demos += currentStats.demos + lastStats.demos;
	totalStats.teamDemos += currentStats.teamDemos + lastStats.teamDemos;
	totalStats.deaths += currentStats.deaths + lastStats.deaths;
	totalStats.ballHits += currentStats.ballHits + lastStats.ballHits;
	totalStats.totalBoostUsed += currentStats.totalBoostUsed + lastStats.totalBoostUsed;
	totalStats.boostPMinute += currentStats.boostPMinute + lastStats.boostPMinute;
	totalStats.inAirPercentage += currentStats.inAirPercentage + lastStats.inAirPercentage;
	totalStats.powerslideCount += currentStats.powerslideCount + lastStats.powerslideCount;
	totalStats.powerslideDuration += currentStats.powerslideDuration + lastStats.powerslideDuration;
	totalStats.shots += currentStats.shots + lastStats.shots;
	totalStats.goals += currentStats.goals + lastStats.goals;
	totalStats.saves += currentStats.saves + lastStats.saves;
	totalStats.assists += currentStats.assists + lastStats.assists;
	totalStats.boostCollected += currentStats.boostCollected + lastStats.boostCollected;
	totalStats.boostCollectedPMinute += currentStats.boostCollectedPMinute + lastStats.boostCollectedPMinute;
	totalStats.boostOverfill += currentStats.boostOverfill + lastStats.boostOverfill;
	totalStats.boostOverfillPMinute += currentStats.boostOverfillPMinute + lastStats.boostOverfillPMinute;

	for (int i = 0; i < previousGameStats.size(); ++i) {
		SummarizedStats newStats = CreateSummaryFrom(previousGameStats[i], totalTimePlayed, totalTimeInAir);
		// [STAT_ADD] 20. Add data
		totalStats.bumps += newStats.bumps;
		totalStats.teamBumps += newStats.teamBumps;
		totalStats.demos += newStats.demos;
		totalStats.teamDemos += newStats.teamDemos;
		totalStats.deaths += newStats.deaths;
		totalStats.ballHits += newStats.ballHits;
		totalStats.totalBoostUsed += newStats.totalBoostUsed;
		totalStats.boostPMinute += newStats.boostPMinute;
		totalStats.inAirPercentage += newStats.inAirPercentage;
		totalStats.powerslideCount += newStats.powerslideCount;
		totalStats.powerslideDuration += newStats.powerslideDuration;
		totalStats.shots += newStats.shots;
		totalStats.goals += newStats.goals;
		totalStats.saves += newStats.saves;
		totalStats.assists += newStats.assists;
		totalStats.boostCollected += newStats.boostCollected;
		totalStats.boostCollectedPMinute += newStats.boostCollectedPMinute;
		totalStats.boostOverfill += newStats.boostOverfill;
		totalStats.boostOverfillPMinute += newStats.boostOverfillPMinute;
	}

	// Avarage
	if (numberOfGames == 0) {
		averageStats = CreateSummaryFrom(nullptr, totalTimePlayed, totalTimeInAir);
	}
	else {
		float averageAirTime = 0.f;
		if (totalTimePlayed != 0.f)
		{
			averageAirTime = totalTimeInAir / totalTimePlayed;
		}

		averageStats = SummarizedStats {
			totalStats.bumps / (float)numberOfGames,
			totalStats.teamBumps / (float)numberOfGames,
			totalStats.demos / (float)numberOfGames,
			totalStats.ballHits / (float)numberOfGames,
			totalStats.totalBoostUsed / (float)numberOfGames,
			totalStats.boostPMinute / (float)numberOfGames,
			totalStats.inAirPercentage / (float)numberOfGames,
			totalStats.powerslideCount / (float)numberOfGames,
			totalStats.powerslideDuration / (float)numberOfGames,
			totalStats.shots / (float)numberOfGames,
			totalStats.goals / (float)numberOfGames,
			totalStats.saves / (float)numberOfGames,
			totalStats.teamDemos / (float) numberOfGames,
			totalStats.deaths / (float) numberOfGames,
			totalStats.assists / (float)numberOfGames,
			totalStats.boostCollected / (float) numberOfGames,
			totalStats.boostCollectedPMinute / (float) numberOfGames,
			totalStats.boostOverfill / (float)numberOfGames,
			totalStats.boostOverfillPMinute / (float)numberOfGames,
		};
	}

	// Some stats can't have totals, so they'll just display the average.
	totalStats.boostPMinute = averageStats.boostPMinute;
	totalStats.inAirPercentage = averageStats.inAirPercentage;
	totalStats.boostCollectedPMinute = averageStats.boostCollectedPMinute;
	totalStats.boostOverfillPMinute = averageStats.boostOverfillPMinute;
}

GameStatsSummary::SummarizedStats GameStatsSummary::CreateSummaryFrom(GameStats* gameStats, float& totalTime, float& totalTimeInAir) {
	if (gameStats == nullptr) {
		return GameStatsSummary::SummarizedStats{
			DEFAULT_SUMMARY_ARGS
		};
	}
	else {
		++numberOfGames;
		totalTime += gameStats->GetTotalPlayedTime();
		totalTimeInAir += gameStats->GetTimeInAir();

		return GameStatsSummary::SummarizedStats{
			gameStats->GetBumps(), gameStats->GetTeamBumps(), gameStats->GetDemos(), 
			gameStats->GetBallHits(), gameStats->GetBoostUsed(), gameStats->GetBoostPMinute(), gameStats->GetInAirPercentage(),
			gameStats->GetPowerslideCount(), gameStats->GetPowerslideTimeInMinutes(), gameStats->GetShots(),
			gameStats->GetGoals(), gameStats->GetSaves(), gameStats->GetTeamDemos(), gameStats->GetDeaths(), gameStats->GetAssists(),
			gameStats->GetBoostCollected(), gameStats->GetBoostCollectedPMinute(), gameStats->GetBoostOverfill(), gameStats->GetBoostOverfillPMinute()
		};
	}
}

void GameStatsSummary::AddTotalAndAverage(GameStatsSummary& other)
{
	if (numberOfGames == 0 && other.numberOfGames == 0)
		return;

	int oldNOfGames = numberOfGames;
	numberOfGames += other.numberOfGames;

	totalStats.Add(other.totalStats, false, oldNOfGames, numberOfGames);
	averageStats.Add(other.averageStats, true, oldNOfGames, numberOfGames);
}

const GameStatsSummary::SummarizedStats& GameStatsSummary::GetCurrent()
{
	return currentStats;
}

const GameStatsSummary::SummarizedStats& GameStatsSummary::GetLast()
{
	return lastStats;
}

const GameStatsSummary::SummarizedStats& GameStatsSummary::GetTotal()
{
	return totalStats;
}

const GameStatsSummary::SummarizedStats& GameStatsSummary::GetAverage()
{
	return averageStats;
}

int GameStatsSummary::GetNumberOfGames()
{
	return numberOfGames;
}
