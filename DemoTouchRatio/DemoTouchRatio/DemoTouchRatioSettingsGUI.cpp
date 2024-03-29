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

#include "pch.h"
#include "DemoTouchRatio.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_internal.h"

#include "Util.h"
#include "Constants.h"

#define DISPLAY_OPTION_SPACING(i) 200*i

void DemoTouchRatio::RenderSettings()
{
	ImGui::TextUnformatted("Contact me via Discord: Meine#8883.");

	// Plugin settings
	RenderDisplayMoments();
	RenderColumnVisibility();
	RenderDisplayLayout();
	RenderCustomBehaviour();
	RenderPersistentStats();

	// Disclaimer
	ImGui::NewLine();
	ImGui::Separator();
	ImGui::TextUnformatted("Use the Reset button below to refresh your current session.");
	if (ImGui::Button("Reset##stats"))
	{
		gameWrapper->Execute([this](GameWrapper* gw) {
			Reset();
		});
	}
}

std::string DemoTouchRatio::GetPluginName()
{
	return "Simple Stats Tracker";
}

void DemoTouchRatio::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

void DemoTouchRatio::RenderDisplayMoments()
{
	if (!ImGui::CollapsingHeader("When are the stats visible?"))
		return;
	ImGui::TextUnformatted("Stats are always visible in main menu and post-game.");

	ImGui::Indent();

	DrawCheckbox("Show stats while in online matches", &*renderInMatches, CVAR_NAME_IN_MATCHES);

	ImGui::Indent();
	ImGuiPushDisable(!*renderInMatches);
	DrawCheckbox("Only if scoreboard is open", &*renderOnlyOnScoreboard, CVAR_NAME_ON_SCOREBOARD);
	ImGuiPopDisable(!*renderInMatches);
	ImGui::Unindent();

	DrawCheckbox("Show stats while in freeplay", &*renderInFreeplay, CVAR_NAME_IN_FREEPLAY);
	DrawCheckbox("Show stats while in custom training", &*renderInCustomTraining, CVAR_NAME_IN_CUSTOM_TRAINING);
	DrawCheckbox("Show ratio while in a replay", &*renderInReplay, CVAR_NAME_IN_REPLAY);

	ImGui::Unindent();
}

