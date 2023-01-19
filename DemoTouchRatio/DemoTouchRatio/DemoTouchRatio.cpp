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
	playedGames(std::vector<GameStats*>()),
	renderer(),
	persistentStats(),
	currentGame(nullptr),
	lastGame(nullptr),
	scoreboardOpened(false),
	renderInMatches(std::make_shared<bool>(true)),
	renderOnlyOnScoreboard(std::make_shared<bool>(false)),
	renderInFreeplay(std::make_shared<bool>(true)),
	renderInCustomTraining(std::make_shared<bool>(true)),
	renderInReplay(std::make_shared<bool>(true)),
	matchAccolades(std::make_shared<bool>(true)),
	customDelayBump(std::make_shared<float>(0.020f)),
	customDelayDemo(std::make_shared<float>(0.f)),
	customDelayBallHit(std::make_shared<float>(0.020f)),
	usePersistentStats(std::make_shared<bool>(true))
{ }

void DemoTouchRatio::onLoad()
{
	// Initialize 
	instance_ = this;
	_globalCvarManager = cvarManager;

	Reset();
	persistentStats.Initialize();

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

	cvarManager->registerCvar(CVAR_NAME_RENDER_HORIZONTAL, "1", "Whether to render the display horizontally", false, true, 12, true, 96, true).bindTo(renderer.renderHorizontal);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DESC_SIZE, "0", "Whether to use custom cell size for desc. cells.", false, true, 0, true, 1, true).bindTo(renderer.customDescSize);
	cvarManager->registerCvar(CVAR_NAME_ROW_SIZE, "16", "Height of the displayed rows", false, true, 12, true, 96, true).bindTo(renderer.rowSize);
	cvarManager->registerCvar(CVAR_NAME_COLUMN_SIZE, "100", "Width of the displayed columns", false, true, 45, true, 250, true).bindTo(renderer.columnSize);

	// Columns show/hide CVar initialization
	cvarManager->registerCvar(CVAR_NAME_SHOW_BUMPS, "1", "Show bumps data column", false, true, 0, true, 1, true).bindTo(renderer.displayBumps);
	cvarManager->registerCvar(CVAR_NAME_SHOW_TEAMBUMPS, "0", "Show team bumps data column", false, true, 0, true, 1, true).bindTo(renderer.displayTeamBumps);
	cvarManager->registerCvar(CVAR_NAME_SHOW_DEMOS, "1", "Show demos data column", false, true, 0, true, 1, true).bindTo(renderer.displayDemos);
	cvarManager->registerCvar(CVAR_NAME_SHOW_BALLHITS, "1", "Show ball touches data column", false, true, 0, true, 1, true).bindTo(renderer.displayBallHits);

	cvarManager->registerCvar(CVAR_NAME_SHOW_PERSISTENT_TOTAL, "0", "Show all-time total stats", false, true, 0, true, 1, true).bindTo(renderer.displayPersistentTotal);
	cvarManager->registerCvar(CVAR_NAME_SHOW_PERSISTENT_AVERAGE, "0", "Show all-time average stats", false, true, 0, true, 1, true).bindTo(renderer.displayPersistentAverage);
	cvarManager->registerCvar(CVAR_NAME_REPLACE_TOTAL, "0", "Replace session total with all-time total stats", false, true, 0, true, 1, true).bindTo(renderer.replaceSessionTotal);
	cvarManager->registerCvar(CVAR_NAME_REPLACE_AVERAGE, "0", "Replace session average with all-time average stats", false, true, 0, true, 1, true).bindTo(renderer.replaceSessionAverage);

	// Custom behaviour
	cvarManager->registerCvar(CVAR_NAME_MATCH_ACCOLADES, "1", "Try to match the match-accolades", false, true, 0, true, 1, true).bindTo(matchAccolades);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DELAY_BUMP, "0.02", "Custom timeout on bump tracking", false, true, 0.f, true, 10.f, true).bindTo(customDelayBump);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DELAY_DEMO, "0.0", "Custom timeout on demo tracking", false, true, 0.f, true, 10.f, true).bindTo(customDelayDemo);
	cvarManager->registerCvar(CVAR_NAME_CUSTOM_DELAY_BALLHIT, "0.02", "Custom timeout on ballhit tracking", false, true, 0.f, true, 10.f, true).bindTo(customDelayBallHit);

	// Store total stats
	cvarManager->registerCvar(CVAR_NAME_PERSISTENT_STATS, "1", "Store all-time stats", false, true, 0, true, 1, true).bindTo(usePersistentStats);

	// Hook binding
	gameWrapper->HookEvent(HOOK_COUNTDOWN_BEGINSTATE, [this](std::string eventName) {
		if (!Util::CanTrack())
			return;

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

	if (lastGame != nullptr) {
		delete lastGame;
		lastGame = nullptr;
	}

	for (int i = 0; i < playedGames.size(); ++i)
	{
		delete playedGames[i];
	}
	playedGames.clear();
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
	if (currentGame == nullptr)
	{
		currentGame = new GameStats();
		currentGame->BindEvents();

		scoreboardOpened = false;
	}
}

void DemoTouchRatio::EndGame()
{
	scoreboardOpened = false;

	if (currentGame != nullptr)
	{
		if (lastGame != nullptr)
		{
			playedGames.push_back(lastGame);
			lastGame = nullptr;
		}

		if (*usePersistentStats) {
			persistentStats.Update(currentGame);
		}

		lastGame = currentGame;
		lastGame->UnbindEvents();
		currentGame = nullptr;
	}
}

void DemoTouchRatio::Render(CanvasWrapper canvas) {
	if (!*renderInMatches && currentGame != nullptr)
		return;

	if (currentGame != nullptr && *renderOnlyOnScoreboard && !scoreboardOpened)
		return;

	if (!*renderInFreeplay && gameWrapper->IsInFreeplay())
		return;

	if (!*renderInCustomTraining && gameWrapper->IsInCustomTraining())
		return;

	if (!*renderInReplay && gameWrapper->IsInReplay())
		return;

	GameStatsSummary summary = GameStatsSummary(currentGame, lastGame, playedGames);
	renderer.RenderStats(&canvas, summary, persistentStats);
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