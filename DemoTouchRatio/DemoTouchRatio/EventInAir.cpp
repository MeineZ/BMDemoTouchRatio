#include <pch.h>
#include <EventInAir.h>

#include <Constants.h>
#include <Params.h>
#include <Util.h>
#include <DemoTouchRatio.h>

EventInAir::EventInAir() :
	minutesInAir(0.f),
	aWheelContacts(true),
	carHitsWorld(true),
	lastGroundTimestamp(0)
{
	lastGroundTimestamp = Util::TimestampInMS();
}

EventInAir::EventInAir(float timeInMinutesInAir) :
	minutesInAir(timeInMinutesInAir),
	aWheelContacts(true),
	carHitsWorld(true),
	lastGroundTimestamp(0)
{}

void EventInAir::RegisterAirTime(uint64_t deltaTime)
{
	CarWrapper localCar = DemoTouchRatio::GetGameWrapper()->GetLocalCar();
	if (!Util::IsLocalPlayer(localCar))
		return;

	if(localCar.GetNumWheelContacts() > 0 || carHitsWorld)
	{
		// There's no method that triggers on 'car doesnt hit world anymore' so we reset the boolean for everytime the game registers a car-is-not-in-the-air-frame
		carHitsWorld = false;

		lastGroundTimestamp = Util::TimestampInMS();
		return;
	}

	float delayInMS = DemoTouchRatio::Instance().GetCustomInAirDelay();
	// Prevent tracking until delay has been taken care of
	if (delayInMS != 0.f && lastGroundTimestamp + static_cast<uint64_t>(delayInMS) > Util::TimestampInMS())
		return;

	minutesInAir += static_cast<float>((static_cast<double>(deltaTime) / 60000.0));
}

void EventInAir::OnCarHitWorld()
{
	carHitsWorld = true;
}

float EventInAir::GetInAirPercentage(float totalTimeInMinutes) const
{
	if (totalTimeInMinutes == 0.f)
		return 0.f;

	return (minutesInAir / totalTimeInMinutes) * 100.f;
}

float EventInAir::GetTimeInAir() const
{
	return minutesInAir;
}

bool EventInAir::InAir() const
{
	CarWrapper localCar = DemoTouchRatio::GetGameWrapper()->GetLocalCar();
	if (!Util::IsLocalPlayer(localCar))
		return false;

	return !(localCar.GetNumWheelContacts() > 0 || carHitsWorld);
}