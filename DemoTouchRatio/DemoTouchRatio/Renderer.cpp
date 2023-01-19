#include <pch.h>
#include "Renderer.h"

#include <Constants.h>
#include <GameStatsSummary.h>
#include <GameStats.h>
#include <DemoTouchRatio.h>
#include <PersistentStats.h>

#define DEFAULT_ROW_SIZE 16.f
#define DEFAULT_COLUMN_SIZE 100.f

int columStart = 10;
int rowStart = 5;

Renderer::Renderer() :
	posX(std::make_shared<int>(0)),
	posY(std::make_shared<int>(500)),
	scale(std::make_shared<float>(1.0f)),
	colorBackground(std::make_shared<LinearColor>(LinearColor(0, 0, 0, 75))),
	colorText(std::make_shared<LinearColor>(LinearColor(255, 255, 255, 127))),
	rowSize(std::make_shared<float>(DEFAULT_ROW_SIZE)),
	columnSize(std::make_shared<float>(DEFAULT_COLUMN_SIZE)),
	displayBumps(std::make_shared<bool>(true)),
	displayTeamBumps(std::make_shared<bool>(false)),
	displayDemos(std::make_shared<bool>(true)),
	displayBallHits(std::make_shared<bool>(true)),
	displayPersistentTotal(std::make_shared<bool>(false)),
	displayPersistentAverage(std::make_shared<bool>(false)),
	replaceSessionTotal(std::make_shared<bool>(false)),
	replaceSessionAverage(std::make_shared<bool>(false)),
	renderHorizontal(std::make_shared<bool>(true)),
	customDescSize(std::make_shared<bool>(false))
{ }

void Renderer::RenderText(CanvasWrapper* canvas, std::string text, int columnId, int rowId, bool isRowDesc = false, bool isColDesc = false)
{
	canvas->SetColor(*colorText);

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

	canvas->SetPosition(Vector2{ *posX + (int)((columStart + offsetColumn + (columnId * *columnSize)) * *scale), *posY + (int)((rowStart + offsetRow + (rowId * *rowSize)) * *scale) });
	canvas->DrawString(text, *scale, *scale);
}

void Renderer::RenderText(CanvasWrapper* canvas, float value, int columnId, int rowId, std::stringstream& ss, int precision)
{
	ss.str("");
	ss << std::fixed << std::setprecision(precision) << value;
	RenderText(canvas, ss.str(), columnId, rowId);
}

void Renderer::RenderText(CanvasWrapper* canvas, std::stringstream& text, int columnId, int rowId, bool isRowDesc = false, bool isColDesc = false) {
	RenderText(canvas, text.str(), columnId, rowId, isRowDesc, isColDesc);
}

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

Vector2 Renderer::GetBox()
{
	int width = 5.f,
		height = 8.f;

	DemoTouchRatio& app = DemoTouchRatio::Instance();

	int numberOfDescriptionCells = 4 
		+ (DemoTouchRatio::Instance().CanRenderInMatches() ? 1 : 0)
		+ (
			(ShouldShowPersistentTotal() && ShouldShowTotal() ? 1 : 0) +
			(ShouldShowPersistentAverage() && ShouldShowAverage() ? 1 : 0)
		);

	if (*renderHorizontal)
	{
		width += (!*customDescSize ? DEFAULT_COLUMN_SIZE : *columnSize) +
			(*displayBumps ? *columnSize : 0) +
			(*displayTeamBumps ? *columnSize : 0) +
			(*displayDemos ? *columnSize : 0) +
			(*displayBallHits ? *columnSize : 0);
		height += *rowSize * numberOfDescriptionCells + (!*customDescSize ? (DEFAULT_ROW_SIZE - *rowSize) : 0.f);
	}
	else
	{
		width += numberOfDescriptionCells * *columnSize + (!*customDescSize ? (DEFAULT_COLUMN_SIZE - *columnSize) : 0.f);
		height += (!*customDescSize ? DEFAULT_ROW_SIZE : *rowSize) +
			(*displayBumps ? *rowSize : 0) +
			(*displayTeamBumps ? *rowSize : 0) +
			(*displayDemos ? *rowSize : 0) +
			(*displayBallHits ? *rowSize : 0);
	}
	return Vector2 {(int)((float)width * *scale), (int)((float)height * *scale) };
}

bool Renderer::ShouldShowTotal()
{
	return !ShouldShowPersistentTotal() || (ShouldShowPersistentTotal() && !*replaceSessionTotal);
}
bool Renderer::ShouldShowAverage()
{
	return !ShouldShowPersistentAverage() || (ShouldShowPersistentAverage() && !*replaceSessionAverage);
}
bool Renderer::ShouldShowPersistentTotal()
{
	return DemoTouchRatio::Instance().UsesPersistentStats() && *displayPersistentTotal;
}
bool Renderer::ShouldShowPersistentAverage()
{
	return DemoTouchRatio::Instance().UsesPersistentStats() && *displayPersistentAverage;
}

