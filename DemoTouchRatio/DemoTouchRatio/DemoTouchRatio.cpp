#include "pch.h"
#include "DemoTouchRatio.h"
#include "Util.h"
#include "Constants.h"
#include "GameStats.h"
#include "GameStatsSummary.h"
#include "Renderer.h"

BAKKESMOD_PLUGIN(DemoTouchRatio, "Demo Touch Count Plugin", plugin_version, 0)

DemoTouchRatio* DemoTouchRatio::instance_ = nullptr;
std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

DemoTouchRatio::DemoTouchRatio() :
	renderer(),
	forcePlaylist(PlaylistType::PLAYLIST_UNKNOWN),
	lastPlaylist(PlaylistType::PLAYLIST_RANKEDSTANDARD),
	playlistStats(nullptr),
	scoreboardOpened(false),
	renderInMatches(std::make_shared<bool>(true)),
	renderOnlyOnScoreboard(std::make_shared<bool>(false)),
	renderInFreeplay(std::make_shared<bool>(true)),
	renderInCustomTraining(std::make_shared<bool>(true)),
	renderInReplay(std::make_shared<bool>(true)),
	forceStatsDisplay(std::make_shared<bool>(false)),
	matchAccolades(std::make_shared<bool>(true)),
	customDelayBump(std::make_shared<float>(0.020f)),
	customDelayDemo(std::make_shared<float>(0.f)),
	customDelayBallHit(std::make_shared<float>(0.020f)),
	customDelayInAir(std::make_shared<float>(0.f)),
	usePersistentStats(std::make_shared<bool>(true))
{} 

