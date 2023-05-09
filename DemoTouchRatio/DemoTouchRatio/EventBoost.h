#pragma once

class EventBoost {
private:
	float lastBoostRegistered;
	float totalBoostUsed;
	float totalBoostCollected;
	float totalOverfill;

public:
	EventBoost();
	EventBoost(float totalBoostUsed, float totalBoostCollected, float totalBoostOverfill);

	void RegisterBoost(float currentBoostAmount);
	void RegisterBoostPickup(BoostPickupWrapper boostPickupWrapper);

	float GetTotalBoostUsed() const;
	float GetBoostPMinute(float totalTimeInMinutes) const;
	float GetTotalBoostCollected() const;
	float GetBoostCollectedPMinute(float totalTimeInMinutes) const;
	float GetTotalBoostOverfill() const;
	float GetBoostOverfillPMinute( float totalTimeInMinutes ) const;
};