void DemoTouchRatio::RenderColumnVisibility()
{
	if (!ImGui::CollapsingHeader("What stats do you want to see?"))
		return;
	ImGui::TextUnformatted("Stats are always tracked. Even if you're not seeing them.");

	ImGui::Indent();

	DrawCheckbox("Display current playlist", &*renderer.renderTitle, CVAR_NAME_RENDER_TITLE);

	// =======================
	ImGui::NewLine();
	// =======================


	DrawCheckbox("Bumps", &*renderer.displayBumps, CVAR_NAME_SHOW_BUMPS);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(1)); DrawCheckbox("Team bumps", &*renderer.displayTeamBumps, CVAR_NAME_SHOW_TEAMBUMPS);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(2)); DrawCheckbox("Ball touches", &*renderer.displayBallHits, CVAR_NAME_SHOW_BALLHITS);
	DrawCheckbox("Demolitions", &*renderer.displayDemos, CVAR_NAME_SHOW_DEMOS);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(1)); DrawCheckbox("Team demolitions", &*renderer.displayTeamDemos, CVAR_NAME_SHOW_TEAMDEMOS);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(2)); DrawCheckbox("Deaths", &*renderer.displayDeaths, CVAR_NAME_SHOW_DEATHS);
	DrawCheckbox("In air %", &*renderer.displayInAirPercentage, CVAR_NAME_SHOW_INAIRPERCENTAGE);
	DrawCheckbox("Powerslide uses", &*renderer.displayPowerslideCount, CVAR_NAME_SHOW_POWERSLIDE_COUNT);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(1)); DrawCheckbox("Powerslide time", &*renderer.displayPowerslideDuration, CVAR_NAME_SHOW_POWERSLIDE_DURATION);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(2)); DrawCheckbox("Powerslide time per use", &*renderer.displayPowerslideDurationPerUse, CVAR_NAME_SHOW_POWERSLIDE_DURATION_PER_USE);
	DrawCheckbox("Shots", &*renderer.displayShots, CVAR_NAME_SHOW_SHOTS);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(1)); DrawCheckbox("Goals", &*renderer.displayGoals, CVAR_NAME_SHOW_GOALS);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(2)); DrawCheckbox("Saves", &*renderer.displaySaves, CVAR_NAME_SHOW_SAVES);
	ImGui::SameLine(DISPLAY_OPTION_SPACING(3)); DrawCheckbox("Assists", &*renderer.displayAssists, CVAR_NAME_SHOW_ASSISTS);

	if (ImGui::TreeNode("Boost stats"))
	{
		DrawCheckbox("Boost usage", &*renderer.displayBoostUsage, CVAR_NAME_SHOW_BOOSTUSAGE);
		ImGui::SameLine(DISPLAY_OPTION_SPACING(1)); DrawCheckbox("Boost/minute", &*renderer.displayBoostPMinute, CVAR_NAME_SHOW_BOOSTPMINUTE);
		DrawCheckbox("Boost collected", &*renderer.displayBoostCollected, CVAR_NAME_SHOW_BOOSTCOLLECTED);
		DrawFieldSideSettings("Sum individual boost collection stats", &*renderer.displayBoostCollectedSum, CVAR_NAME_SHOW_BOOSTCOLLECTED_SUM,
			&*renderer.displayBoostCollectedOwn, CVAR_NAME_SHOW_BOOSTCOLLECTED_OWN,
			&*renderer.displayBoostCollectedOpponent, CVAR_NAME_SHOW_BOOSTCOLLECTED_OPPONENT,
			&*renderer.displayBoostCollectedNeutral, CVAR_NAME_SHOW_BOOSTCOLLECTED_NEUTRAL
		);
		DrawCheckbox("Boost collected/minute", &*renderer.displayBoostCollectedPMinute, CVAR_NAME_SHOW_BOOSTCOLLECTEDPMINUTE);
		DrawFieldSideSettings("Sum individual boost collection per minute stats", &*renderer.displayBoostCollectedPMinuteSum, CVAR_NAME_SHOW_BOOSTCOLLECTEDPMINUTE_SUM,
			&*renderer.displayBoostCollectedPMinuteOwn, CVAR_NAME_SHOW_BOOSTCOLLECTEDPMINUTE_OWN,
			&*renderer.displayBoostCollectedPMinuteOpponent, CVAR_NAME_SHOW_BOOSTCOLLECTEDPMINUTE_OPPONENT,
			&*renderer.displayBoostCollectedPMinuteNeutral, CVAR_NAME_SHOW_BOOSTCOLLECTEDPMINUTE_NEUTRAL
		);
		DrawCheckbox("Boost overfill", &*renderer.displayBoostOverfill, CVAR_NAME_SHOW_BOOSTOVERFILL);
		DrawFieldSideSettings("Sum individual boost overfill stats", &*renderer.displayBoostOverfillSum, CVAR_NAME_SHOW_BOOSTOVERFILL_SUM,
			&*renderer.displayBoostOverfillOwn, CVAR_NAME_SHOW_BOOSTOVERFILL_OWN,
			&*renderer.displayBoostOverfillOpponent, CVAR_NAME_SHOW_BOOSTOVERFILL_OPPONENT,
			&*renderer.displayBoostOverfillNeutral, CVAR_NAME_SHOW_BOOSTOVERFILL_NEUTRAL
		);
		DrawCheckbox("Boost overfill/minute", &*renderer.displayBoostOverfillPMinute, CVAR_NAME_SHOW_BOOSTOVERFILLPMINUTE);
		DrawFieldSideSettings("Sum individual boost overfill per minute stats", &*renderer.displayBoostOverfillPMinuteSum, CVAR_NAME_SHOW_BOOSTOVERFILLPMINUTE_SUM,
			&*renderer.displayBoostOverfillPMinuteOwn, CVAR_NAME_SHOW_BOOSTOVERFILLPMINUTE_OWN,
			&*renderer.displayBoostOverfillPMinuteOpponent, CVAR_NAME_SHOW_BOOSTOVERFILLPMINUTE_OPPONENT,
			&*renderer.displayBoostOverfillPMinuteNeutral, CVAR_NAME_SHOW_BOOSTOVERFILLPMINUTE_NEUTRAL
		);
		ImGui::TreePop();
	}

	// =======================
	ImGui::NewLine();
	// =======================

	// Total / average
	ImGui::TextUnformatted("Current session stats.");
	DrawCheckbox("Show total##normal", &*renderer.displayTotal, CVAR_NAME_SHOW_TOTAL);
	ImGui::SameLine(200); DrawCheckbox("Show average##normal", &*renderer.displayAverage, CVAR_NAME_SHOW_AVERAGE);

	// =======================
	ImGui::NewLine();
	// =======================

	// Playlists total / average
	ImGui::TextUnformatted("Current session stats, but from all playlists combined. Its label is surrounded with '<...>'.");
	DrawCheckbox("Show total##playlists", &*renderer.displayPlaylistsTotal, CVAR_NAME_SHOW_PLAYLISTS_TOTAL);
	ImGui::SameLine(200); DrawCheckbox("Show average##playlists", &*renderer.displayPlaylistsAverage, CVAR_NAME_SHOW_PLAYLISTS_AVERAGE);

	// =======================
	ImGui::NewLine();
	// =======================

	if (!*usePersistentStats)
	{
		ImGui::TextUnformatted("Tracking total stats is disabled.");
		ImGui::NewLine();
	}

	// All-time stats
	ImGui::TextUnformatted("All-time stats. Its label is surrounded with '[...]'.");
	ImGui::TextUnformatted("See \"Track stats across game sessions\" section for more information.");
	ImGuiPushDisable(!*usePersistentStats);
	DrawCheckbox("Show total##persistent", &*renderer.displayPersistentTotal, CVAR_NAME_SHOW_PERSISTENT_TOTAL);
	ImGui::SameLine(200); DrawCheckbox("Show average##persistent", &*renderer.displayPersistentAverage, CVAR_NAME_SHOW_PERSISTENT_AVERAGE);
	ImGuiPopDisable(!*usePersistentStats);

	// =======================
	ImGui::NewLine();
	// =======================

	// All-time playlists stats
	ImGui::TextUnformatted("All-time stats, but from all playlists combined. Its label is surrounded with '[[...]]'.");
	ImGuiPushDisable(!*usePersistentStats);
	DrawCheckbox("Show total##playlistsPersistent", &*renderer.displayPlaylistsPersistentTotal, CVAR_NAME_SHOW_PLAYLISTS_PERSISTENT_TOTAL);
	ImGui::SameLine(200); DrawCheckbox("Show average##playlistsPersistent", &*renderer.displayPlaylistsPersistentAverage, CVAR_NAME_SHOW_PLAYLISTS_PERSISTENT_AVERAGE);
	ImGuiPopDisable(!*usePersistentStats);

	ImGui::Unindent();
}

