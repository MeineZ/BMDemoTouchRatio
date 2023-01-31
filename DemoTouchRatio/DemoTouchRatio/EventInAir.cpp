#include <pch.h>
#include <EventInAir.h>

#include <Constants.h>
#include <Params.h>
#include <Util.h>
#include <DemoTouchRatio.h>

EventInAir::EventInAir() :
	minutesInAir(0.f),
	aWheelContacts(true),
	carHitsWorld(true)
{}

EventInAir::EventInAir(float timeInMinutesInAir) :
	minutesInAir(timeInMinutesInAir),
	aWheelContacts(true),
	carHitsWorld(true)
{ }

void EventInAir::RegisterAirTime(uint64_t deltaTime)
{
	DEBUGLOG("REGISTERING AIRTIME");
	CarWrapper localCar = DemoTouchRatio::GetGameWrapper()->GetLocalCar();
	if (!Util::IsLocalPlayer(localCar))
		return;

	DEBUGLOG("FOUND LOCALCAR: {}", localCar.GetNumWheelContacts());
	DEBUGLOG("carHitsWorld: {}", carHitsWorld);
	if(localCar.GetNumWheelContacts() > 0 || carHitsWorld)
	{
		// There's no method that triggers on 'car doesnt hit world anymore' so we reset the boolean for everytime the game registers a car-is-not-in-the-air-frame
		carHitsWorld = false;
		return;
	}

	minutesInAir += static_cast<float>((static_cast<double>(deltaTime) / 60000.0));
}

void EventInAir::OnCarHitWorld()
{
	carHitsWorld = true;
}

float EventInAir::GetInAirPercentage(float totalTimeInMinutes) const
{
	return (minutesInAir / totalTimeInMinutes) * 100.f;
}

float EventInAir::GetTimeInAir() const
{
	return minutesInAir;
}