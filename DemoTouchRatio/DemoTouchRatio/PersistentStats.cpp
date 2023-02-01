#include <pch.h>
#include <PersistentStats.h>

#include <fstream>
#include <DemoTouchRatio.h>
#include <logging.h>

#define STATS_FOLDER "demotouchdata"
#define STATS_FILENAME "demotouchstats"
#define STATS_FILEEXTENSION ".csv"

// [STAT_ADD] 17. Add arguments
#define STATS_COUNT 8

PersistentStats::PersistentStats() :
	suffix(""),
	total(),
	average(DEFAULT_SUMMARY_ARGS),
	nOfGames(0)
{ }

void PersistentStats::Initialize(std::string fileSuffix)
{
	total = GameStats();
	average = GameStatsSummary::SummarizedStats(DEFAULT_SUMMARY_ARGS);
	nOfGames = 0;
	suffix = fileSuffix;

	LoadFile();
}

const GameStats& PersistentStats::Update(GameStats* stats)
{
	if (stats == nullptr)
		return total;

	++nOfGames;
	// [STAT_ADD] 11. Add arguments
	total = GameStats(
		total.GetBumps() + stats->GetBumps(),
		total.GetTeamBumps() + stats->GetTeamBumps(),
		total.GetDemos() + stats->GetDemos(),
		total.GetBallHits() + stats->GetBallHits(),
		total.GetTotalPlayedTime() + stats->GetTotalPlayedTime(),
		total.GetBoostUsed() + stats->GetBoostUsed(),
		total.GetTimeInAir() + stats->GetTimeInAir()
	);

	// Revert if file update failed
	if (!UpdateFile()) {
		--nOfGames;

		// [STAT_ADD] 12. Add arguments
		total = GameStats(
			total.GetBumps() - stats->GetBumps(),
			total.GetTeamBumps() - stats->GetTeamBumps(),
			total.GetDemos() - stats->GetDemos(),
			total.GetBallHits() - stats->GetBallHits(),
			total.GetTotalPlayedTime() - stats->GetTotalPlayedTime(),
			total.GetBoostUsed() - stats->GetBoostUsed(),
			total.GetTimeInAir() - stats->GetTimeInAir()
		);
	}

	UpdateAverageStats();

	return total;
}

void PersistentStats::UpdateAverageStats()
{
	if (nOfGames == 0)
	{
		average = GameStatsSummary::SummarizedStats(DEFAULT_SUMMARY_ARGS);
		return;
	}

	// [STAT_ADD] 13. Add arguments
	average = GameStatsSummary::SummarizedStats(
		(float)total.GetBumps() / (float)nOfGames, 
		(float)total.GetTeamBumps() / (float)nOfGames,
		(float)total.GetDemos() / (float)nOfGames,
		(float)total.GetBallHits() / (float)nOfGames,
		total.GetBoostUsed() / (float)nOfGames,
		total.GetBoostPMinute(),
		total.GetInAirPercentage());
}

void PersistentStats::Clear()
{
	nOfGames = 0;
	// [STAT_ADD] 14. Add arguments
	total = GameStats(0, 0, 0, 0, 0, 0, 0);
	average = GameStatsSummary::SummarizedStats(DEFAULT_SUMMARY_ARGS);
	// Update file
	UpdateFile();
}

const GameStats& PersistentStats::GetTotalStats() const {
	return total;
}

const GameStatsSummary::SummarizedStats& PersistentStats::GetAverageStats() const
{
	return average;
}

int PersistentStats::GetNumberOfGames() const
{
	return nOfGames;
}

std::filesystem::path PersistentStats::GetFullPath()
{
	std::filesystem::path filePath = DemoTouchRatio::Instance().GetGameWrapper()->GetDataFolder() / STATS_FOLDER;
	std::filesystem::path fullPath = filePath / (STATS_FILENAME + suffix + STATS_FILEEXTENSION);
	if (!std::filesystem::exists(fullPath)) {
		std::filesystem::create_directories(filePath);
	}

	return fullPath;
}

void PersistentStats::LoadFile()
{
	try {
		// Load content of file
		std::ifstream in(GetFullPath());
		int stats[STATS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0};
		if (in) {
			char comma;
			int count = 0;
			while (in >> stats[count]) {
				if (in >> comma) {
					count++;
				}
				else {
					break;
				}

				if (count >= STATS_COUNT) {
					break;
				}
			}

			in.close();
		}

		nOfGames = stats[0];
		total = GameStats(
			stats[1], stats[2], stats[3], stats[4],
			reinterpret_cast<float&>(stats[5]),
			reinterpret_cast<float&>(stats[6]),
			reinterpret_cast<float&>(stats[7])
		);
		UpdateAverageStats();
	}
	catch (std::exception ex) {
		DEBUGLOG("[FILESYSTEM_LOADFLAG]");
		DEBUGLOG(ex.what());
	}
}

bool PersistentStats::UpdateFile()
{
	try {
		// Update content of file
		std::ofstream out(GetFullPath(), std::ofstream::trunc);
		if (out) {
			float totalPlayedTime = total.GetTotalPlayedTime();
			float totalBoostUsed = total.GetBoostUsed();
			float totalTimeInAir = total.GetTimeInAir();
			out << nOfGames << ","
				<< total.GetBumps() << ','
				<< total.GetTeamBumps() << ','
				<< total.GetDemos() << ','
				<< total.GetBallHits() << ','
				<< reinterpret_cast<int&>(totalPlayedTime) << ','
				<< reinterpret_cast<int&>(totalBoostUsed) << ','
				<< reinterpret_cast<int&>(totalTimeInAir);

			out.close();
		}
		return true;
	}
	catch (std::exception ex) {
		DEBUGLOG("[FILESYSTEM_UPDATEFLAG]");
		DEBUGLOG(ex.what());
	}
	return false;
}