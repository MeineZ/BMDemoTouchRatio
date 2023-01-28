#include <pch.h>
#include <EventBoost.h>

#include <Util.h>

EventBoost::EventBoost() :
	lastBoostRegistered(0),
	totalBoostUsed(0)
{}

EventBoost::EventBoost(float totalBoostUsed) :
	lastBoostRegistered(0),
	totalBoostUsed(totalBoostUsed)
{}

void EventBoost::RegisterBoost(float currentBoostAmount)
{
	// Register last known boost after getting the difference
	float boostDiff = currentBoostAmount - lastBoostRegistered;
	lastBoostRegistered = currentBoostAmount;

	// We don't care about stationary or gained boost
	if (boostDiff >= 0)
		return;

	// Boost difference is always negative here, so subtraction actually adds it
	totalBoostUsed -= boostDiff * 100.f;
}

float EventBoost::GetTotalBoostUsed() const
{
	return totalBoostUsed;
}

float EventBoost::GetBoostPMinute(float totalTime) const
{
	if (totalTime == 0)
		return 0.f;

	return ((1.0f / totalTime) * totalBoostUsed) * 60.f;
}