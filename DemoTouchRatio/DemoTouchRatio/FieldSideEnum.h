#pragma once

enum class FieldSide
{ 
	// y <= -1663
	Team1 = 0,
	Blue = 0,
	// y >= 1663
	Team2 = 1,
	Orange = 1,
	// y > -1663 && y < 1663
	Mid = 2
};