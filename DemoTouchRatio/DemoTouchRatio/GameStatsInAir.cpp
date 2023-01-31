#include <pch.h>
#include <GameStats.h>

#include <Util.h>

void GameStats::HandleInAir(uint64_t deltaTime)
{
	// Do not track boost if delta time is over 200ms (5fps). This means there is some kind of gap between the tracking,
	// allbeit insane amount of lagg a goal that has been scored, you've been demo'd. In all these moments, the physics tick method isn't called,
	// which is extremely useful in this case.
	if (!Util::CanTrack() || deltaTime > 200)
		return;

	inAirData.RegisterAirTime(deltaTime);
}

void GameStats::OnCarWorldHit(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack() || !Util::IsLocalPlayer(carWrapper))
		return;

	inAirData.OnCarHitWorld();
}

float GameStats::GetTimeInAir() const
{
	return inAirData.GetTimeInAir();
}

float GameStats::GetInAirPercentage() const
{
	return inAirData.GetInAirPercentage(totalPlayedTime);
}