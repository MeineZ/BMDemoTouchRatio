#pragma once

#include <string>
#include <memory>

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "Renderer.h"
#include "PlaylistStats.h"
#include "PlaylistEnum.h"
#include "PersistentStorage.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

#define NUMBER_OF_TRACKED_PLAYLISTS 10

class DemoTouchRatio: public BakkesMod::Plugin::BakkesModPlugin
	, public BakkesMod::Plugin::PluginSettingsWindow
{
private:
	static DemoTouchRatio* instance_;
	std::shared_ptr<PersistentStorage> persistentCVarStorage;

	PlaylistType forcePlaylist;
	PlaylistType lastPlaylist;
	PlaylistStats* playlistStats;

	Renderer renderer;

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
	std::shared_ptr<float> customDelayInAir; //

	std::shared_ptr<bool> usePersistentStats;

	void onLoad() override;
	void onUnload() override;

	void Reset();

	PlaylistStats* GetCurrentStats();

	void ResetForcePlaylist();
	void ForceNextPlaylist();
	void ForcePrevPlaylist();

public:
	DemoTouchRatio();

	static DemoTouchRatio& Instance();
	static GameWrapper* GetGameWrapper();

	void EndGame();
	void CreateNewGame();

	void Render(CanvasWrapper canvas);

	bool IsPlaylistForced();
	PlaylistType GetCurrentShownPlaylist();
	bool CanRenderInMatches();
	bool CanTrackTeamBumps();
	bool ShouldMatchAccolades();
	bool UsesPersistentStats();
	float GetCustomBumpDelay();
	float GetCustomDemoDelay();
	float GetCustomBallHitDelay();
	float GetCustomInAirDelay();


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


template <typename T, typename std::enable_if<std::is_base_of<ObjectWrapper, T>::value>::type*>
void GameWrapper::HookEventWithCaller(std::string eventName,
	std::function<void(T caller, void* params, std::string eventName)> callback)
{
	auto wrapped_callback = [callback](ActorWrapper caller, void* params, std::string eventName)
	{
		callback(T(caller.memory_address), params, eventName);
	};
	HookEventWithCaller<ActorWrapper>(eventName, wrapped_callback);
}

template <typename T, typename std::enable_if<std::is_base_of<ObjectWrapper, T>::value>::type*>
void GameWrapper::HookEventWithCallerPost(std::string eventName,
	std::function<void(T caller, void* params, std::string eventName)> callback)
{
	auto wrapped_callback = [callback](ActorWrapper caller, void* params, std::string eventName)
	{
		callback(T(caller.memory_address), params, eventName);
	};
	HookEventWithCallerPost<ActorWrapper>(eventName, wrapped_callback);
}