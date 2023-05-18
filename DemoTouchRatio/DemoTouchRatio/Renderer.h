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

#include "FieldSidesData.h"

#include <memory>
#include <sstream>

class CanvasWrapper;
class GameStats;
class GameStatsSummary;
class PersistentStats;

#define DEFAULT_ROW_SIZE 16.f
#define DEFAULT_COLUMN_SIZE 100.f
#define TITLE_SIZE 1.2f
#define NAME_SIZE 1.0f

#define X_OFFSET 10
#define Y_OFFSET 5

#define STATS_RENDER_ARGUMENTS float current, float last, float total, float average, float playlistsTotal, float playlistsAverage, float persistentTotal, float persistentAverage, float persistentPlaylistsTotal, float persistentPlaylistsAverage
#define FIELDSIDESSTATS_RENDER_ARGUMENTS FieldSidesData current, FieldSidesData last, FieldSidesData total, FieldSidesData average, FieldSidesData playlistsTotal, FieldSidesData playlistsAverage, FieldSidesData persistentTotal, FieldSidesData persistentAverage, FieldSidesData persistentPlaylistsTotal, FieldSidesData persistentPlaylistsAverage, bool displayTotal, bool displaySum, bool displayOwn, bool displayOpponent, bool displayNeutral 

class Renderer {
private:

	// Private utils
	void RenderText(CanvasWrapper* canvas, std::string text, int columnId, int rowId, bool isRowDesc, bool isColDesc);
	void RenderText(CanvasWrapper* canvas, std::stringstream& text, int columnId, int rowId, bool isRowDesc, bool isColDesc);
	void RenderText(CanvasWrapper* canvas, float value, int columnId, int rowId, std::stringstream& ss, int precision = 2);

	Vector2 GetBox();

	void RenderTitle(CanvasWrapper* canvas, int width); // Renders title for table
	void RenderHeader(CanvasWrapper* canvas, int nOfGames, int playlistsNOfGames, int persistentNOfGames, int playlistsPersistentNOfGames); // Renders header for table
	void RenderData(CanvasWrapper* canvas, int nth, std::string label, STATS_RENDER_ARGUMENTS, int decimals = 0); // Renders data row/column for table
	void RenderFieldsSidesData(CanvasWrapper* canvas, int& nth, std::string postLabel, FIELDSIDESSTATS_RENDER_ARGUMENTS); // Renders fieldsidesdata row/column for table

public:
	std::shared_ptr<int> posX;
	std::shared_ptr<int> posY;
	std::shared_ptr<float> scale;
	std::shared_ptr<LinearColor> colorBackground; // The color used for the background
	std::shared_ptr<LinearColor> colorText; // The color used for texts

	std::shared_ptr<float> rowSize;
	std::shared_ptr<float> columnSize;

	// [STAT_ADD] 3. Declare display bool
	std::shared_ptr<bool> displayBumps;
	std::shared_ptr<bool> displayTeamBumps;
	std::shared_ptr<bool> displayDemos;
	std::shared_ptr<bool> displayTeamDemos;
	std::shared_ptr<bool> displayDeaths;
	std::shared_ptr<bool> displayBallHits;
	std::shared_ptr<bool> displayBoostUsage;
	std::shared_ptr<bool> displayBoostPMinute;
	std::shared_ptr<bool> displayBoostCollected;
	std::shared_ptr<bool> displayBoostCollectedSum;
	std::shared_ptr<bool> displayBoostCollectedOwn;
	std::shared_ptr<bool> displayBoostCollectedOpponent;
	std::shared_ptr<bool> displayBoostCollectedNeutral;
	std::shared_ptr<bool> displayBoostCollectedPMinute;
	std::shared_ptr<bool> displayBoostCollectedPMinuteSum;
	std::shared_ptr<bool> displayBoostCollectedPMinuteOwn;
	std::shared_ptr<bool> displayBoostCollectedPMinuteOpponent;
	std::shared_ptr<bool> displayBoostCollectedPMinuteNeutral;
	std::shared_ptr<bool> displayBoostOverfill;
	std::shared_ptr<bool> displayBoostOverfillSum;
	std::shared_ptr<bool> displayBoostOverfillOwn;
	std::shared_ptr<bool> displayBoostOverfillOpponent;
	std::shared_ptr<bool> displayBoostOverfillNeutral;
	std::shared_ptr<bool> displayBoostOverfillPMinute;
	std::shared_ptr<bool> displayBoostOverfillPMinuteSum;
	std::shared_ptr<bool> displayBoostOverfillPMinuteOwn;
	std::shared_ptr<bool> displayBoostOverfillPMinuteOpponent;
	std::shared_ptr<bool> displayBoostOverfillPMinuteNeutral;
	std::shared_ptr<bool> displayInAirPercentage;
	std::shared_ptr<bool> displayPowerslideCount;
	std::shared_ptr<bool> displayPowerslideDuration;
	std::shared_ptr<bool> displayPowerslideDurationPerUse;
	std::shared_ptr<bool> displayShots;
	std::shared_ptr<bool> displayGoals;
	std::shared_ptr<bool> displaySaves;
	std::shared_ptr<bool> displayAssists;

	std::shared_ptr<bool> displayTotal;
	std::shared_ptr<bool> displayAverage;
	std::shared_ptr<bool> displayPlaylistsTotal;
	std::shared_ptr<bool> displayPlaylistsAverage;
	std::shared_ptr<bool> displayPersistentTotal;
	std::shared_ptr<bool> displayPersistentAverage;
	std::shared_ptr<bool> displayPlaylistsPersistentTotal;
	std::shared_ptr<bool> displayPlaylistsPersistentAverage;

	std::shared_ptr<bool> renderTitle;
	std::shared_ptr<bool> renderHorizontal;
	std::shared_ptr<bool> customDescSize;

	Renderer();

	void RenderStats(CanvasWrapper* canvas, GameStatsSummary& gameStats, PersistentStats& persistentStats, GameStatsSummary& playlists, PersistentStats& playlistsPersistent);

	void ResetScale();
	void ResetColors();
	void ResetTableSizes();

	// Public utils
	bool ShouldShowTotal();
	bool ShouldShowAverage();
	bool ShouldShowPlaylistsTotal();
	bool ShouldShowPlaylistsAverage();
	bool ShouldShowPersistentTotal();
	bool ShouldShowPersistentAverage();
	bool ShouldShowPlaylistsPersistentTotal();
	bool ShouldShowPlaylistsPersistentAverage();
};