void DemoTouchRatio::onLoad()
{
	// Initialize 
	instance_ = this;
	_globalCvarManager = cvarManager;

	Reset();

	// Initialize playlist stats
	playlistStats = new PlaylistStats[NUMBER_OF_TRACKED_PLAYLISTS] {
		PlaylistStats(PlaylistType::PLAYLIST_PRIVATEMATCH),
		PlaylistStats(PlaylistType::PLAYLIST_EXHIBITION),
		PlaylistStats(PlaylistType::PLAYLIST_CASUAL),
		PlaylistStats(PlaylistType::PLAYLIST_RANKEDDUEL),
		PlaylistStats(PlaylistType::PLAYLIST_RANKEDDOUBLES),
		PlaylistStats(PlaylistType::PLAYLIST_RANKEDSTANDARD),
		PlaylistStats(PlaylistType::PLAYLIST_RANKEDHOOPS),
		PlaylistStats(PlaylistType::PLAYLIST_RANKEDRUMBLE),
		PlaylistStats(PlaylistType::PLAYLIST_RANKEDDROPSHOT),
		PlaylistStats(PlaylistType::PLAYLIST_RANKEDSNOWDAY)
	};

	for (int i = 0; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i)
	{
		playlistStats[i].Initialize();
	}

	// Display CVar initialization
	cvarManager->registerCvar(CVAR_NAME_IN_MATCHES, "1", "Draw DemoTouch display during matches", false, true, 0, true, 1, true).bindTo(renderInMatches);
	cvarManager->registerCvar(CVAR_NAME_ON_SCOREBOARD, "0", "Draw DemoTouch display only when scoreboard is open", false, true, 0, true, 1, true).bindTo(renderOnlyOnScoreboard);
	cvarManager->registerCvar(CVAR_NAME_IN_FREEPLAY, "1", "Draw DemoTouch display while in freeplay", false, true, 0, true, 1, true).bindTo(renderInFreeplay);
	cvarManager->registerCvar(CVAR_NAME_IN_CUSTOM_TRAINING, "1", "Draw DemoTouch display while in custom training", false, true, 0, true, 1, true).bindTo(renderInCustomTraining);
	cvarManager->registerCvar(CVAR_NAME_IN_REPLAY, "1", "Draw DemoTouch display while in replays", false, true, 0, true, 1, true).bindTo(renderInReplay);

	// Renderer CVar initialization
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_X, "0", "X position of the display", false, true, 0, true, 3840, true).bindTo(renderer.posX);
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_Y, "500", "Y position of the display", false, true, 0, true, 2160, true).bindTo(renderer.posY);
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_SCALE, "1", "Scale of the display", false, true, 0.5f, true, 5.0f, true).bindTo(renderer.scale);
	cvarManager->registerCvar(CVAR_NAME_COLOR_BACKGROUND, "(0, 0, 0, 75)", "Background color", false, false, 0, false, 255, true).bindTo(renderer.colorBackground);
	cvarManager->registerCvar(CVAR_NAME_COLOR_TEXT, "(255, 255, 255, 127)", "Text color", false, false, 0, false, 255, true).bindTo(renderer.colorText);

	cvarManager->registerCvar(CVAR_NAME_RENDER_HORIZONTAL, "1", "Whether to render the display horizontally", false, true, 0, true, 1, true).bindTo(renderer.renderHorizontal);
	cvarManager->registerCvar(CVAR_NAME_RENDER_TITLE, "1", "Whether to render the title (showing the current playlist)", false, true, 0, true, 1, true).bindTo(renderer.renderTitle);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DESC_SIZE, "0", "Whether to use custom cell size for desc. cells.", false, true, 0, true, 1, true).bindTo(renderer.customDescSize);
	cvarManager->registerCvar(CVAR_NAME_ROW_SIZE, "16", "Height of the displayed rows", false, true, 12, true, 96, true).bindTo(renderer.rowSize);
	cvarManager->registerCvar(CVAR_NAME_COLUMN_SIZE, "100", "Width of the displayed columns", false, true, 45, true, 250, true).bindTo(renderer.columnSize);

	// Columns show/hide CVar initialization
	// [STAT_ADD] 2. Register display cvar
	cvarManager->registerCvar(CVAR_NAME_SHOW_BUMPS, "1", "Show bumps data column", false, true, 0, true, 1, true).bindTo(renderer.displayBumps);
	cvarManager->registerCvar(CVAR_NAME_SHOW_TEAMBUMPS, "0", "Show team bumps data column", false, true, 0, true, 1, true).bindTo(renderer.displayTeamBumps);
	cvarManager->registerCvar(CVAR_NAME_SHOW_DEMOS, "1", "Show demos data column", false, true, 0, true, 1, true).bindTo(renderer.displayDemos);
	cvarManager->registerCvar(CVAR_NAME_SHOW_TEAMDEMOS, "0", "Show team demos data column", false, true, 0, true, 1, true).bindTo(renderer.displayTeamDemos);
	cvarManager->registerCvar(CVAR_NAME_SHOW_DEATHS, "0", "Show deaths data column", false, true, 0, true, 1, true).bindTo(renderer.displayDeaths);
	cvarManager->registerCvar(CVAR_NAME_SHOW_BALLHITS, "1", "Show ball touches data column", false, true, 0, true, 1, true).bindTo(renderer.displayBallHits);
	cvarManager->registerCvar(CVAR_NAME_SHOW_BOOSTUSAGE, "0", "Show boost usage data column", false, true, 0, true, 1, true).bindTo(renderer.displayBoostUsage);
	cvarManager->registerCvar(CVAR_NAME_SHOW_BOOSTPMINUTE, "0", "Show boost per minute data column", false, true, 0, true, 1, true).bindTo(renderer.displayBoostPMinute);
	cvarManager->registerCvar(CVAR_NAME_SHOW_BOOSTCOLLECTED, "0", "Show boost collected data column", false, true, 0, true, 1, true).bindTo(renderer.displayBoostCollected);
	cvarManager->registerCvar(CVAR_NAME_SHOW_BOOSTCOLLECTEDPMINUTE, "0", "Show boost collected per minute data column", false, true, 0, true, 1, true).bindTo(renderer.displayBoostCollectedPMinute);
	cvarManager->registerCvar(CVAR_NAME_SHOW_INAIRPERCENTAGE, "0", "Show in air percentage data column", false, true, 0, true, 1, true).bindTo(renderer.displayInAirPercentage);
	cvarManager->registerCvar(CVAR_NAME_SHOW_POWERSLIDE_COUNT, "0", "Show powerslide count data column", false, true, 0, true, 1, true).bindTo(renderer.displayPowerslideCount);
	cvarManager->registerCvar(CVAR_NAME_SHOW_POWERSLIDE_DURATION, "0", "Show powerslide duration data column", false, true, 0, true, 1, true).bindTo(renderer.displayPowerslideDuration);
	cvarManager->registerCvar( CVAR_NAME_SHOW_POWERSLIDE_DURATION_PER_USE, "0", "Show powerslide duration per use data column", false, true, 0, true, 1, true ).bindTo( renderer.displayPowerslideDurationPerUse );
	cvarManager->registerCvar(CVAR_NAME_SHOW_SHOTS, "0", "Show goals data column", false, true, 0, true, 1, true).bindTo(renderer.displayShots);
	cvarManager->registerCvar(CVAR_NAME_SHOW_GOALS, "0", "Show goals data column", false, true, 0, true, 1, true).bindTo(renderer.displayGoals);
	cvarManager->registerCvar(CVAR_NAME_SHOW_SAVES, "0", "Show saves data column", false, true, 0, true, 1, true).bindTo(renderer.displaySaves);

	cvarManager->registerCvar(CVAR_NAME_SHOW_TOTAL, "1", "Show session total stats", false, true, 0, true, 1, true).bindTo(renderer.displayTotal);
	cvarManager->registerCvar(CVAR_NAME_SHOW_AVERAGE, "1", "Show session average stats", false, true, 0, true, 1, true).bindTo(renderer.displayAverage);
	cvarManager->registerCvar(CVAR_NAME_SHOW_PLAYLISTS_TOTAL, "0", "Show session total stats over all gamemodes", false, true, 0, true, 1, true).bindTo(renderer.displayPlaylistsTotal);
	cvarManager->registerCvar(CVAR_NAME_SHOW_PLAYLISTS_AVERAGE, "0", "Show session average stats over all gamemodes", false, true, 0, true, 1, true).bindTo(renderer.displayPlaylistsAverage);

	cvarManager->registerCvar(CVAR_NAME_SHOW_PERSISTENT_TOTAL, "0", "Show all-time total stats", false, true, 0, true, 1, true).bindTo(renderer.displayPersistentTotal);
	cvarManager->registerCvar(CVAR_NAME_SHOW_PERSISTENT_AVERAGE, "0", "Show all-time average stats", false, true, 0, true, 1, true).bindTo(renderer.displayPersistentAverage);
	cvarManager->registerCvar(CVAR_NAME_SHOW_PLAYLISTS_PERSISTENT_TOTAL, "0", "Show all-time total stats over all gamemodes", false, true, 0, true, 1, true).bindTo(renderer.displayPlaylistsPersistentTotal);
	cvarManager->registerCvar(CVAR_NAME_SHOW_PLAYLISTS_PERSISTENT_AVERAGE, "0", "Show all-time average stats over all gamemodes", false, true, 0, true, 1, true).bindTo(renderer.displayPlaylistsPersistentAverage);

	cvarManager->registerCvar(CVAR_NOT_FORCE_SHOW_STATS, "0", "Force the display of your stats", true, true, 0, true, 1, false).bindTo(forceStatsDisplay);

	// Custom behaviour
	cvarManager->registerCvar(CVAR_NAME_MATCH_ACCOLADES, "1", "Try to match the match-accolades", false, true, 0, true, 1, true).bindTo(matchAccolades);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DELAY_BUMP, "0.02", "Custom timeout on bump tracking", false, true, 0.f, true, 10.f, true).bindTo(customDelayBump);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DELAY_DEMO, "0.0", "Custom timeout on demo tracking", false, true, 0.f, true, 10.f, true).bindTo(customDelayDemo);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DELAY_BALLHIT, "0.02", "Custom timeout on ballhit tracking", false, true, 0.f, true, 10.f, true).bindTo(customDelayBallHit);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DELAY_INAIR, "0.0", "Custom timeout on in-air tracking", false, true, 0.f, true, 10.f, true).bindTo(customDelayInAir);

	// Store total stats
	cvarManager->registerCvar(CVAR_NAME_PERSISTENT_STATS, "1", "Store all-time stats", false, true, 0, true, 1, true).bindTo(usePersistentStats);

	// Register notifiers
	cvarManager->registerCvar(CVAR_NOT_RESET_STATS, "0", "Cvar notifier to reset stats", true, true, 0, true, 1, false);
	cvarManager->registerNotifier(CVAR_NOT_RESET_STATS, [this](std::vector<std::string> params) {
		Reset();
	}, "Trigger a reset for your session stats", PERMISSION_ALL);

	cvarManager->registerCvar(CVAR_NOT_RESET_FORCE_PLAYLIST, "0", "Cvar notifier to reset forced playlist", true, true, 0, true, 1, false);
	cvarManager->registerNotifier(CVAR_NOT_RESET_FORCE_PLAYLIST, [this](std::vector<std::string> params) {
		ResetForcePlaylist();
	}, "Trigger a reset the forced playlist (show last played mode)", PERMISSION_ALL);

	cvarManager->registerCvar(CVAR_NOT_FORCE_NEXT_PLAYLIST, "0", "Cvar notifier to show next forced playlist", true, true, 0, true, 1, false);
	cvarManager->registerNotifier(CVAR_NOT_FORCE_NEXT_PLAYLIST, [this](std::vector<std::string> params) {
		ForceNextPlaylist();
	}, "Show next forced playlist", PERMISSION_ALL);

	cvarManager->registerCvar(CVAR_NOT_FORCE_PREV_PLAYLIST, "0", "Cvar notifier to show previous forced playlist", true, true, 0, true, 1, false);
	cvarManager->registerNotifier(CVAR_NOT_FORCE_PREV_PLAYLIST, [this](std::vector<std::string> params) {
		ForcePrevPlaylist();
	}, "Show previous forced playlist", PERMISSION_ALL);


	// Hook binding
	gameWrapper->HookEvent(HOOK_COUNTDOWN_BEGINSTATE, [this](std::string eventName) {
		CreateNewGame();
	});

	gameWrapper->HookEvent(HOOK_MATCH_STARTED, [this](std::string eventName) {
		CreateNewGame();
	});
	
	gameWrapper->HookEvent(HOOK_ON_MAIN_MENU, [this](std::string eventName) {
		EndGame();
	});
	gameWrapper->HookEvent(HOOK_ON_WINNER_SET, [this](std::string eventName) {
		EndGame();
	});
	gameWrapper->HookEvent(HOOK_ON_LEAVE_MATCH, [this](std::string eventName) {
		EndGame();
	});

	gameWrapper->HookEvent(HOOK_ON_SCOREBOARD_OPENED, [this](std::string eventName) { scoreboardOpened = true; });
	gameWrapper->HookEvent(HOOK_ON_SCOREBOARD_CLOSED, [this](std::string eventName) { scoreboardOpened = false; });

	// Register drawable
	gameWrapper->RegisterDrawable(std::bind(&DemoTouchRatio::Render, this, std::placeholders::_1));
}

