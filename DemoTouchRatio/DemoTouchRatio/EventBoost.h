/*
 * Copyright 2023 MeineZ@GitHub

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

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