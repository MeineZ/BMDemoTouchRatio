/*
 * Copyright 2023 MeineZ@GitHub

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

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
	void DrawFieldSideSettings( const char *sumLabel, bool *sum, const char *sumCvar, bool *own, const char *ownCvar, bool *opponent, const char *opponentCvar, bool *neutral, const char *neutralCvar );

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