#include <pch.h>
#include "GameStatsSummary.h"

#include <GameStats.h>

GameStatsSummary::GameStatsSummary(GameStats* currentGameStats, GameStats* lastGameStats, std::vector<GameStats*> previousGameStats):
	currentStats(SummarizedStats(0, 0, 0, 0)),
	lastStats(SummarizedStats(0, 0, 0, 0)),
	totalStats(SummarizedStats(0, 0, 0, 0)),
	averageStats(SummarizedStats(0, 0, 0, 0))
{
	numberOfGames = 0;
	// Current
	currentStats = CreateSummaryFrom(currentGameStats);
	// Last
	lastStats = CreateSummaryFrom(lastGameStats);

	// Total
	totalStats = CreateSummaryFrom(nullptr);
	totalStats.bumps += currentStats.bumps + lastStats.bumps;
	totalStats.teamBumps += currentStats.teamBumps + lastStats.teamBumps;
	totalStats.demos += currentStats.demos + lastStats.demos;
	totalStats.ballHits += currentStats.ballHits + lastStats.ballHits;

	for (int i = 0; i < previousGameStats.size(); ++i) {
		SummarizedStats newStats = CreateSummaryFrom(previousGameStats[i]);
		totalStats.bumps += newStats.bumps;
		totalStats.teamBumps += newStats.teamBumps;
		totalStats.demos += newStats.demos;
		totalStats.ballHits += newStats.ballHits;
	}

	// Avarage
	if (numberOfGames == 0) {
		averageStats = CreateSummaryFrom(nullptr);
	}
	else {
		averageStats = SummarizedStats {
			(float)totalStats.bumps / (float)numberOfGames,
			(float)totalStats.teamBumps / (float)numberOfGames,
			(float)totalStats.demos / (float)numberOfGames,
			(float)totalStats.ballHits / (float)numberOfGames,
		};
	}
}

GameStatsSummary::SummarizedStats GameStatsSummary::CreateSummaryFrom(GameStats* gameStats) {
	if (gameStats == nullptr) {
		return GameStatsSummary::SummarizedStats{
			0,0,0,0
		};
	}
	else {
		++numberOfGames;
		return GameStatsSummary::SummarizedStats{
			gameStats->GetBumps(), gameStats->GetTeamBumps(), gameStats->GetDemos(), gameStats->GetBallHits()
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

std::string GameStatsSummary::SummarizedStats::toString() const {
	std::stringstream stringStream;
	float biggest = (float)(bumps >= demos && bumps >= ballHits
		? bumps
		: (demos >= bumps && demos >= ballHits
			? demos
			: ballHits));

	if (biggest == 0.f) {
		return "0.00/0.00/0.00";
	}
	stringStream << std::fixed << std::setprecision(2) << (bumps / biggest) << "/" << (demos / biggest) << "/" << (ballHits / biggest);
	return stringStream.str();
}
