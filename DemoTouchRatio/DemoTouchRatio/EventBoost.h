#pragma once

#include "FieldSideEnum.h"
#include "FieldSidesData.h"
#include "BoostEnum.h"
#include "TeamEnum.h"

class EventBoost {
private:
	float lastBoostRegistered;
	float totalBoostUsed;
	FieldSidesData boostCollected;
	FieldSidesData boostOverfill;

private:
	FieldSide FindBoostPadSide(BoostPickupWrapper& boostPickupWrapper);
	float FindBoostPadSize(BoostPickupWrapper& boostPickupWrapper);
	BoostPadType FindBoostPadType(BoostPickupWrapper& boostPickupWrapper);
	void HandleBoostCollected(BoostPickupWrapper &boostPickupWrapper);

	void UpdateCollectedOverfillStat(FieldSide side, Team team, float boostCollected, float boostOverfill);

public:
	EventBoost();
	EventBoost(float totalBoostUsed, FieldSidesData boostCollected, FieldSidesData boostOverfill );

	void RegisterBoost(float currentBoostAmount);
	void RegisterBoostPickup(BoostPickupWrapper boostPickupWrapper);

	float GetTotalBoostUsed() const;
	float GetBoostPMinute(float totalTimeInMinutes) const;
	FieldSidesData GetTotalBoostCollected() const;
	FieldSidesData GetBoostCollectedPMinute(float totalTimeInMinutes) const;
	FieldSidesData GetTotalBoostOverfill() const;
	FieldSidesData GetBoostOverfillPMinute( float totalTimeInMinutes ) const;
};