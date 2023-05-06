#pragma once

class EventBoost {
private:
	float lastBoostRegistered;
	float totalBoostUsed;
	float totalBoostCollected;

public:
	EventBoost();
	EventBoost(float totalBoostUsed ,float totalBoostCollected);

	void RegisterBoost(float currentBoostAmount);

	float GetTotalBoostUsed() const;
	float GetBoostPMinute(float totalTimeInMinutes) const;
	float GetTotalBoostCollected() const;
	float GetBoostCollectedPMinute(float totalTimeInMinutes) const;
};