void DemoTouchRatio::RenderDisplayLayout()
{
	if (!ImGui::CollapsingHeader("Stats table position & dimensions"))
		return;

	ImGui::Indent();
	ImGui::TextUnformatted("Position");
	DrawSlider("X", &*renderer.posX, 0, (int)ImGui::GetIO().DisplaySize.x, CVAR_NAME_DISPLAY_X);
	DrawSlider("Y", &*renderer.posY, 0, (int)ImGui::GetIO().DisplaySize.y, CVAR_NAME_DISPLAY_Y);

	// =======================
	ImGui::NewLine();
	// =======================

	ImGui::TextUnformatted("Scale");
	DrawSlider("Scale", &*renderer.scale, 0.5f, 5.f, CVAR_NAME_DISPLAY_SCALE);
	ImGui::SameLine();
	if (ImGui::Button("Reset##scale")) {
		gameWrapper->Execute([this](GameWrapper* gw) {
			renderer.ResetScale();
			cvarManager->getCvar(CVAR_NAME_DISPLAY_SCALE).setValue(*(renderer.scale));
		});
	}

	// =======================
	ImGui::NewLine();
	// =======================

	ImGui::TextUnformatted("Dimensions");
	DrawCheckbox("Display stats horizontally", &*renderer.renderHorizontal, CVAR_NAME_RENDER_HORIZONTAL);
	DrawCheckbox("Apply custom width to descriptions", &*renderer.customDescSize, CVAR_NAME_CUSTOM_DESC_SIZE);
	DrawSlider("Row height", &*renderer.rowSize, 12.f, 96.f, CVAR_NAME_ROW_SIZE);
	DrawSlider("Column width", &*renderer.columnSize, 45.f, 250.f, CVAR_NAME_COLUMN_SIZE);
	if (ImGui::Button("Reset row/column size"))
	{
		gameWrapper->Execute([this](GameWrapper* gw) {
			renderer.ResetTableSizes();

			cvarManager->getCvar(CVAR_NAME_ROW_SIZE).setValue(*(renderer.rowSize));
			cvarManager->getCvar(CVAR_NAME_COLUMN_SIZE).setValue(*(renderer.columnSize));
		});
	}

	// =======================
	ImGui::NewLine();
	// =======================

	ImGui::TextUnformatted("Colors");
	DrawColorPicker("Background", &*renderer.colorBackground, CVAR_NAME_COLOR_BACKGROUND);
	DrawColorPicker("Text", &*renderer.colorText, CVAR_NAME_COLOR_TEXT);
	if (ImGui::Button("Reset colors"))
	{
		gameWrapper->Execute([this](GameWrapper* gw) {
			renderer.ResetColors();

			cvarManager->getCvar(CVAR_NAME_COLOR_BACKGROUND).setValue(*(renderer.colorBackground));
			cvarManager->getCvar(CVAR_NAME_COLOR_TEXT).setValue(*(renderer.colorText));
		});
	}

	ImGui::Unindent();
}

