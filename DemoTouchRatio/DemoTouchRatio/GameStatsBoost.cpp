#include "pch.h"
#include <GameStats.h>
#include <EventData.h>
#include <DemoTouchRatio.h>

#include "Util.h"
#include "Params.h"

void GameStats::HandleBoost(uint64_t deltaTime)
{
	// Do not track boost if delta time is over 200ms (5fps). This means there is some kind of gap between the tracking,
	// allbeit insane amount of lagg a goal that has been scored, you've been demo'd. In all these moments, the physics tick method isn't called,
	// which is extremely useful in this case.
	if (!Util::CanTrack() || deltaTime > 200)
		return;

	CarWrapper car = DemoTouchRatio::GetGameWrapper()->GetLocalCar();
	if (car.IsNull())
		return;

	BoostWrapper boostWrapper = car.GetBoostComponent();
	if (boostWrapper.IsNull())
		return;

	boostData.RegisterBoost(boostWrapper.GetCurrentBoostAmount());
}

float GameStats::GetBoostUsed() const
{
	return boostData.GetTotalBoostUsed();
}

float GameStats::GetBoostPMinute() const
{
	return boostData.GetBoostPMinute(totalPlayedTime);
}