void Renderer::RenderStats(CanvasWrapper* canvas, GameStatsSummary& gameStats, PersistentStats &persistentStats)
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
	canvas->FillBox(GetBox());

	if (*renderHorizontal)
	{

		int currentColumn = 0;

		// DRAW FIRST COLUMN
		int currentRow = 1;
		if (canRenderInMatches)
			RenderText(canvas, "Current", currentColumn, currentRow++, true, false);
		RenderText(canvas, "Latest", currentColumn, currentRow++, true, false);
		if (ShouldShowTotal())
			RenderText(canvas, "Total", currentColumn, currentRow++, true, false);
		if (ShouldShowAverage())
		{
			stringStream.str("");
			stringStream << "Avg. #" << gameStats.GetNumberOfGames();
			RenderText(canvas, stringStream, currentColumn, currentRow++, true, false);
		}

		if (ShouldShowPersistentTotal())
		{
				stringStream.str("");
				if (ShouldShowPersistentAverage())
				{
					stringStream << "[Total]";
				}
				else
				{
					stringStream << "[Total #" << persistentStats.GetNumberOfGames() << "]";
				}
				RenderText(canvas, stringStream, currentColumn, currentRow++, true, false);
		}
		if (ShouldShowPersistentAverage())
		{
			stringStream.str("");
			stringStream << "[Avg. #" << persistentStats.GetNumberOfGames() << "]";
			RenderText(canvas, stringStream, currentColumn, currentRow++, true, false);
		}
		++currentColumn;


		// DRAW BUMPS COLUMN
		if (*displayBumps)
		{
			currentRow = 0;
			RenderText(canvas, "Bumps", currentColumn, currentRow++, false, true);
			if (canRenderInMatches)
				RenderText(canvas, gameStats.GetCurrent().bumps, currentColumn, currentRow++, stringStream, 0);
			RenderText(canvas, gameStats.GetLast().bumps, currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowTotal())
				RenderText(canvas, gameStats.GetTotal().bumps, currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowAverage())
				RenderText(canvas, gameStats.GetAverage().bumps, currentColumn, currentRow++, stringStream, 2);

			if (ShouldShowPersistentTotal())
				RenderText(canvas, persistentStats.GetTotalStats().GetBumps(), currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowPersistentAverage())
				RenderText(canvas, persistentStats.GetAverageStats().bumps, currentColumn, currentRow++, stringStream, 2);
			++currentColumn;
		}


		// DRAW TEAM BUMPS COLUMN
		if (*displayTeamBumps)
		{
			currentRow = 0;
			RenderText(canvas, "Team bumps", currentColumn, currentRow++, false, true);
			if (canRenderInMatches)
				RenderText(canvas, gameStats.GetCurrent().teamBumps, currentColumn, currentRow++, stringStream, 0);
			RenderText(canvas, gameStats.GetLast().teamBumps, currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowTotal())
				RenderText(canvas, gameStats.GetTotal().teamBumps, currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowAverage())
				RenderText(canvas, gameStats.GetAverage().teamBumps, currentColumn, currentRow++, stringStream, 2);

			if (ShouldShowPersistentTotal())
				RenderText(canvas, persistentStats.GetTotalStats().GetTeamBumps(), currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowPersistentAverage())
				RenderText(canvas, persistentStats.GetAverageStats().teamBumps, currentColumn, currentRow++, stringStream, 2);
			++currentColumn;
		}

		// DRAW DEMOS COLUMN
		if (*displayDemos)
		{
			currentRow = 0;
			RenderText(canvas, "Demos", currentColumn, currentRow++, false, true);
			if (canRenderInMatches)
				RenderText(canvas, gameStats.GetCurrent().demos, currentColumn, currentRow++, stringStream, 0);
			RenderText(canvas, gameStats.GetLast().demos, currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowTotal())
				RenderText(canvas, gameStats.GetTotal().demos, currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowAverage())
				RenderText(canvas, gameStats.GetAverage().demos, currentColumn, currentRow++, stringStream, 2);

			if (ShouldShowPersistentTotal())
				RenderText(canvas, persistentStats.GetTotalStats().GetDemos(), currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowPersistentAverage())
				RenderText(canvas, persistentStats.GetAverageStats().demos, currentColumn, currentRow++, stringStream, 2);
			++currentColumn;
		}

		// DRAW BALL HITS COLUMN
		if (*displayBallHits)
		{
			currentRow = 0;
			RenderText(canvas, "Ball hits", currentColumn, currentRow++, false, true);
			if (canRenderInMatches)
				RenderText(canvas, gameStats.GetCurrent().ballHits, currentColumn, currentRow++, stringStream, 0);
			RenderText(canvas, gameStats.GetLast().ballHits, currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowTotal())
				RenderText(canvas, gameStats.GetTotal().ballHits, currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowAverage())
				RenderText(canvas, gameStats.GetAverage().ballHits, currentColumn, currentRow++, stringStream, 2);

			if (ShouldShowPersistentTotal())
				RenderText(canvas, persistentStats.GetTotalStats().GetBallHits(), currentColumn, currentRow++, stringStream, 0);
			if (ShouldShowPersistentAverage())
				RenderText(canvas, persistentStats.GetAverageStats().ballHits, currentColumn, currentRow++, stringStream, 2);
			++currentColumn;
		}
	}
	// Render vertical
	else
	{
		int currentRow = 0;

		// DRAW FIRST ROW
		int currentColumn = 1;
		if (canRenderInMatches)
			RenderText(canvas, "Current", currentColumn++, currentRow, false, true);
		RenderText(canvas, "Latest", currentColumn++, currentRow, false, true);
		if (ShouldShowTotal())
			RenderText(canvas, "Total", currentColumn++, currentRow, false, true);
		if (ShouldShowAverage()) {
			stringStream.str("");
			stringStream << "Avg. #" << gameStats.GetNumberOfGames();
			RenderText(canvas, stringStream, currentColumn++, currentRow, false, true);
		}

		if (ShouldShowPersistentTotal())
		{
			stringStream.str("");
			stringStream << "[Total #" << persistentStats.GetNumberOfGames() << "]";
			RenderText(canvas, stringStream, currentColumn++, currentRow, false, true);
		}
		if (ShouldShowPersistentAverage())
		{
			stringStream.str("");
			stringStream << "[Avg. #" << persistentStats.GetNumberOfGames() << "]";
			RenderText(canvas, stringStream, currentColumn++, currentRow, false, true);
		}
		++currentRow;


		// DRAW BUMPS ROW
		if (*displayBumps)
		{
			currentColumn = 0;
			RenderText(canvas, "Bumps", currentColumn++, currentRow, true, false);
			if (canRenderInMatches)
				RenderText(canvas, gameStats.GetCurrent().bumps, currentColumn++, currentRow, stringStream, 0);
			RenderText(canvas, gameStats.GetLast().bumps, currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowTotal())
				RenderText(canvas, gameStats.GetTotal().bumps, currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowAverage())
				RenderText(canvas, gameStats.GetAverage().bumps, currentColumn++, currentRow, stringStream, 2);

			if (ShouldShowPersistentTotal())
				RenderText(canvas, persistentStats.GetTotalStats().GetBumps(), currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowPersistentAverage())
				RenderText(canvas, persistentStats.GetAverageStats().bumps, currentColumn++, currentRow, stringStream, 2);
			++currentRow;
		}

		// DRAW TEAM BUMPS ROW
		if (*displayTeamBumps)
		{
			currentColumn = 0;
			RenderText(canvas, "Team Bumps", currentColumn++, currentRow, true, false);
			if (canRenderInMatches)
				RenderText(canvas, gameStats.GetCurrent().teamBumps, currentColumn++, currentRow, stringStream, 0);
			RenderText(canvas, gameStats.GetLast().teamBumps, currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowTotal())
				RenderText(canvas, gameStats.GetTotal().teamBumps, currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowAverage())
				RenderText(canvas, gameStats.GetAverage().teamBumps, currentColumn++, currentRow, stringStream, 2);

			if (ShouldShowPersistentTotal())
				RenderText(canvas, persistentStats.GetTotalStats().GetTeamBumps(), currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowPersistentAverage())
				RenderText(canvas, persistentStats.GetAverageStats().teamBumps, currentColumn++, currentRow, stringStream, 2);
			++currentRow;
		}

		// DRAW DEMOS ROW
		if (*displayDemos)
		{
			currentColumn = 0;
			RenderText(canvas, "Demos", currentColumn++, currentRow, true, false);
			if (canRenderInMatches)
				RenderText(canvas, gameStats.GetCurrent().demos, currentColumn++, currentRow, stringStream, 0);
			RenderText(canvas, gameStats.GetLast().demos, currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowTotal())
				RenderText(canvas, gameStats.GetTotal().demos, currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowAverage())
				RenderText(canvas, gameStats.GetAverage().demos, currentColumn++, currentRow, stringStream, 2);

			if (ShouldShowPersistentTotal())
				RenderText(canvas, persistentStats.GetTotalStats().GetDemos(), currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowPersistentAverage())
				RenderText(canvas, persistentStats.GetAverageStats().demos, currentColumn++, currentRow, stringStream, 2);
			++currentRow;
		}

		// DRAW BALL HITS ROW
		if (*displayBallHits)
		{
			currentColumn = 0;
			RenderText(canvas, "Ball hits", currentColumn++, currentRow, true, false);
			if (canRenderInMatches)
				RenderText(canvas, gameStats.GetCurrent().ballHits, currentColumn++, currentRow, stringStream, 0);
			RenderText(canvas, gameStats.GetLast().ballHits, currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowTotal())
				RenderText(canvas, gameStats.GetTotal().ballHits, currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowAverage())
				RenderText(canvas, gameStats.GetAverage().ballHits, currentColumn++, currentRow, stringStream, 2);

			if (ShouldShowPersistentTotal())
				RenderText(canvas, persistentStats.GetTotalStats().GetBallHits(), currentColumn++, currentRow, stringStream, 0);
			if (ShouldShowPersistentAverage())
				RenderText(canvas, persistentStats.GetAverageStats().ballHits, currentColumn++, currentRow, stringStream, 2);
			++currentRow;
		}
	}
}