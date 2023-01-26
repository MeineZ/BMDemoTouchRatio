#include "pch.h"
#include <GameStats.h>
#include <EventData.h>
#include <DemoTouchRatio.h>

#include "Util.h"
#include "Params.h"

void GameStats::HandleBoostUsage()
{
	if (!Util::CanTrack())
		return;

	CarWrapper car = DemoTouchRatio::GetGameWrapper()->GetLocalCar();
	if (car.IsNull())
		return;

	DEBUGLOG("CHECK BOOST");
	BoostWrapper boostWrapper = car.GetBoostComponent();
	if (boostWrapper.IsNull())
		return;


	DEBUGLOG("CHECK BOOST PROPERTIES");
	float boostAmount = boostWrapper.GetCurrentBoostAmount();
	float lastBoostAmount = boostWrapper.GetLastBoostAmount();
	float activeTime = boostWrapper.GetActiveTime();
	float activityTime = boostWrapper.GetActivityTime();
	float inactiveTime = boostWrapper.GetInactiveTime();
	DEBUGLOG("Some data about the boost component {} ||1|| {} ||2|| {} ||3|| {} ||4|| {}", boostAmount, lastBoostAmount, activeTime, activityTime, inactiveTime);
}

