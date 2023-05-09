#include <pch.h>
#include "Renderer.h"

#include <Constants.h>
#include <GameStatsSummary.h>
#include <GameStats.h>
#include <DemoTouchRatio.h>
#include <PersistentStats.h>

#define STATS_RENDER_ARGUMENTS_SOURCE(propName, propFunc) \
gameStats.GetCurrent().propName, \
gameStats.GetLast().propName, \
gameStats.GetTotal().propName, \
gameStats.GetAverage().propName, \
playlistsStats.GetTotal().propName, \
playlistsStats.GetAverage().propName, \
persistentStats.GetTotalStats().propFunc(), \
persistentStats.GetAverageStats().propName, \
playlistsPersistent.GetTotalStats().propFunc(), \
playlistsPersistent.GetAverageStats().propName

Renderer::Renderer() :
	posX(std::make_shared<int>(0)),
	posY(std::make_shared<int>(500)),
	scale(std::make_shared<float>(1.0f)),
	colorBackground(std::make_shared<LinearColor>(LinearColor(0, 0, 0, 75))),
	colorText(std::make_shared<LinearColor>(LinearColor(255, 255, 255, 127))),
	rowSize(std::make_shared<float>(DEFAULT_ROW_SIZE)),
	columnSize(std::make_shared<float>(DEFAULT_COLUMN_SIZE)),
	// [STAT_ADD] 4. Add display bool
	displayBumps(std::make_shared<bool>(true)),
	displayTeamBumps(std::make_shared<bool>(false)),
	displayDemos(std::make_shared<bool>(true)),
	displayTeamDemos(std::make_shared<bool>(false)),
	displayDeaths(std::make_shared<bool>(false)),
	displayBallHits(std::make_shared<bool>(true)),
	displayBoostUsage(std::make_shared<bool>(false)),
	displayBoostPMinute(std::make_shared<bool>(false)),
	displayBoostCollected(std::make_shared<bool>(false)),
	displayBoostCollectedPMinute(std::make_shared<bool>(false)),
	displayBoostOverfill(std::make_shared<bool>(false)),
	displayBoostOverfillPMinute(std::make_shared<bool>(false)),
	displayInAirPercentage(std::make_shared<bool>(false)),
	displayPowerslideCount(std::make_shared<bool>(false)),
	displayPowerslideDuration(std::make_shared<bool>(false)),
	displayPowerslideDurationPerUse(std::make_shared<bool>(false)),
	displayShots(std::make_shared<bool>(false)),
	displayGoals(std::make_shared<bool>(false)),
	displaySaves( std::make_shared<bool>(false)),
	displayAssists(std::make_shared<bool>(false)),
	displayTotal(std::make_shared<bool>(true)),
	displayAverage(std::make_shared<bool>(true)),
	displayPlaylistsTotal(std::make_shared<bool>(false)),
	displayPlaylistsAverage(std::make_shared<bool>(false)),
	displayPersistentTotal(std::make_shared<bool>(false)),
	displayPersistentAverage(std::make_shared<bool>(false)),
	displayPlaylistsPersistentTotal(std::make_shared<bool>(false)),
	displayPlaylistsPersistentAverage(std::make_shared<bool>(false)),
	renderTitle(std::make_shared<bool>(true)),
	renderHorizontal(std::make_shared<bool>(true)),
	customDescSize(std::make_shared<bool>(false))
{ }

void Renderer::ResetScale() {
	*scale = 1.f;
}

void Renderer::ResetColors()
{
	colorBackground->R = 0.f;
	colorBackground->G = 0.f;
	colorBackground->B = 0.f;
	colorBackground->A = 75.f;

	colorText->R = 255.f;
	colorText->G = 255.f;
	colorText->B = 255.f;
	colorText->A = 127.f;
}

void Renderer::ResetTableSizes()
{
	if (*renderHorizontal)
	{
		*rowSize = DEFAULT_ROW_SIZE;
		*columnSize = DEFAULT_COLUMN_SIZE;
	}
	else
	{
		*rowSize = DEFAULT_ROW_SIZE;
		*columnSize = DEFAULT_COLUMN_SIZE;
	}
}

