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

#include <pch.h>
#include "Renderer.h"

#include <DemoTouchRatio.h>

#define COUNTDATA_BOOSTFIELDSIDE(boostType) \
(*boostType ? ((*boostType##Sum && !(*boostType##Own && *boostType##Opponent && *boostType##Neutral))? 2 : 1) : 0) + \
((!*boostType && *boostType##Sum) ? 1 : 0) + \
((!*boostType##Sum && *boostType##Own) ? 1 : 0) + \
((!*boostType##Sum && *boostType##Opponent) ? 1 : 0) + \
((!*boostType##Sum && *boostType##Neutral) ? 1 : 0) 

Vector2 Renderer::GetBox()
{
	int width = X_OFFSET,
		height = Y_OFFSET;

	DemoTouchRatio& app = DemoTouchRatio::Instance();

	int numberOfDescriptionCells = 2 // description + last is always shown
		+ (DemoTouchRatio::Instance().CanRenderInMatches() ? 1 : 0) // current
		+ (ShouldShowTotal() ? 1 : 0) // total
		+ (ShouldShowAverage() ? 1 : 0) // average
		+ (ShouldShowPlaylistsTotal() ? 1 : 0) // playlists total
		+ (ShouldShowPlaylistsAverage() ? 1 : 0) // playlists average
		+ (ShouldShowPersistentTotal() ? 1 : 0) // persistent total
		+ (ShouldShowPersistentAverage() ? 1 : 0) // persistent average
		+ (ShouldShowPlaylistsPersistentTotal() ? 1 : 0) // playlists persistent total
		+ (ShouldShowPlaylistsPersistentAverage() ? 1 : 0) // playlists persistent average
		;

	int numberOfDataShown = 
		(*displayBumps ? 1 : 0) +
		(*displayTeamBumps ? 1 : 0) +
		(*displayDemos ? 1 : 0) +
		(*displayTeamDemos ? 1 : 0) +
		(*displayDeaths ? 1 : 0) +
		(*displayBallHits ? 1 : 0) +
		(*displayBoostUsage ? 1 : 0) +
		(*displayBoostPMinute ? 1 : 0) +
		COUNTDATA_BOOSTFIELDSIDE(displayBoostCollected) +
		COUNTDATA_BOOSTFIELDSIDE(displayBoostCollectedPMinute) +
		COUNTDATA_BOOSTFIELDSIDE(displayBoostOverfill) +
		COUNTDATA_BOOSTFIELDSIDE(displayBoostOverfillPMinute) +
		(*displayInAirPercentage ? 1 : 0) +
		(*displayPowerslideCount ? 1 : 0) +
		(*displayPowerslideDuration ? 1 : 0) +
		(*displayPowerslideDurationPerUse ? 1 : 0) +
		(*displayShots ? 1 : 0) +
		(*displayGoals ? 1 : 0) +
		(*displayAssists ? 1 : 0) +
		(*displaySaves ? 1 : 0);

	if (*renderHorizontal)
	{
		width += (!*customDescSize ? DEFAULT_COLUMN_SIZE : *columnSize) + (numberOfDataShown * *columnSize);
		height += *rowSize * numberOfDescriptionCells + (!*customDescSize ? (DEFAULT_ROW_SIZE - *rowSize) : 0.f);
	}
	else
	{
		width += *columnSize * numberOfDescriptionCells + (!*customDescSize ? (DEFAULT_COLUMN_SIZE - *columnSize) : 0.f);
		height += (!*customDescSize ? DEFAULT_ROW_SIZE : *rowSize) + (numberOfDataShown * *rowSize);
	}
	// Include title
	if (*renderTitle)
	{
		height += *rowSize * TITLE_SIZE;
	}
	// Calculate box size including the scale
	return Vector2{ (int)((float)width * *scale), (int)((float)height * *scale) };
}

void Renderer::RenderText(CanvasWrapper* canvas, std::string text, int columnId, int rowId, bool isRowDesc = false, bool isColDesc = false)
{
	// Take custom size off into account
	float offsetColumn = 0.f,
		offsetRow = 0.f;
	if (!*customDescSize && !isRowDesc)
	{
		offsetColumn = DEFAULT_COLUMN_SIZE;
		--columnId;
	}

	if (!*customDescSize && !isColDesc)
	{
		offsetRow = DEFAULT_ROW_SIZE;
		--rowId;
	}

	int titleSize = 0;
	if (*renderTitle)
	{
		titleSize = int(*rowSize * TITLE_SIZE);
	}

	canvas->SetPosition(Vector2{ *posX + (int)((X_OFFSET + offsetColumn + (columnId * *columnSize)) * *scale), titleSize + *posY + (int)((Y_OFFSET + offsetRow + (rowId * *rowSize)) * *scale)});
	canvas->DrawString(text, *scale, *scale);
}

void Renderer::RenderText(CanvasWrapper* canvas, float value, int columnId, int rowId, std::stringstream& ss, int precision)
{
	ss.str("");
	ss << std::fixed << std::setprecision(precision) << value;
	RenderText(canvas, ss.str(), columnId, rowId);
}

void Renderer::RenderText(CanvasWrapper* canvas, std::stringstream& text, int columnId, int rowId, bool isRowDesc = false, bool isColDesc = false)
{
	RenderText(canvas, text.str(), columnId, rowId, isRowDesc, isColDesc);
}

bool Renderer::ShouldShowTotal()
{
	return *displayTotal;
}
bool Renderer::ShouldShowAverage()
{
	return *displayAverage;
}
bool Renderer::ShouldShowPlaylistsTotal()
{
	return *displayPlaylistsTotal;
}
bool Renderer::ShouldShowPlaylistsAverage()
{
	return *displayPlaylistsAverage;
}

bool Renderer::ShouldShowPersistentTotal()
{
	return DemoTouchRatio::Instance().UsesPersistentStats() && *displayPersistentTotal;
}
bool Renderer::ShouldShowPersistentAverage()
{
	return DemoTouchRatio::Instance().UsesPersistentStats() && *displayPersistentAverage;
}
bool Renderer::ShouldShowPlaylistsPersistentTotal()
{
	return DemoTouchRatio::Instance().UsesPersistentStats() && *displayPlaylistsPersistentTotal;
}
bool Renderer::ShouldShowPlaylistsPersistentAverage()
{
	return DemoTouchRatio::Instance().UsesPersistentStats() && *displayPlaylistsPersistentAverage;
}