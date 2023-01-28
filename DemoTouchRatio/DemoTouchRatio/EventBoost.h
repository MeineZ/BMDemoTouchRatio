#pragma once

class EventBoost {
private:
	float lastBoostRegistered;
	float totalBoostUsed;

public:
	EventBoost();
	EventBoost(float totalBoostUsed);

	void RegisterBoost(float currentBoostAmount);

	float GetTotalBoostUsed() const;
	float GetBoostPMinute(float totalTimeInMinutes) const;
};