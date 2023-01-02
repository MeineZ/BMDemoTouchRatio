#include "pch.h"
#include "DemoTouchRatio.h"
#include "Util.h"
#include "Constants.h"
#include "GameStats.h"

BAKKESMOD_PLUGIN(DemoTouchRatio, "Demo Touch Ratio Plugin", plugin_version, 0)

DemoTouchRatio* DemoTouchRatio::instance_ = nullptr;
GameStats* DemoTouchRatio::lastGame = nullptr;
GameStats* DemoTouchRatio::currentGame = nullptr;

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void DemoTouchRatio::onLoad()
{
	instance_ = this;
	_globalCvarManager = cvarManager;

	lastGame = nullptr;
	currentGame = nullptr;

	bumpCounter = 0;
	demoCounter = 0;
	ballHitCounter = 0;

	playedGames = std::vector<GameStats*>();

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
}

void DemoTouchRatio::onUnload()
{
	gameWrapper->UnregisterDrawables();

	gameWrapper->UnhookEvent(HOOK_COUNTDOWN_BEGINSTATE);

	for (int i = 0; i < playedGames.size(); ++i)
	{
		delete playedGames[i];
	}
	playedGames.clear();
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