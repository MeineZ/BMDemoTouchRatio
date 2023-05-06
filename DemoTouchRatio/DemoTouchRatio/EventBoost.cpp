#include <pch.h>
#include <EventBoost.h>

#include <Util.h>

EventBoost::EventBoost() :
	lastBoostRegistered(0),
	totalBoostUsed(0),
	totalBoostCollected(0)
{}

EventBoost::EventBoost(float totalBoostUsed, float totalBoostCollected) :
	lastBoostRegistered(0),
	totalBoostUsed(totalBoostUsed),
	totalBoostCollected(totalBoostCollected)
{}

void EventBoost::RegisterBoost(float currentBoostAmount)
{
	// Register last known boost after getting the difference
	float boostDiff = currentBoostAmount - lastBoostRegistered;
	lastBoostRegistered = currentBoostAmount;

	// We don't care about stationary or gained boost
	if (boostDiff >= 0)
	{
		// Boost difference is always positive here
		totalBoostCollected += boostDiff * 100.f;
	}
	else
	{
		// Boost difference is always negative here, so subtraction actually adds it
		totalBoostUsed -= boostDiff * 100.f;
	}
}

float EventBoost::GetTotalBoostUsed() const
{
	return totalBoostUsed;
}

float EventBoost::GetBoostPMinute(float totalTimeInMinutes) const
{
	if (totalTimeInMinutes == 0)
		return 0.f;

	return static_cast<float>(((1.0 / static_cast<double>(totalTimeInMinutes * 60.0)) * static_cast<double>(totalBoostUsed)) * 60.0);
}

float EventBoost::GetTotalBoostCollected() const
{
	return totalBoostCollected;
}

float EventBoost::GetBoostCollectedPMinute(float totalTimeInMinutes) const
{
	if (totalTimeInMinutes == 0)
		return 0.f;

	return static_cast<float>(((1.0 / static_cast<double>(totalTimeInMinutes * 60.0)) * static_cast<double>(totalBoostCollected)) * 60.0);
}