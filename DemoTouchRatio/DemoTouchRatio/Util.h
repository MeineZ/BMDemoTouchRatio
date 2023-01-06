#pragma once

static class Util
{
public:
	static bool IsLocalPlayer(CarWrapper& carWrapper);

	static bool CanTrack();

	static int GetCurrentPing();

	static uint64_t TimestampInMS();

	static bool AreInSameTeam(CarWrapper carA, CarWrapper carB);
};