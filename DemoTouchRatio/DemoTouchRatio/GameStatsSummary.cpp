#include <pch.h>
#include "GameStatsSummary.h"

#include <GameStats.h>

GameStatsSummary::GameStatsSummary(GameStats* currentGameStats, GameStats* lastGameStats, std::vector<GameStats*> previousGameStats):
	currentStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	lastStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	totalStats(SummarizedStats(DEFAULT_SUMMARY_ARGS)),
	averageStats(SummarizedStats(DEFAULT_SUMMARY_ARGS))
{
	numberOfGames = 0;
	// Current
	currentStats = CreateSummaryFrom(currentGameStats);
	// Last
	lastStats = CreateSummaryFrom(lastGameStats);

	// Total
	// [STAT_ADD] 19. Add data
	totalStats = CreateSummaryFrom(nullptr);
	totalStats.bumps += currentStats.bumps + lastStats.bumps;
	totalStats.teamBumps += currentStats.teamBumps + lastStats.teamBumps;
	totalStats.demos += currentStats.demos + lastStats.demos;
	totalStats.ballHits += currentStats.ballHits + lastStats.ballHits;
	totalStats.totalBoostUsed += currentStats.totalBoostUsed + lastStats.totalBoostUsed;
	totalStats.boostPMinute += currentStats.boostPMinute + lastStats.boostPMinute;

	for (int i = 0; i < previousGameStats.size(); ++i) {
		SummarizedStats newStats = CreateSummaryFrom(previousGameStats[i]);
		// [STAT_ADD] 20. Add data
		totalStats.bumps += newStats.bumps;
		totalStats.teamBumps += newStats.teamBumps;
		totalStats.demos += newStats.demos;
		totalStats.ballHits += newStats.ballHits;
		totalStats.totalBoostUsed += newStats.totalBoostUsed;
		totalStats.boostPMinute += newStats.boostPMinute;
	}

	// Avarage
	if (numberOfGames == 0) {
		averageStats = CreateSummaryFrom(nullptr);
	}
	else {
		averageStats = SummarizedStats {
			totalStats.bumps / (float)numberOfGames,
			totalStats.teamBumps / (float)numberOfGames,
			totalStats.demos / (float)numberOfGames,
			totalStats.ballHits / (float)numberOfGames,
			totalStats.totalBoostUsed / (float)numberOfGames,
			totalStats.boostPMinute / (float)numberOfGames
		};
	}
}

GameStatsSummary::SummarizedStats GameStatsSummary::CreateSummaryFrom(GameStats* gameStats) {
	if (gameStats == nullptr) {
		return GameStatsSummary::SummarizedStats{
			DEFAULT_SUMMARY_ARGS
		};
	}
	else {
		++numberOfGames;
		return GameStatsSummary::SummarizedStats{
			gameStats->GetBumps(), gameStats->GetTeamBumps(), gameStats->GetDemos(), gameStats->GetBallHits(),
			gameStats->GetBoostUsed(), gameStats->GetBoostPMinute()
		};
	}
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
