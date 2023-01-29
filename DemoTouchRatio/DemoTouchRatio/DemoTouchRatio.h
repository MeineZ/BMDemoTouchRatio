#pragma once

#include <string>

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "Renderer.h"
#include "PersistentStats.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class GameStats;

class DemoTouchRatio: public BakkesMod::Plugin::BakkesModPlugin
	, public BakkesMod::Plugin::PluginSettingsWindow
{
private:
	static DemoTouchRatio* instance_;

	std::vector<GameStats*> playedGames;
	Renderer renderer;
	PersistentStats persistentStats;

	GameStats* lastGame;
	GameStats* currentGame;

	bool scoreboardOpened;

	std::shared_ptr<bool> renderOnlyOnScoreboard; // Setting if plugin may only render when scoreboard is open
	std::shared_ptr<bool> renderInMatches; // Setting if plugin may render during matches
	std::shared_ptr<bool> renderInFreeplay; // Setting if plugin may render during freeplay
	std::shared_ptr<bool> renderInCustomTraining; // Setting if plugin may render during custom training
	std::shared_ptr<bool> renderInReplay; // Setting if plugin may render during replays
	std::shared_ptr<bool> forceStatsDisplay;

	std::shared_ptr<bool> matchAccolades; //
	std::shared_ptr<float> customDelayBump; //
	std::shared_ptr<float> customDelayDemo; //
	std::shared_ptr<float> customDelayBallHit; //

	std::shared_ptr<bool> usePersistentStats;

	void onLoad() override;
	void onUnload() override;

	void Reset();

public:
	DemoTouchRatio();

	static DemoTouchRatio& Instance();
	static GameWrapper* GetGameWrapper();

	void EndGame();
	void CreateNewGame();

	void Render(CanvasWrapper canvas);

	bool CanRenderInMatches();
	bool CanTrackTeamBumps();
	bool ShouldMatchAccolades();
	bool UsesPersistentStats();
	float GetCustomBumpDelay();
	float GetCustomDemoDelay();
	float GetCustomBallHitDelay();


	// On render thread
private:
	// Settings renderer
	void RenderDisplayMoments();
	void RenderColumnVisibility();
	void RenderDisplayLayout();
	void RenderCustomBehaviour();
	void RenderPersistentStats();

	// ImGui Helpers
	void ImGuiPushDisable(bool isDisabled);
	void ImGuiPopDisable(bool isDisabled);
	void DrawCheckbox(const char* label, bool* checked, const char* cvarName);
	void DrawSlider(const char* label, int* value, int min, int max, const char* cvarName, float width = 350.f);
	void DrawSlider(const char* label, float* value, float min, float max, const char* cvarName, float width = 350.f);
	void DrawColorPicker(const char* label, LinearColor* color, const char* cvarName, float width = 350.f);

public:
	// Settings renderer
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;
};
