#pragma once

#include <memory>
#include <sstream>

class CanvasWrapper;
class GameStats;
class GameStatsSummary;

class Renderer {
private:
	void RenderText(CanvasWrapper* canvas, std::string text, Vector2 textPos);
	void RenderText(CanvasWrapper* canvas, std::stringstream& text, Vector2 textPos);
	void RenderText(CanvasWrapper* canvas, float value, Vector2 textPos, std::stringstream& ss, int precision = 2);

public:
	std::shared_ptr<int> posX;
	std::shared_ptr<int> posY;
	std::shared_ptr<float> scale;

	Renderer();

	void RenderStats(CanvasWrapper* canvas, GameStatsSummary& gameStats);

	void ResetScale();
};