#include <pch.h>
#include <EventBoost.h>

#include <Util.h>

EventBoost::EventBoost() :
	lastBoostRegistered(0.333f), // You always start the match with 33.3 boost
	totalBoostUsed(0),
	totalBoostCollected(0),
	totalOverfill(0)
{}

EventBoost::EventBoost(float totalBoostUsed, float totalBoostCollected, float totalBoostOverfill) :
	lastBoostRegistered(0),
	totalBoostUsed(totalBoostUsed),
	totalBoostCollected(totalBoostCollected),
	totalOverfill(totalBoostOverfill)
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

void EventBoost::RegisterBoostPickup(BoostPickupWrapper boostPickupWrapper)
{
	if (boostPickupWrapper.IsNull())
		return;

	// This is the way to find out whether this boost collected event comes from the player.
	if (Util::GetLocalBoost() <= lastBoostRegistered)
		return;

	float boostSize = 0.0f;
	if (boostPickupWrapper.GetBoostType() == '1' || boostPickupWrapper.GetBoostType() == 1)
	{
		// BIG PAD!
		// Boost amount is 9999. So we limit to 100 as the overfill would be insanely big otherwise.
		boostSize = 100.0f;
	}
	else 
	{
		// SMOLL PAD!
		boostSize = boostPickupWrapper.GetBoostAmount() * 100.0f;
	}

	float overfill = (lastBoostRegistered * 100.0f) + boostSize - 100.f;
	if (overfill < 0)
		return; // no overfill

	// Boost overfill is always negative here, so subtraction actually adds it
	totalOverfill += overfill;

	// Register boost amount to make sure we only handle one boost pick up event per frame.
	RegisterBoost(Util::GetLocalBoost());
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

float EventBoost::GetTotalBoostOverfill() const
{
	return totalOverfill;
}

float EventBoost::GetBoostOverfillPMinute( float totalTimeInMinutes ) const
{
	if( totalTimeInMinutes == 0 )
		return 0.f;

	return static_cast<float>( ( ( 1.0 / static_cast<double>( totalTimeInMinutes * 60.0 ) ) * static_cast<double>(totalOverfill) ) * 60.0 );
}