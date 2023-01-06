#pragma once

#include <string>

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "Renderer.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class GameStats;

class DemoTouchRatio: public BakkesMod::Plugin::BakkesModPlugin
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{
	static DemoTouchRatio* instance_;

	std::vector<GameStats*> playedGames;
	Renderer renderer;

	GameStats* lastGame;
	GameStats* currentGame;

	std::shared_ptr<bool> enabled; // Setting if plugin is enabled
	std::shared_ptr<bool> renderInMatches; // Setting if plugin may render during matches
	std::shared_ptr<bool> renderInFreeplay; // Setting if plugin may render during freeplay
	std::shared_ptr<bool> renderInCustomTraining; // Setting if plugin may render during custom training
	std::shared_ptr<bool> renderInReplay; // Setting if plugin may render during replays

	void onLoad() override;
	void onUnload() override; // Uncomment and implement if you need a unload method

	void Reset();
	void ResetColors();

public:
	DemoTouchRatio();

	static DemoTouchRatio& Instance();
	static GameWrapper* GameWrapper();

	void EndGame();
	void CreateNewGame();

	void Render(CanvasWrapper canvas);

	bool CanRenderInMatches();
	bool CanTrackTeamBumps();

	//void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