void DemoTouchRatio::Reset() {
	scoreboardOpened = false;

	EndGame();

	if (playlistStats == nullptr)
		return;

	// EndGame
	for (int i = 0; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i)
	{
		playlistStats[i].Reset();
	}
}

void DemoTouchRatio::onUnload()
{
	gameWrapper->UnregisterDrawables();

	gameWrapper->UnhookEvent(HOOK_COUNTDOWN_BEGINSTATE);
	gameWrapper->UnhookEvent(HOOK_ON_MAIN_MENU);
	gameWrapper->UnhookEvent(HOOK_ON_WINNER_SET);
	gameWrapper->UnhookEvent(HOOK_ON_SCOREBOARD_OPENED);
	gameWrapper->UnhookEvent(HOOK_ON_SCOREBOARD_CLOSED);

	Reset();

	delete playlistStats;
	playlistStats = nullptr;
}

DemoTouchRatio& DemoTouchRatio::Instance()
{
	return *instance_;
}

GameWrapper* DemoTouchRatio::GetGameWrapper()
{
	return &*instance_->gameWrapper;
}

void DemoTouchRatio::CreateNewGame()
{
	if (!Util::CanTrack())
		return;

	scoreboardOpened = false;

	if (playlistStats == nullptr)
		return;

	lastPlaylist = Util::CurrentPlaylist();

	// Create new game 
	for (int i = 0; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i)
	{
		playlistStats[i].CreateNewGame();
		if (playlistStats[i].GetCurrentGame() != nullptr)
			return;
	}
	// Set to unknown playlist if no supported gamemode is active
	lastPlaylist = PlaylistType::PLAYLIST_UNKNOWN;
}

