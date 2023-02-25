#include <pch.h>
#include <EventPowerSlide.h>

#include <Constants.h>
#include <Params.h>
#include <Util.h>
#include <DemoTouchRatio.h>

EventPowerslide::EventPowerslide() :
	count(0),
	minutesInPowerslide(0.f),
	processedPress(false)
{}

EventPowerslide::EventPowerslide(int count, float minutesInAir) :
	count(count),
	minutesInPowerslide(minutesInAir),
	processedPress(false)
{}

void EventPowerslide::RegisterPowerslide(uint64_t deltaTime, bool inAir)
{

	CarWrapper localCar = DemoTouchRatio::GetGameWrapper()->GetLocalCar();
	if (!Util::IsLocalPlayer(localCar))
		return;

	bool handBrakePressed = localCar.GetInput().Handbrake != 0;
	if (!handBrakePressed && processedPress)
	{
		processedPress = false;
	}

	if (inAir)
	{
		processedPress = false;
		return;
	}

	if (handBrakePressed)
	{
		minutesInPowerslide += static_cast<float>((static_cast<double>(deltaTime) / 60000.0));
		if (!processedPress)
		{
			++count;
			processedPress = true;
		}
	}
}

float EventPowerslide::GetPowerslideDuration() const
{
	return minutesInPowerslide;
}

float EventPowerslide::GetPowerslideCount() const
{
	return count;
}