#include <pch.h>
#include "Renderer.h"

#include <Constants.h>
#include <GameStatsSummary.h>
#include <GameStats.h>
#include <DemoTouchRatio.h>
#include <PersistentStats.h>

#include <PlaylistEnum.h>

void Renderer::RenderTitle(CanvasWrapper* canvas)
{
	if (!*renderTitle)
		return;

	canvas->SetPosition(Vector2{ *posX + X_OFFSET, *posY + Y_OFFSET });

	std::stringstream sstream;
	sstream << PlaylistHelpers::GetPlaylistName(DemoTouchRatio::Instance().GetCurrentShownPlaylist()) << (DemoTouchRatio::Instance().IsPlaylistForced() ? " (manual selection)" : "");
	canvas->DrawString(sstream.str(), TITLE_SIZE * *scale, TITLE_SIZE * *scale);
}

void Renderer::RenderHeader(CanvasWrapper *canvas, int nOfGames, int playlistsNOfGames, int persistentNOfGames, int playlistsPersistentNOfGames)
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
	{
		stringStream.str("");
		if (ShouldShowAverage()) stringStream << "Total";
		else stringStream << "Total #" << nOfGames;
		renderCell(stringStream.str(), increment++);
	}
	if (ShouldShowAverage())
	{
		stringStream.str("");
		stringStream << "Avg. #" << nOfGames;
		renderCell(stringStream.str(), increment++);
	}
	if (ShouldShowPlaylistsTotal())
	{
		stringStream.str("");
		if (ShouldShowPlaylistsAverage()) stringStream << "<Total>";
		else stringStream << "<Total #" << playlistsNOfGames << ">";
		renderCell(stringStream.str(), increment++);
	}
	if (ShouldShowPlaylistsAverage())
	{
		stringStream.str("");
		stringStream << "<Avg. #" << playlistsNOfGames << ">";
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
	if (ShouldShowPlaylistsPersistentTotal())
	{
		stringStream.str("");
		if (ShouldShowPlaylistsPersistentAverage()) stringStream << "[[Total]]";
		else stringStream << "[[Total #" << playlistsPersistentNOfGames << "]]";

		renderCell(stringStream.str(), increment++);
	}
	if (ShouldShowPlaylistsPersistentAverage())
	{
		stringStream.str("");
		stringStream << "[[Avg. #" << playlistsPersistentNOfGames << "]]";
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
	if (ShouldShowPlaylistsTotal())
		renderCellFloat(playlistsTotal, increment++, 0, stringStream);
	if (ShouldShowPlaylistsAverage())
		renderCellFloat(playlistsAverage, increment++, 2, stringStream);

	if (ShouldShowPersistentTotal())
		renderCellFloat(persistentTotal, increment++, 0, stringStream);
	if (ShouldShowPersistentAverage())
		renderCellFloat(persistentAverage, increment++, 2, stringStream);
	if (ShouldShowPlaylistsPersistentTotal())
		renderCellFloat(persistentPlaylistsTotal, increment++, 0, stringStream);
	if (ShouldShowPlaylistsPersistentAverage())
		renderCellFloat(persistentPlaylistsAverage, increment++, 2, stringStream);
}