void DemoTouchRatio::EndGame()
{
	scoreboardOpened = false;
	
	if (playlistStats == nullptr)
		return;

	// EndGame
	for (int i = 0; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i)
	{
		playlistStats[i].EndGame();
	}
}

void DemoTouchRatio::Render(CanvasWrapper canvas) {
	PlaylistStats* currentStats = nullptr;
	currentStats = GetCurrentStats();
	if (currentStats == nullptr)
		return;

	// ALWAYS show if the user uses the force notifier
	if (!*forceStatsDisplay)
	{
		if (!*renderInMatches && currentStats->GetCurrentGame() != nullptr)
			return;

		if (currentStats->GetCurrentGame() != nullptr && *renderOnlyOnScoreboard && !scoreboardOpened)
			return;

		if (!*renderInFreeplay && gameWrapper->IsInFreeplay())
			return;

		if (!*renderInCustomTraining && gameWrapper->IsInCustomTraining())
			return;

		if (!*renderInReplay && gameWrapper->IsInReplay())
			return;
	}

	// Calculate all playlists stats if neccessary
	GameStatsSummary playlistsSummary = GameStatsSummary();
	PersistentStats playlistsPersistentSummary = playlistStats[0].GetPersistentStats();
	if (renderer.ShouldShowPlaylistsTotal() || renderer.ShouldShowPlaylistsAverage())
	{
		for (int i = 0; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i) {
			GameStatsSummary playlistSummary = GameStatsSummary(playlistStats[i].GetCurrentGame(), playlistStats[i].GetLastGame(), playlistStats[i].GetPlayedGames());
			playlistsSummary.AddTotalAndAverage(playlistSummary);
		}
	}

	if (renderer.ShouldShowPlaylistsPersistentTotal() || renderer.ShouldShowPlaylistsPersistentAverage())
	{
		for (int i = 1; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i) {
			playlistsPersistentSummary.Add(playlistStats[i].GetPersistentStats());
		}
	}

	GameStatsSummary summary = GameStatsSummary(currentStats->GetCurrentGame(), currentStats->GetLastGame(), currentStats->GetPlayedGames());
	renderer.RenderStats(&canvas, summary, currentStats->GetPersistentStats(), playlistsSummary, playlistsPersistentSummary);
}

