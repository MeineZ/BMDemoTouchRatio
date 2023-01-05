#pragma once

#include <vector>
#include <sstream>

class GameStats;

class GameStatsSummary {
private:
	struct SummarizedStats {
		float bumps = 0;
		float demos = 0;
		float ballHits = 0;

		SummarizedStats(int bumps, int demos, int ballHits) {
			this->bumps = bumps;
			this->demos = demos;
			this->ballHits = ballHits;
		}

		SummarizedStats(float bumps, float demos, float ballHits) {
			this->bumps = bumps;
			this->demos = demos;
			this->ballHits = ballHits;
		}

		std::string toString() const;
	};

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