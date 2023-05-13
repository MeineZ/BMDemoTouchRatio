#pragma once

#include "PlaylistEnum.h"

static class Util
{
public:
	static CarWrapper GetLocalPlayer();

	static bool IsLocalPlayer(CarWrapper& carWrapper);

	static float GetLocalBoost();

	static bool CanTrack();

	static int GetCurrentPing();

	static uint64_t TimestampInMS();

	static bool AreInSameTeam(CarWrapper carA, CarWrapper carB);

	static PlaylistType CurrentPlaylist();
};