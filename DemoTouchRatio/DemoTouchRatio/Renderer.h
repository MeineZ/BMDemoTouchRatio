#pragma once

#include <memory>
#include <sstream>

class CanvasWrapper;
class GameStats;
class GameStatsSummary;
class PersistentStats;

#define DEFAULT_ROW_SIZE 16.f
#define DEFAULT_COLUMN_SIZE 100.f

#define STATS_RENDER_ARGUMENTS float current, float last, float total, float average, float persistentTotal, float persistentAverage

class Renderer {
private:

	// Private utils
	void RenderText(CanvasWrapper* canvas, std::string text, int columnId, int rowId, bool isRowDesc, bool isColDesc);
	void RenderText(CanvasWrapper* canvas, std::stringstream& text, int columnId, int rowId, bool isRowDesc, bool isColDesc);
	void RenderText(CanvasWrapper* canvas, float value, int columnId, int rowId, std::stringstream& ss, int precision = 2);

	Vector2 GetBox();

	void RenderHeader(CanvasWrapper* canvas, int nOfGames, int persistentNOfGames); // Renders header for table
	void RenderData(CanvasWrapper* canvas, int nth, std::string label, STATS_RENDER_ARGUMENTS); // Renders header for table

public:
	std::shared_ptr<int> posX;
	std::shared_ptr<int> posY;
	std::shared_ptr<float> scale;
	std::shared_ptr<LinearColor> colorBackground; // The color used for the background
	std::shared_ptr<LinearColor> colorText; // The color used for texts

	std::shared_ptr<float> rowSize;
	std::shared_ptr<float> columnSize;

	// [STAT_ADD] 3. Declare display bool
	std::shared_ptr<bool> displayBumps;
	std::shared_ptr<bool> displayTeamBumps;
	std::shared_ptr<bool> displayDemos;
	std::shared_ptr<bool> displayBallHits;
	std::shared_ptr<bool> displayBoostUsage;
	std::shared_ptr<bool> displayBoostPMinute;

	std::shared_ptr<bool> displayTotal;
	std::shared_ptr<bool> displayAverage;
	std::shared_ptr<bool> displayPersistentTotal;
	std::shared_ptr<bool> displayPersistentAverage;

	std::shared_ptr<bool> renderHorizontal;
	std::shared_ptr<bool> customDescSize;

	Renderer();

	void RenderStats(CanvasWrapper* canvas, GameStatsSummary& gameStats, PersistentStats& persistentStats);

	void ResetScale();
	void ResetColors();
	void ResetTableSizes();

	// Public utils
	bool ShouldShowTotal();
	bool ShouldShowAverage();
	bool ShouldShowPersistentTotal();
	bool ShouldShowPersistentAverage();
};