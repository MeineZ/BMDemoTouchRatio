#include <pch.h>
#include <PersistentStats.h>

#include <fstream>
#include <DemoTouchRatio.h>
#include <logging.h>

#define STATS_FOLDER "demotouchdata"
#define STATS_FILENAME "demotouchstats"
#define STATS_FILEEXTENSION ".csv"

// [STAT_ADD] 17. Add arguments
#define STATS_COUNT 18

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
		total.GetTimeInAir() + stats->GetTimeInAir(),
		total.GetPowerslideCount() + stats->GetPowerslideCount(),
		total.GetPowerslideTimeInMinutes() + stats->GetPowerslideTimeInMinutes(),
		total.GetShots() + stats->GetShots(),
		total.GetGoals() + stats->GetGoals(),
		total.GetSaves() + stats->GetSaves(),
		total.GetTeamDemos() + stats->GetTeamDemos(),
		total.GetDeaths() + stats->GetDeaths(),
		total.GetBoostCollected() + stats->GetBoostCollected(),
		total.GetAssists() + stats->GetAssists(),
		total.GetBoostOverfill() + stats->GetBoostOverfill()
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
			total.GetTimeInAir() - stats->GetTimeInAir(),
			total.GetPowerslideCount() - stats->GetPowerslideCount(),
			total.GetPowerslideTimeInMinutes() - stats->GetPowerslideTimeInMinutes(),
			total.GetShots() - stats->GetShots(),
			total.GetGoals() - stats->GetGoals(),
			total.GetSaves() - stats->GetSaves(),
			total.GetTeamDemos() - stats->GetTeamDemos(),
			total.GetDeaths() - stats->GetDeaths(),
			total.GetBoostCollected() - stats->GetBoostCollected(),
			total.GetAssists() - stats->GetAssists(),
			total.GetBoostOverfill() - stats->GetBoostOverfill()
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
		total.GetInAirPercentage(),
		(float)total.GetPowerslideCount() / (float)nOfGames,
		total.GetPowerslideTimeInMinutes() / (float)nOfGames,
		(float)total.GetShots() / (float)nOfGames,
		(float)total.GetGoals() / (float)nOfGames,
		(float)total.GetSaves() / (float)nOfGames,
		(float) total.GetTeamDemos() / (float) nOfGames,
		(float) total.GetDeaths() / (float) nOfGames,
		total.GetBoostCollected() / (float)nOfGames,
		total.GetBoostCollectedPMinute(),
		(float) total.GetAssists() / (float) nOfGames,
		total.GetBoostOverfill() / (float) nOfGames,
		total.GetBoostOverfillPMinute() );
}

void PersistentStats::Clear()
{
	nOfGames = 0;
	// [STAT_ADD] 14. Add arguments
	total = GameStats(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	average = GameStatsSummary::SummarizedStats(DEFAULT_SUMMARY_ARGS);
	// Update file
	UpdateFile();
}

void PersistentStats::Add(PersistentStats& other)
{
	if (nOfGames == 0 && other.nOfGames == 0)
		return;

	total = GameStats(
		total.GetBumps() + other.total.GetBumps(),
		total.GetTeamBumps() + other.total.GetTeamBumps(),
		total.GetDemos() + other.total.GetDemos(),
		total.GetBallHits() + other.total.GetBallHits(),
		total.GetTotalPlayedTime() + other.total.GetTotalPlayedTime(),
		total.GetBoostUsed() + other.total.GetBoostUsed(),
		total.GetTimeInAir() + other.total.GetTimeInAir(),
		total.GetPowerslideCount() + other.total.GetPowerslideCount(),
		total.GetPowerslideTimeInMinutes() + other.total.GetPowerslideTimeInMinutes(),
		total.GetShots() + other.total.GetShots(),
		total.GetGoals() + other.total.GetGoals(),
		total.GetSaves() + other.total.GetSaves(),
		total.GetTeamDemos() + other.total.GetTeamDemos(),
		total.GetDeaths() + other.total.GetDeaths(),
		total.GetBoostCollected() + other.total.GetBoostCollected(),
		total.GetAssists() + other.total.GetAssists(),
		total.GetBoostOverfill() + other.total.GetBoostOverfill()
	);

	int oldGames = nOfGames;
	nOfGames += other.nOfGames;
	average.Add(other.average, true, oldGames, nOfGames);
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
		int stats[STATS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
			reinterpret_cast<float&>(stats[7]),
			stats[8],
			reinterpret_cast<float&>(stats[9]),
			stats[10],
			stats[11],
			stats[12],
			stats[13],
			stats[14],
			reinterpret_cast<float&>(stats[15]),
			stats[16],
			reinterpret_cast<float &>(stats[17])
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
			float totalBoostCollected = total.GetBoostCollected();
			float totalBoostOverfill = total.GetBoostOverfill();
			float totalTimeInAir = total.GetTimeInAir();
			float totalTimeInPowerslide = total.GetPowerslideTimeInMinutes();
			out << nOfGames << ","
				<< total.GetBumps() << ','
				<< total.GetTeamBumps() << ','
				<< total.GetDemos() << ','
				<< total.GetBallHits() << ','
				<< reinterpret_cast<int&>(totalPlayedTime) << ','
				<< reinterpret_cast<int&>(totalBoostUsed) << ','
				<< reinterpret_cast<int&>(totalTimeInAir) << ','
				<< total.GetPowerslideCount() << ','
				<< reinterpret_cast<int&>(totalTimeInPowerslide) << ','
				<< total.GetShots() << ','
				<< total.GetGoals() << ','
				<< total.GetSaves() << ','
				<< total.GetTeamDemos() << ','
				<< total.GetDeaths() << ','
				<< reinterpret_cast<int&>(totalBoostCollected) << ','
				<< total.GetAssists() << ','
				<< reinterpret_cast<int &>(totalBoostOverfill);

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