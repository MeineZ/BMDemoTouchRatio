#pragma once

#include <GameStats.h>
#include <GameStatsSummary.h>

class PersistentStats {
private:
	GameStats total;
	GameStatsSummary::SummarizedStats average;
	int nOfGames;

	std::filesystem::path GetFullPath();
	void LoadFile();
	bool UpdateFile();

	void UpdateAverageStats();

public:
	PersistentStats();

	void Initialize();
	const GameStats& Update(GameStats* stats);
	void Clear();

	const GameStats& GetTotalStats() const;
	const GameStatsSummary::SummarizedStats& GetAverageStats() const;
	int GetNumberOfGames() const;

};