PlaylistStats* DemoTouchRatio::GetCurrentStats()
{
	if (playlistStats == nullptr)
		return nullptr;

	for (int i = 0; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i)
	{
		if (playlistStats[i].IsCurrent(GetCurrentShownPlaylist()))
		{
			return &playlistStats[i];
		}
	}
}

void DemoTouchRatio::ResetForcePlaylist()
{
	forcePlaylist = PlaylistType::PLAYLIST_UNKNOWN;
}

void DemoTouchRatio::ForceNextPlaylist()
{
	forcePlaylist = PlaylistHelpers::GetNextPlaylist(
		IsPlaylistForced()
		?  forcePlaylist
		: lastPlaylist);
}

void DemoTouchRatio::ForcePrevPlaylist()
{
	forcePlaylist = PlaylistHelpers::GetPrevPlaylist(
		IsPlaylistForced()
		? forcePlaylist
		: lastPlaylist);
}

bool DemoTouchRatio::IsPlaylistForced()
{
	return forcePlaylist != PlaylistType::PLAYLIST_UNKNOWN;
}

PlaylistType DemoTouchRatio::GetCurrentShownPlaylist()
{
	return IsPlaylistForced()
		? forcePlaylist
		: lastPlaylist;
}

bool DemoTouchRatio::CanRenderInMatches() {
	return *renderInMatches;
}

bool DemoTouchRatio::CanTrackTeamBumps() {
	return *(renderer.displayTeamBumps);
}

bool DemoTouchRatio::ShouldMatchAccolades()
{
	return *matchAccolades;
}

bool DemoTouchRatio::UsesPersistentStats()
{
	return *usePersistentStats;
}


float DemoTouchRatio::GetCustomBumpDelay()
{
	return *customDelayBump * 1000.f;
}

float DemoTouchRatio::GetCustomDemoDelay()
{
	return *customDelayDemo * 1000.f;
}

float DemoTouchRatio::GetCustomBallHitDelay()
{
	return *customDelayBallHit * 1000.f;
}

float DemoTouchRatio::GetCustomInAirDelay()
{
	return *customDelayInAir * 1000.f;
}