void DemoTouchRatio::RenderCustomBehaviour()
{
	if (!ImGui::CollapsingHeader("Customize plugin behaviour"))
		return;

	ImGui::TextUnformatted("This plugin always tries to keep track of your actual stats to match the match-accolades as close as possible.");
	ImGui::TextUnformatted("Here you can disable this default behaviour and customize it.");

	ImGui::Indent();

	DrawCheckbox("Try to match match-accolades", &*matchAccolades, CVAR_NAME_MATCH_ACCOLADES);
	ImGui::NewLine();
	if (!*matchAccolades)
	{
		ImGui::TextUnformatted("Setting the delay (in seconds) of stats tracking, will change the rate at which a stat is tracked.");
		ImGui::TextUnformatted("For example, if the ball hit delay is set to 1.0 second it will only detect a ball hit each second.");
		ImGui::TextUnformatted("This has been tweaked in the plugin settings to ressemble match-accolade stats.");
		ImGui::TextUnformatted("So changing it here may cause inaccuracy with actual stats.");
	}

	ImGuiPushDisable(*matchAccolades);
	DrawSlider("Bump delay", &*customDelayBump, 0.f, 10.f, CVAR_NAME_CUSTOM_DELAY_BUMP); ImGui::SameLine();
	if (ImGui::Button("Reset##delaybump"))
	{
		*customDelayBump = 0.02f;
		gameWrapper->Execute([this](GameWrapper* gw) { cvarManager->getCvar(CVAR_NAME_CUSTOM_DELAY_BUMP).setValue(*customDelayBump); });
	}

	/*DrawSlider("Demo delay", &*customDelayDemo, 0.f, 10.f, CVAR_NAME_CUSTOM_DELAY_DEMO); ImGui::SameLine();
	if (ImGui::Button("Reset##delaydemo"))
	{
		*customDelayDemo = 0.f;
		gameWrapper->Execute([this](GameWrapper* gw) { cvarManager->getCvar(CVAR_NAME_CUSTOM_DELAY_DEMO).setValue(*customDelayDemo); });
	}*/

	DrawSlider("Ball hit delay", &*customDelayBallHit, 0.f, 10.f, CVAR_NAME_CUSTOM_DELAY_BALLHIT); ImGui::SameLine();
	if (ImGui::Button("Reset##delayballhit"))
	{
		*customDelayBallHit = 0.02f;
		gameWrapper->Execute([this](GameWrapper* gw) { cvarManager->getCvar(CVAR_NAME_CUSTOM_DELAY_BALLHIT).setValue(*customDelayBallHit); });
	}

	DrawSlider("In air delay", &*customDelayInAir, 0.f, 10.f, CVAR_NAME_CUSTOM_DELAY_INAIR); ImGui::SameLine();
	if (ImGui::Button("Reset##delayinair"))
	{
		*customDelayInAir = 0.0f;
		gameWrapper->Execute([this](GameWrapper* gw) { cvarManager->getCvar(CVAR_NAME_CUSTOM_DELAY_INAIR).setValue(*customDelayInAir); });
	}

	ImGuiPopDisable(*matchAccolades);

	ImGui::NewLine();

	ImGui::TextUnformatted("Bind the following notifiers to a key to manually scroll through playlist stats.");
	ImGui::TextUnformatted("Default behaviour shows the latest playlist played, but you can manually scroll through playlist stats with the following notifiers:");
	ImGui::TextUnformatted("      -  `cl_demotouch_plugin_force_next_playlist` to scroll forward");
	ImGui::TextUnformatted("      -  `cl_demotouch_plugin_force_prev_playlist` to scroll back");
	ImGui::TextUnformatted("      -  `cl_demotouch_plugin_reset_force_playlist` to reset to default behaviour (show latest playlist played)");

	ImGui::Unindent();
}


