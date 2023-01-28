#include <pch.h>
#include "Renderer.h"

#include <DemoTouchRatio.h>

#define X_OFFSET 10
#define Y_OFFSET 5

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
			(*displayBallHits ? *columnSize : 0) +
			(*displayBoostUsage ? *columnSize : 0) +
			(*displayBoostPMinute ? *columnSize : 0);
		height += *rowSize * numberOfDescriptionCells + (!*customDescSize ? (DEFAULT_ROW_SIZE - *rowSize) : 0.f);
	}
	else
	{
		width += *columnSize * numberOfDescriptionCells + (!*customDescSize ? (DEFAULT_COLUMN_SIZE - *columnSize) : 0.f);
		height += (!*customDescSize ? DEFAULT_ROW_SIZE : *rowSize) +
			(*displayBumps ? *rowSize : 0) +
			(*displayTeamBumps ? *rowSize : 0) +
			(*displayDemos ? *rowSize : 0) +
			(*displayBallHits ? *rowSize : 0) +
			(*displayBoostUsage ? *rowSize : 0) +
			(*displayBoostPMinute ? *rowSize : 0);
	}
	return Vector2{ (int)((float)width * *scale), (int)((float)height * *scale) };
}

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

	canvas->SetPosition(Vector2{ *posX + (int)((X_OFFSET + offsetColumn + (columnId * *columnSize)) * *scale), *posY + (int)((Y_OFFSET + offsetRow + (rowId * *rowSize)) * *scale) });
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