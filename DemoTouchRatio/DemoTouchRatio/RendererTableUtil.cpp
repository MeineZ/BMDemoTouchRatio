#include <pch.h>
#include "Renderer.h"

#include <Constants.h>
#include <GameStatsSummary.h>
#include <GameStats.h>
#include <DemoTouchRatio.h>
#include <PersistentStats.h>

void Renderer::RenderHeader(CanvasWrapper *canvas, int nOfGames, int persistentNOfGames)
{
	std::stringstream stringStream;

	int increment = 1;
	std::function<void(std::string, int)> renderCell = [this, canvas](std::string label, int increment) {
		RenderText(canvas, label, *renderHorizontal ? 0 : increment, *renderHorizontal ? increment : 0, *renderHorizontal, !*renderHorizontal);
	};

	// Render time!
	if (DemoTouchRatio::Instance().CanRenderInMatches())
		renderCell("Current", increment++);

	renderCell("Latest", increment++);

	if (ShouldShowTotal())
		renderCell("Total", increment++);
	if (ShouldShowAverage())
	{
		stringStream << "Avg. #" << nOfGames;
		renderCell(stringStream.str(), increment++);
	}

	if (ShouldShowPersistentTotal())
	{
		stringStream.str("");
		if (ShouldShowPersistentAverage()) stringStream << "[Total]";
		else stringStream << "[Total #" << persistentNOfGames << "]";

		renderCell(stringStream.str(), increment++);
	}
	if (ShouldShowPersistentAverage())
	{
		stringStream.str("");
		stringStream << "[Avg. #" << persistentNOfGames << "]";
		renderCell(stringStream.str(), increment++);
	}
}

void Renderer::RenderData(CanvasWrapper* canvas, int nth, std::string label, STATS_RENDER_ARGUMENTS)
{
	std::stringstream stringStream;

	int increment = 0;
	std::function<void(std::string, int)> renderCellString = [this, canvas, nth](std::string label, int increment) {
		RenderText(canvas, label, *renderHorizontal ? nth : increment, *renderHorizontal ? increment : nth, !*renderHorizontal, *renderHorizontal);
	};
	std::function<void(float, int, int, std::stringstream&)> renderCellFloat = [this, canvas, nth](float value, int increment, int decimals, std::stringstream & stringStream) {
		RenderText(canvas, value, *renderHorizontal ? nth : increment, *renderHorizontal ? increment : nth, stringStream, decimals);
	};

	renderCellString(label, increment++);

	if (DemoTouchRatio::Instance().CanRenderInMatches())
		renderCellFloat(current, increment++, 0, stringStream);

	renderCellFloat(last, increment++, 0, stringStream);

	if (ShouldShowTotal())
		renderCellFloat(total, increment++, 0, stringStream);
	if (ShouldShowAverage())
		renderCellFloat(average, increment++, 2, stringStream);

	if (ShouldShowPersistentTotal())
		renderCellFloat(persistentTotal, increment++, 0, stringStream);
	if (ShouldShowPersistentAverage())
		renderCellFloat(persistentAverage, increment++, 2, stringStream);
}