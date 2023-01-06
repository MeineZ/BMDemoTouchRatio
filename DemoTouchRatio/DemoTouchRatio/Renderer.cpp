#include <pch.h>
#include "Renderer.h"

#include <GameStatsSummary.h>
#include <GameStats.h>
#include <DemoTouchRatio.h>

int columStart = 10;
int columnSize = 90;
int rowStart = 5;
int rowSize = 16;

Renderer::Renderer() :
	posX(std::make_shared<int>(0)),
	posY(std::make_shared<int>(500)),
	scale(std::make_shared<float>(1.0f)),
	colorBackground(std::make_shared<LinearColor>(LinearColor())),
	colorText(std::make_shared<LinearColor>(LinearColor())),
	displayBumps(std::make_shared<bool>(true)),
	displayTeamBumps(std::make_shared<bool>(false)),
	displayDemos(std::make_shared<bool>(true)),
	displayBallHits(std::make_shared<bool>(true))
{ }

void Renderer::RenderStats(CanvasWrapper* canvas, GameStatsSummary& gameStats) {
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
	canvas->FillBox(Vector2{ (int)(GetBoxWidth() * *scale), (int)(90.0f * *scale)});

	int currentColumn = 0;

	// DRAW FIRST COLUMN
	int currentRow = 1;
	RenderText(canvas, "Current", currentColumn, currentRow++);
	RenderText(canvas, "Latest", currentColumn, currentRow++);
	RenderText(canvas, "Total", currentColumn, currentRow++);
	stringStream.str("");
	stringStream << "Avg. (" << gameStats.GetNumberOfGames() << ")";
	RenderText(canvas, stringStream, currentColumn, currentRow++);
	++currentColumn;


	// DRAW BUMPS COLUMN
	if (*displayBumps) {
		currentRow = 0;
		RenderText(canvas, "Bumps", currentColumn, currentRow++);
		if (canRenderInMatches)
			RenderText(canvas, gameStats.GetCurrent().bumps, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetLast().bumps, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetTotal().bumps, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetAverage().bumps, currentColumn, currentRow++, stringStream, 2);
		++currentColumn;
	}


	// DRAW TEAM BUMPS COLUMN
	if (*displayTeamBumps) {
		currentRow = 0;
		RenderText(canvas, "Team bumps", currentColumn, currentRow++);
		if (canRenderInMatches)
			RenderText(canvas, gameStats.GetCurrent().teamBumps, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetLast().teamBumps, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetTotal().teamBumps, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetAverage().teamBumps, currentColumn, currentRow++, stringStream, 2);
		++currentColumn;
	}

	// DRAW DEMOS COLUMN
	if (*displayDemos) {
		currentRow = 0;
		RenderText(canvas, "Demos", currentColumn, currentRow++);
		if (canRenderInMatches)
			RenderText(canvas, gameStats.GetCurrent().demos, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetLast().demos, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetTotal().demos, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetAverage().demos, currentColumn, currentRow++, stringStream, 2);
		++currentColumn;
	}

	// DRAW BALL HITS COLUMN
	if (*displayBallHits) {
		currentRow = 0;
		RenderText(canvas, "Ball hits", currentColumn, currentRow++);
		if (canRenderInMatches)
			RenderText(canvas, gameStats.GetCurrent().ballHits, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetLast().ballHits, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetTotal().ballHits, currentColumn, currentRow++, stringStream, 0);
		RenderText(canvas, gameStats.GetAverage().ballHits, currentColumn, currentRow++, stringStream, 2);
		++currentColumn;
	}
}

void Renderer::RenderText(CanvasWrapper* canvas, std::string text, int columnId, int rowId)
{
	canvas->SetColor(*colorText);
	canvas->SetPosition(Vector2{ *posX + (int)((columStart + (columnId * columnSize)) * *scale), *posY + (int)((rowStart + (rowId * rowSize)) * *scale) });
	canvas->DrawString(text, *scale, *scale);
}

void Renderer::RenderText(CanvasWrapper* canvas, float value, int columnId, int rowId, std::stringstream& ss, int precision)
{
	ss.str("");
	ss << std::fixed << std::setprecision(precision) << value;
	RenderText(canvas, ss.str(), columnId, rowId);
}

void Renderer::RenderText(CanvasWrapper* canvas, std::stringstream& text, int columnId, int rowId) {
	RenderText(canvas, text.str(), columnId, rowId);
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

int Renderer::GetBoxWidth()
{
	return columnSize +
		(*displayBumps ? columnSize : 0) +
		(*displayTeamBumps ? columnSize : 0) +
		(*displayDemos ? columnSize : 0) +
		(*displayBallHits ? columnSize : 0);
}