void Renderer::RenderStats(CanvasWrapper* canvas, GameStatsSummary& gameStats, PersistentStats &persistentStats, GameStatsSummary& playlistsStats, PersistentStats& playlistsPersistent)
{
	std::stringstream stringStream;
	DemoTouchRatio app = DemoTouchRatio::Instance();
	bool canRenderInMatches = DemoTouchRatio::Instance().CanRenderInMatches();

	Vector2 position = {
		*posX,
		*posY
	};

	// DRAW BOX
	canvas->SetColor(*colorBackground);
	canvas->SetPosition(position);
	Vector2 size = GetBox();
	canvas->FillBox(size);

	canvas->SetColor(*colorText);

	RenderTitle(canvas, size.X);
	RenderHeader(canvas, gameStats.GetNumberOfGames(), playlistsStats.GetNumberOfGames(), persistentStats.GetNumberOfGames(), playlistsPersistent.GetNumberOfGames());

	int nth = 1;
	if(*displayBumps) RenderData(canvas, nth++, "Bumps", STATS_RENDER_ARGUMENTS_SOURCE(bumps, GetBumps));
	if (*displayTeamBumps) RenderData(canvas, nth++, "T.Bumps", STATS_RENDER_ARGUMENTS_SOURCE(teamBumps, GetTeamBumps));
	if (*displayDemos) RenderData(canvas, nth++, "Demos", STATS_RENDER_ARGUMENTS_SOURCE(demos, GetDemos));
	if (*displayTeamDemos) RenderData(canvas, nth++, "T.Demos", STATS_RENDER_ARGUMENTS_SOURCE(teamDemos, GetTeamDemos));
	if (*displayDeaths) RenderData(canvas, nth++, "Deaths", STATS_RENDER_ARGUMENTS_SOURCE(deaths, GetDeaths));
	if (*displayBallHits) RenderData(canvas, nth++, "Ball hits", STATS_RENDER_ARGUMENTS_SOURCE(ballHits, GetBallHits));
	if (*displayBoostUsage) RenderData(canvas, nth++, "Boost", STATS_RENDER_ARGUMENTS_SOURCE(totalBoostUsed, GetBoostUsed));
	if (*displayBoostPMinute) RenderData(canvas, nth++, "Boost/min", STATS_RENDER_ARGUMENTS_SOURCE(boostPMinute, GetBoostPMinute));
	if (*displayBoostCollected) RenderData(canvas, nth++, "Boost col", STATS_RENDER_ARGUMENTS_SOURCE(totalBoostCollected, GetBoostCollected));
	if (*displayBoostCollectedPMinute) RenderData(canvas, nth++, "Boost col/min", STATS_RENDER_ARGUMENTS_SOURCE(boostCollectedPMinute, GetBoostCollectedPMinute));
	if (*displayBoostOverfill) RenderData(canvas, nth++, "Overfill", STATS_RENDER_ARGUMENTS_SOURCE(totalBoostOverfill, GetBoostOverfill));
	if (*displayBoostOverfillPMinute) RenderData(canvas, nth++, "Overfill/min", STATS_RENDER_ARGUMENTS_SOURCE(boostOverfillPMinute, GetBoostOverfillPMinute));
	if (*displayInAirPercentage) RenderData(canvas, nth++, "Air %", STATS_RENDER_ARGUMENTS_SOURCE(inAirPercentage, GetInAirPercentage));
	if (*displayPowerslideCount) RenderData(canvas, nth++, "Pwrslide uses", STATS_RENDER_ARGUMENTS_SOURCE(powerslideCount, GetPowerslideCount));
	if (*displayPowerslideDuration) RenderData(canvas, nth++, "Pwrslide time", STATS_RENDER_ARGUMENTS_SOURCE(powerslideDuration * 60.0f, GetPowerslideTimeInSeconds), 1);
	if (*displayPowerslideDurationPerUse) RenderData(canvas, nth++, "Pwrslide t/use", STATS_RENDER_ARGUMENTS_SOURCE(powerslideDurationPerUse * 60.0f, GetPowerslideTimePerUseInSeconds), 2);
	if (*displayShots) RenderData(canvas, nth++, "Shots", STATS_RENDER_ARGUMENTS_SOURCE(shots, GetShots));
	if (*displayGoals) RenderData(canvas, nth++, "Goals", STATS_RENDER_ARGUMENTS_SOURCE(goals, GetGoals));
	if( *displayAssists) RenderData( canvas, nth++, "Assists", STATS_RENDER_ARGUMENTS_SOURCE(assists, GetAssists));
	if( *displaySaves) RenderData( canvas, nth++, "Saves", STATS_RENDER_ARGUMENTS_SOURCE(saves, GetSaves));
}