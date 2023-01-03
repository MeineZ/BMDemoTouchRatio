#include "pch.h"
#include "DemoTouchRatio.h"
#include "Util.h"
#include "Constants.h"
#include "GameStats.h"
#include "GameStatsSummary.h"
#include "Renderer.h"

BAKKESMOD_PLUGIN(DemoTouchRatio, "Demo Touch Ratio Plugin", plugin_version, 0)

DemoTouchRatio* DemoTouchRatio::instance_ = nullptr;
std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

DemoTouchRatio::DemoTouchRatio() :
	playedGames(std::vector<GameStats*>()),
	renderer(),
	currentGame(nullptr),
	lastGame(nullptr),
	enabled(std::make_shared<bool>(true)),
	renderInMatches(std::make_shared<bool>(true))
{ }

void DemoTouchRatio::onLoad()
{
	// Initialize 
	instance_ = this;
	_globalCvarManager = cvarManager;

	Reset();

	// Display CVar initialization
	cvarManager->registerCvar(CVAR_NAME_ENABLED, "1", "Enable DemoTouchRatio plugin", false, true, 0, true, 1, true).bindTo(enabled);
	cvarManager->registerCvar(CVAR_NAME_IN_MATCHES, "1", "Draw DemoTouchRation display during matches", false, true, 0, true, 1, true).bindTo(renderInMatches);

	// Renderer CVar initialization
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_X, "0", "X position of the display", false, true, 0, true, 3840, true).bindTo(renderer.posX);
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_Y, "500", "Y position of the display", false, true, 0, true, 2160, true).bindTo(renderer.posY);
	cvarManager->registerCvar(CVAR_NAME_DISPLAY_SCALE, "1", "Scale of the display", false, true, 0.5f, true, 5.0f, true).bindTo(renderer.scale);

	// CVar Hooks
	cvarManager->registerNotifier(CVAR_NAME_DISPLAY_SCALE_RESET, [this](std::vector<std::string> params) {
		renderer.ResetScale();
		cvarManager->getCvar(CVAR_NAME_DISPLAY_SCALE).setValue(*(renderer.scale));
	}, "Reset display scale to 1.0", PERMISSION_ALL);

	cvarManager->registerNotifier(CVAR_NAME_RESET, [this](std::vector<std::string> params) {
		Reset();
	}, "Start a fresh demo touch ratio session", PERMISSION_ALL);

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

	GameStatsSummary summary = GameStatsSummary(currentGame, lastGame, playedGames);
	renderer.RenderStats(&canvas, summary);
}