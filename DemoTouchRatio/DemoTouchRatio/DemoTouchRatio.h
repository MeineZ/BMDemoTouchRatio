#pragma once

#include <string>

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class GameStats;

class DemoTouchRatio: public BakkesMod::Plugin::BakkesModPlugin
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{
	static DemoTouchRatio* instance_;

	std::vector<GameStats*> playedGames;
	//std::shared_ptr<bool> enabled;

	int bumpCounter;
	int demoCounter;
	int ballHitCounter;

	static GameStats* lastGame;
	static GameStats* currentGame;

	void onLoad() override;
	void onUnload() override; // Uncomment and implement if you need a unload method

public:
	static DemoTouchRatio& Instance();
	static GameWrapper* GameWrapper();

	void EndGame();
	void CreateNewGame();

	//void ResetCurrentGame();
	//static GameStats* GetCurrentGame();
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
