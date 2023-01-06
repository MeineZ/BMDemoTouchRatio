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
	currentGame(nullptr),
	lastGame(nullptr),
	enabled(std::make_shared<bool>(true)),
	renderInMatches(std::make_shared<bool>(true)),
	renderInFreeplay(std::make_shared<bool>(true)),
	renderInCustomTraining(std::make_shared<bool>(true)),
	renderInReplay(std::make_shared<bool>(true))
{ }

void DemoTouchRatio::onLoad()
{
	// Initialize 
	instance_ = this;
	_globalCvarManager = cvarManager;

	Reset();

	// Display CVar initialization
	cvarManager->registerCvar(CVAR_NAME_ENABLED, "1", "Enable DemoTouch plugin", false, true, 0, true, 1, true).bindTo(enabled);
	cvarManager->registerCvar(CVAR_NAME_IN_MATCHES, "1", "Draw DemoTouch display during matches", false, true, 0, true, 1, true).bindTo(renderInMatches);
	cvarManager->registerCvar(CVAR_NAME_IN_FREEPLAY, "1", "Draw DemoTouch display while in freeplay", false, true, 0, true, 1, true).bindTo(renderInFreeplay);
	cvarManager->registerCvar(CVAR_NAME_IN_CUSTOM_TRAINING, "1", "Draw DemoTouch display while in custom training", false, true, 0, true, 1, true).bindTo(renderInCustomTraining);
	cvarManager->registerCvar(CVAR_NAME_IN_REPLAY, "1", "Draw DemoTouch display while in replays", false, true, 0, true, 1, true).bindTo(renderInReplay);

	// Renderer CVar initialization
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_X, "0", "X position of the display", false, true, 0, true, 3840, true).bindTo(renderer.posX);
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_Y, "500", "Y position of the display", false, true, 0, true, 2160, true).bindTo(renderer.posY);
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_SCALE, "1", "Scale of the display", false, true, 0.5f, true, 5.0f, true).bindTo(renderer.scale);
	cvarManager->registerCvar(CVAR_NAME_COLOR_BACKGROUND, "(0, 0, 0, 75)", "Background color", false, false, 0, false, 255, true).bindTo(renderer.colorBackground);
	cvarManager->registerCvar(CVAR_NAME_COLOR_TEXT, "(255, 255, 255, 127)", "Text color", false, false, 0, false, 255, true).bindTo(renderer.colorText);

	// Columns show/hide CVar initialization
	cvarManager->registerCvar(CVAR_NAME_SHOW_BUMPS, "1", "Show bumps data column", false, true, 0, true, 1, true).bindTo(renderer.displayBumps);
	cvarManager->registerCvar(CVAR_NAME_SHOW_TEAMBUMPS, "0", "Show team bumps data column", false, true, 0, true, 1, true).bindTo(renderer.displayTeamBumps);
	cvarManager->registerCvar(CVAR_NAME_SHOW_DEMOS, "1", "Show demos data column", false, true, 0, true, 1, true).bindTo(renderer.displayDemos);
	cvarManager->registerCvar(CVAR_NAME_SHOW_BALLHITS, "1", "Show ball touches data column", false, true, 0, true, 1, true).bindTo(renderer.displayBallHits);


	// CVar Hooks
	cvarManager->registerNotifier(CVAR_NAME_DISPLAY_SCALE_RESET, [this](std::vector<std::string> params) {
		renderer.ResetScale();
		cvarManager->getCvar(CVAR_NAME_DISPLAY_SCALE).setValue(*(renderer.scale));
	}, "Reset display scale to 1.0", PERMISSION_ALL);

	cvarManager->registerNotifier(CVAR_NAME_RESET_COLORS, [this](std::vector<std::string> params) {
		ResetColors();
	}, "Reset to default colors", PERMISSION_ALL);

	cvarManager->registerNotifier(CVAR_NAME_RESET, [this](std::vector<std::string> params) {
		Reset();
	}, "Start a fresh demo touch session", PERMISSION_ALL);

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

	// Register drawable
	gameWrapper->RegisterDrawable(std::bind(&DemoTouchRatio::Render, this, std::placeholders::_1));
}

void DemoTouchRatio::Reset() {
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

void DemoTouchRatio::ResetColors()
{
	renderer.ResetColors();

	cvarManager->getCvar(CVAR_NAME_COLOR_BACKGROUND).setValue(*(renderer.colorBackground));
	cvarManager->getCvar(CVAR_NAME_COLOR_TEXT).setValue(*(renderer.colorText));
}

void DemoTouchRatio::onUnload()
{
	gameWrapper->UnregisterDrawables();

	gameWrapper->UnhookEvent(HOOK_COUNTDOWN_BEGINSTATE);
	gameWrapper->UnhookEvent(HOOK_ON_MAIN_MENU);
	gameWrapper->UnhookEvent(HOOK_ON_WINNER_SET);

	Reset();
}

DemoTouchRatio& DemoTouchRatio::Instance()
{
	return *instance_;
}

GameWrapper* DemoTouchRatio::GameWrapper()
{
	return &*instance_->gameWrapper;
}

void DemoTouchRatio::CreateNewGame()
{
	if (currentGame == nullptr)
	{
		currentGame = new GameStats();
		currentGame->BindEvents();
	}
}

void DemoTouchRatio::EndGame()
{
	if (lastGame != nullptr)
	{
		playedGames.push_back(lastGame);
		lastGame = nullptr;
	}

	if (currentGame != nullptr)
	{
		lastGame = currentGame;
		lastGame->UnbindEvents();
		currentGame = nullptr;
	}
}

void DemoTouchRatio::Render(CanvasWrapper canvas) {
	if (!*renderInMatches && currentGame != nullptr)
		return;

	if (!*renderInFreeplay && gameWrapper->IsInFreeplay())
		return;

	if (!*renderInCustomTraining && gameWrapper->IsInCustomTraining())
		return;

	if (!*renderInReplay && gameWrapper->IsInReplay())
		return;

	GameStatsSummary summary = GameStatsSummary(currentGame, lastGame, playedGames);
	renderer.RenderStats(&canvas, summary);
}

bool DemoTouchRatio::CanRenderInMatches() {
	return *renderInMatches;
}

bool DemoTouchRatio::CanTrackTeamBumps() {
	return *(renderer.displayTeamBumps);
}