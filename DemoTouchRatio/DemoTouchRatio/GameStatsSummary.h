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
	public:
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

			if (isAverage)
			{
				this->bumps = this->bumps / static_cast<float>(totalGames);
				this->teamBumps = this->teamBumps / static_cast<float>(totalGames);
				this->demos = this->demos / static_cast<float>(totalGames);
				this->ballHits = this->ballHits / static_cast<float>(totalGames);
				this->totalBoostUsed = this->totalBoostUsed / static_cast<float>(totalGames);
			}
			// These are always averages
			this->boostPMinute = this->boostPMinute / static_cast<float>(totalGames);
			this->inAirPercentage = this->inAirPercentage / static_cast<float>(totalGames);
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