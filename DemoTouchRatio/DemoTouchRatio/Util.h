#pragma once

static class Util
{
public:
	static bool IsLocalPlayer(CarWrapper& carWrapper);

	static bool CanTrack();

	static uint64_t TimestampInMS();
};