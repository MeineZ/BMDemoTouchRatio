#pragma once

#include <vector>
#include <sstream>

class GameStats;

// [STAT_ADD] 18. Add arguments
#define DEFAULT_SUMMARY_ARGS 0,0,0,0,0,0,0

class GameStatsSummary {
public:
	struct SummarizedStats
	{
		float bumps = 0;
		float teamBumps = 0;
		float demos = 0;
		float ballHits = 0;
		float boostPMinute = 0;
		float totalBoostUsed = 0;
		float inAirPercentage = 0;

		// [STAT_ADD] 15. Add arguments
		SummarizedStats(int bumps, int teamBumps, int demos, int ballHits, float totalBoostUsed, float boostPMinute, float inAirPercentage)
		{
			this->bumps = bumps;
			this->teamBumps = teamBumps;
			this->demos = demos;
			this->ballHits = ballHits;
			this->totalBoostUsed = totalBoostUsed;
			this->boostPMinute = boostPMinute;
			this->inAirPercentage = inAirPercentage;
		}

		// [STAT_ADD] 16. Add arguments
		SummarizedStats(float bumps, float teamBumps, float demos, float ballHits, float totalBoostUsed, float boostPMinute, float inAirPercentage)
		{
			this->bumps = bumps;
			this->teamBumps = teamBumps;
			this->demos = demos;
			this->ballHits = ballHits;
			this->totalBoostUsed = totalBoostUsed;
			this->boostPMinute = boostPMinute;
			this->inAirPercentage = inAirPercentage;
		}
	};

private:

	int numberOfGames;
	SummarizedStats currentStats;
	SummarizedStats lastStats;
	SummarizedStats totalStats;
	SummarizedStats averageStats;

	SummarizedStats CreateSummaryFrom(GameStats* currentGameStats);

public:
	GameStatsSummary(GameStats* currentGameStats, GameStats* lastGameStats, std::vector<GameStats*> previousGameStats);

	const SummarizedStats& GetCurrent();
	const SummarizedStats& GetLast();
	const SummarizedStats& GetTotal();
	const SummarizedStats& GetAverage();
	int GetNumberOfGames();
};