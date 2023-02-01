#include <pch.h>
#include "Renderer.h"

#include <DemoTouchRatio.h>

Vector2 Renderer::GetBox()
{
	int width = X_OFFSET,
		height = Y_OFFSET;

	DemoTouchRatio& app = DemoTouchRatio::Instance();

	int numberOfDescriptionCells = 2 // description + last is always shown
		+ (DemoTouchRatio::Instance().CanRenderInMatches() ? 1 : 0) // current
		+ (ShouldShowTotal() ? 1 : 0) // total
		+ (ShouldShowAverage() ? 1 : 0) // average
		+ (ShouldShowPersistentTotal() ? 1 : 0) // persistent total
		+ (ShouldShowPersistentAverage() ? 1 : 0) // persistent average
		;

	int numberOfDataShown = 
		(*displayBumps ? 1 : 0) +
		(*displayTeamBumps ? 1 : 0) +
		(*displayDemos ? 1 : 0) +
		(*displayBallHits ? 1 : 0) +
		(*displayBoostUsage ? 1 : 0) +
		(*displayBoostPMinute ? 1 : 0) +
		(*displayInAirPercentage ? 1 : 0);

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
	height += *rowSize * TITLE_SIZE;
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

	canvas->SetPosition(Vector2{ *posX + (int)((X_OFFSET + offsetColumn + (columnId * *columnSize)) * *scale), (int)(*rowSize * TITLE_SIZE) + *posY + (int)((Y_OFFSET + offsetRow + (rowId * *rowSize)) * *scale)});
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
bool Renderer::ShouldShowPersistentTotal()
{
	return DemoTouchRatio::Instance().UsesPersistentStats() && *displayPersistentTotal;
}
bool Renderer::ShouldShowPersistentAverage()
{
	return DemoTouchRatio::Instance().UsesPersistentStats() && *displayPersistentAverage;
}