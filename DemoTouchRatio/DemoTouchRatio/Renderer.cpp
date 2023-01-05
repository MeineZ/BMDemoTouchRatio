#include <pch.h>
#include "Renderer.h"

#include <GameStatsSummary.h>

LinearColor backgroundColor = LinearColor(0.f, 0.f, 0.f, 75.f);
LinearColor textColor = LinearColor(255.f, 255.f, 255.f, 127.f);
int column1 = 10;
int column2 = 100;
int column3 = 190;
int column4 = 280;
int row1 = 5;
int row2 = 21;
int row3 = 37;
int row4 = 53;
int row5 = 69;

Renderer::Renderer() :
	posX(std::make_shared<int>(0)),
	posY(std::make_shared<int>(500)),
	scale(std::make_shared<float>(1.0f))
{ }

void Renderer::RenderStats(CanvasWrapper* canvas, GameStatsSummary& gameStats) {
	std::stringstream stringStream;

	Vector2 position = {
		*posX,
		*posY
	};

	// DRAW BOX
	canvas->SetColor(backgroundColor);
	canvas->SetPosition(position);
	canvas->FillBox(Vector2{ (int)(365.0f * *scale), (int)(90.0f * *scale) });

	// DRAW HEADER
	RenderText(canvas, "Bumps", Vector2{ column2, row1 });
	RenderText(canvas, "Demos", Vector2{ column3, row1 });
	RenderText(canvas, "Ball Hits", Vector2{ column4, row1 });

	// DRAW CURRENT GAME
	RenderText(canvas, "Current", Vector2{ column1, row2 });
	RenderText(canvas, gameStats.GetCurrent().bumps, Vector2{column2, row2}, stringStream, 0);
	RenderText(canvas, gameStats.GetCurrent().demos, Vector2{ column3, row2 }, stringStream, 0);
	RenderText(canvas, gameStats.GetCurrent().ballHits, Vector2{ column4, row2 }, stringStream, 0);

	// DRAW LAST GAME
	RenderText(canvas, "Last", Vector2{ column1, row3 });
	RenderText(canvas, gameStats.GetLast().bumps, Vector2{ column2, row3 }, stringStream, 0);
	RenderText(canvas, gameStats.GetLast().demos, Vector2{ column3, row3 }, stringStream, 0);
	RenderText(canvas, gameStats.GetLast().ballHits, Vector2{ column4, row3 }, stringStream, 0);

	// DRAW SESSION TOTAL
	RenderText(canvas, "Total", Vector2{ column1, row4 });
	RenderText(canvas, gameStats.GetTotal().bumps, Vector2{ column2, row4 }, stringStream, 0);
	RenderText(canvas, gameStats.GetTotal().demos, Vector2{ column3, row4 }, stringStream, 0);
	RenderText(canvas, gameStats.GetTotal().ballHits, Vector2{ column4, row4 }, stringStream, 0);

	// DRAW AVERAGE
	stringStream.str("");
	stringStream << "Avg. (" << gameStats.GetNumberOfGames() << ")";
	RenderText(canvas, stringStream, Vector2{ column1, row5 });
	RenderText(canvas, gameStats.GetAverage().bumps, Vector2{column2, row5}, stringStream, 2);
	RenderText(canvas, gameStats.GetAverage().demos, Vector2{ column3, row5 }, stringStream, 2);
	RenderText(canvas, gameStats.GetAverage().ballHits, Vector2{ column4, row5 }, stringStream, 2);
}

void Renderer::RenderText(CanvasWrapper* canvas, std::string text, Vector2 textPos)
{
	canvas->SetColor(textColor);
	canvas->SetPosition(Vector2{ *posX + (int)(textPos.X * *scale), *posY + (int)(textPos.Y * *scale) });
	canvas->DrawString(text, *scale, *scale);
}

void Renderer::RenderText(CanvasWrapper* canvas, float value, Vector2 textPos, std::stringstream& ss, int precision)
{
	ss.str("");
	ss << std::fixed << std::setprecision(precision) << value;
	RenderText(canvas, ss.str(), textPos);
}

void Renderer::RenderText(CanvasWrapper* canvas, std::stringstream& text, Vector2 textPos) {
	RenderText(canvas, text.str(), textPos);
}

void Renderer::ResetScale() {
	*scale = 1.f;
}