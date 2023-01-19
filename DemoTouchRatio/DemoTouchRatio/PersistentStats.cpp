#include <pch.h>
#include <PersistentStats.h>

#include <fstream>
#include <DemoTouchRatio.h>
#include <logging.h>

#define STATS_FOLDER "demotouchdata"
#define STATS_FILE "demotouchstats.csv"

#define STATS_COUNT 5

#define DEFAULT_SUMMARY_ARGS 0,0,0,0

PersistentStats::PersistentStats():
	total(),
	average(DEFAULT_SUMMARY_ARGS),
	nOfGames(0)
{ }

void PersistentStats::Initialize()
{
	total = GameStats();
	average = GameStatsSummary::SummarizedStats(DEFAULT_SUMMARY_ARGS);
	nOfGames = 0;

	LoadFile();
}

const GameStats& PersistentStats::Update(GameStats* stats)
{
	if (stats == nullptr)
		return total;

	++nOfGames;
	total = GameStats(
		total.GetBumps() + stats->GetBumps(),
		total.GetTeamBumps() + stats->GetTeamBumps(),
		total.GetDemos() + stats->GetDemos(),
		total.GetBallHits() + stats->GetBallHits()
	);

	if (!UpdateFile()) {
		--nOfGames;

		total = GameStats(
			total.GetBumps() - stats->GetBumps(),
			total.GetTeamBumps() - stats->GetTeamBumps(),
			total.GetDemos() - stats->GetDemos(),
			total.GetBallHits() - stats->GetBallHits()
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
	
	average = GameStatsSummary::SummarizedStats(
		(float)total.GetBumps() / (float)nOfGames, 
		(float)total.GetTeamBumps() / (float)nOfGames,
		(float)total.GetDemos() / (float)nOfGames,
		(float)total.GetBallHits() / (float)nOfGames);
}

void PersistentStats::Clear()
{
	nOfGames = 0;
	total = GameStats(0, 0, 0, 0);
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
	std::filesystem::path fullPath = filePath / STATS_FILE;
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
		int stats[STATS_COUNT] = { 0, 0, 0, 0, 0 };
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
		total = GameStats(stats[1], stats[2], stats[3], stats[4]);
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
			out << nOfGames << ","
				<< total.GetBumps() << ','
				<< total.GetTeamBumps() << ','
				<< total.GetDemos() << ','
				<< total.GetBallHits();

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