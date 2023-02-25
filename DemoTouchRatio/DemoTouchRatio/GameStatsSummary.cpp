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
	totalStats.ballHits += currentStats.ballHits + lastStats.ballHits;
	totalStats.totalBoostUsed += currentStats.totalBoostUsed + lastStats.totalBoostUsed;
	totalStats.boostPMinute += currentStats.boostPMinute + lastStats.boostPMinute;
	totalStats.inAirPercentage += currentStats.inAirPercentage + lastStats.inAirPercentage;
	totalStats.powerslideCount += currentStats.powerslideCount + lastStats.powerslideCount;
	totalStats.powerslideDuration += currentStats.powerslideDuration + lastStats.powerslideDuration;

	for (int i = 0; i < previousGameStats.size(); ++i) {
		SummarizedStats newStats = CreateSummaryFrom(previousGameStats[i], totalTimePlayed, totalTimeInAir);
		// [STAT_ADD] 20. Add data
		totalStats.bumps += newStats.bumps;
		totalStats.teamBumps += newStats.teamBumps;
		totalStats.demos += newStats.demos;
		totalStats.ballHits += newStats.ballHits;
		totalStats.totalBoostUsed += newStats.totalBoostUsed;
		totalStats.boostPMinute += newStats.boostPMinute;
		totalStats.inAirPercentage += newStats.inAirPercentage;
		totalStats.powerslideCount += newStats.powerslideCount;
		totalStats.powerslideDuration += newStats.powerslideDuration;
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
			totalStats.powerslideDuration / (float)numberOfGames
		};
	}

	// Some stats can't have totals, so they'll just display the average.
	totalStats.boostPMinute = averageStats.boostPMinute;
	totalStats.inAirPercentage = averageStats.inAirPercentage;
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
			gameStats->GetBumps(), gameStats->GetTeamBumps(), gameStats->GetDemos(), gameStats->GetBallHits(),
			gameStats->GetBoostUsed(), gameStats->GetBoostPMinute(), gameStats->GetInAirPercentage(),
			gameStats->GetPowerslideCount(), gameStats->GetPowerslideTimeInMinutes()
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