void DemoTouchRatio::RenderPersistentStats() {
	if (!ImGui::CollapsingHeader("Track stats across game sessions"))
		return;

	ImGui::TextUnformatted("Below you can choose if you want to keep tracking stats across game sessions.");
	ImGui::TextUnformatted("You can automatically store your stats after each game to keep track of your total sum of stats.");
	ImGui::TextUnformatted("Choose in the \"What stats do you want to see?\" section, whether you want to display them.");

	// Clear total stats
	ImGui::Indent();
	bool oldUsePersistentStats = *usePersistentStats;
	DrawCheckbox("Store stats", &*usePersistentStats, CVAR_NAME_PERSISTENT_STATS);
	if (oldUsePersistentStats  != *usePersistentStats && *usePersistentStats) {
		for (int i = 0; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i)
		{
			playlistStats[i].InitializePersistentStats();
		}
	}

	ImGui::NewLine();

	static uint64_t timeStamp = 0;
	if (ImGui::Button("Clear total stats")) {
		timeStamp = Util::TimestampInMS();
	}
	if (timeStamp + 10000 > Util::TimestampInMS()) { // Wait 10 seconds before hiding it again
		ImGui::NewLine();
		ImGui::TextUnformatted("!! This removes your all-time tracked stats !!");
		if (ImGui::Button("YES, I WANT TO CLEAR MY TOTAL STATS")) {
			for (int i = 0; i < NUMBER_OF_TRACKED_PLAYLISTS; ++i)
			{
				playlistStats[i].GetPersistentStats().Clear();
			}
		}
	}

	ImGui::Unindent();
}

void DemoTouchRatio::DrawFieldSideSettings( const char *sumLabel, bool *sum, const char *sumCvar, bool *own, const char *ownCvar, bool *opponent, const char *opponentCvar, bool *neutral, const char *neutralCvar )
{
	ImGui::Indent();

	std::string chckbx_id(std::string("##") + sumCvar);

	DrawCheckbox((sumLabel + chckbx_id).c_str(), sum, sumCvar);
	DrawCheckbox(("Defensive/Own##" + chckbx_id).c_str(), own, ownCvar);
	ImGui::SameLine( DISPLAY_OPTION_SPACING( 1 ) ); DrawCheckbox(("Mid/Neutral" + chckbx_id).c_str(), neutral, neutralCvar );
	ImGui::SameLine( DISPLAY_OPTION_SPACING( 2 ) ); DrawCheckbox(("Opponent/Stolen" + chckbx_id).c_str(), opponent, opponentCvar );

	ImGui::Unindent();
}

void DemoTouchRatio::DrawCheckbox(const char* label, bool* checked, const char* cvarName)
{
	bool oldValue = *checked;
	ImGui::Checkbox(label, checked);
	if (oldValue != *checked)
	{
		gameWrapper->Execute([this, cvarName, checked](GameWrapper* gw) {
			cvarManager->getCvar(cvarName).setValue(*checked);
		});
	}
}

void DemoTouchRatio::DrawSlider(const char* label, int* val, int min, int max, const char* cvarName, float width)
{
	int oldValue = *val;
	ImGui::SetNextItemWidth(width);
	ImGui::SliderInt(label, val, min, max);
	if (oldValue != *val)
	{
		gameWrapper->Execute([this, cvarName, val](GameWrapper* gw) {
			cvarManager->getCvar(cvarName).setValue(*val);
		});
	}
}

void DemoTouchRatio::DrawSlider(const char* label, float* val, float min, float max, const char* cvarName, float width)
{
	float oldValue = *val;
	ImGui::SetNextItemWidth(width);
	ImGui::SliderFloat(label, val, min, max, "%.4f");
	if (oldValue != *val)
	{
		gameWrapper->Execute([this, cvarName, val](GameWrapper* gw) {
			cvarManager->getCvar(cvarName).setValue(*val);
		});
	}
}

void DemoTouchRatio::DrawColorPicker(const char* label, LinearColor* color, const char* cvarName, float width)
{
	LinearColor oldColor = *color / 255.f;
	LinearColor newColor = *color / 255.f;
	ImGui::SetNextItemWidth(width);
	ImGui::ColorEdit4(
		label, 
		reinterpret_cast<float*>(&newColor),
		ImGuiColorEditFlags_AlphaBar 
		| ImGuiColorEditFlags_AlphaPreview 
		| ImGuiColorEditFlags_NoOptions);
	
	if (oldColor.R != newColor.R || oldColor.B != newColor.B || oldColor.G != newColor.G || oldColor.A != newColor.A)
	{
		*color = newColor * 255.f;
		gameWrapper->Execute([this, cvarName, color](GameWrapper* gw) {
			cvarManager->getCvar(cvarName).setValue(*color);
		});
	}
}

void DemoTouchRatio::ImGuiPushDisable(bool isDisabled)
{
	if (isDisabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
}

void DemoTouchRatio::ImGuiPopDisable(bool isDisabled)
{
	if (isDisabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
}