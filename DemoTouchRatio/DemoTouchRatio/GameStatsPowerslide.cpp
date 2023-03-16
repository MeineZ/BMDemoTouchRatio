#include <pch.h>
#include <GameStats.h>

#include <Util.h>

void GameStats::HandlePowerslide(uint64_t deltaTime)
{
	// Do not track powerslide if delta time is over 200ms (5fps). This means there is some kind of gap between the tracking,
	// allbeit insane amount of lagg a goal that has been scored, you've been demo'd. In all these moments, the physics tick method isn't called,
	// which is extremely useful in this case.
	if (!Util::CanTrack() || deltaTime > 200)
		return;

	powerslideData.RegisterPowerslide(deltaTime, inAirData.InAir());
}

int GameStats::GetPowerslideCount() const
{
	return powerslideData.GetPowerslideCount();
}

float GameStats::GetPowerslideTimeInMinutes() const
{
	return powerslideData.GetPowerslideDuration();
}

float GameStats::GetPowerslideTimeInSeconds() const
{
	return powerslideData.GetPowerslideDuration() * 60.f;
}

float GameStats::GetPowerslideTimePerUseInSeconds() const
{
	if (powerslideData.GetPowerslideCount() == 0.f)
		return 0.f;

	return (powerslideData.GetPowerslideDuration() * 60.f) / powerslideData.GetPowerslideCount();
}