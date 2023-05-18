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

#include <pch.h>
#include <EventBoost.h>

#include <Util.h>

EventBoost::EventBoost() :
	lastBoostRegistered(0.333f), // You always start the match with 33.3 boost
	totalBoostUsed(0),
	boostCollected(FieldSidesData(0, 0, 0)),
	boostOverfill(FieldSidesData(0, 0, 0))
{}

EventBoost::EventBoost( float totalBoostUsed, FieldSidesData boostCollected, FieldSidesData boostOverfill ) :
	lastBoostRegistered(0),
	totalBoostUsed(totalBoostUsed),
	boostCollected(boostCollected),
	boostOverfill(boostOverfill)
{}

void EventBoost::RegisterBoost(float currentBoostAmount)
{
	// Register last known boost after getting the difference
	float boostDiff = currentBoostAmount - lastBoostRegistered;
	lastBoostRegistered = currentBoostAmount;

	// We don't care about stationary or gained boost
	if( boostDiff >= 0 )
		return;

	// Boost difference is always negative here, so subtraction actually adds it
	totalBoostUsed -= boostDiff * 100.f;
}

void EventBoost::RegisterBoostPickup(BoostPickupWrapper boostPickupWrapper)
{
	if (boostPickupWrapper.IsNull())
		return;

	// This is the way to find out whether this boost collected event comes from the player.
	if (Util::GetLocalBoost() <= lastBoostRegistered)
		return;

	HandleBoostCollected(boostPickupWrapper);

	// Register boost amount to make sure we only handle one boost pick up event per frame.
	RegisterBoost(Util::GetLocalBoost());
}

FieldSide EventBoost::FindBoostPadSide(BoostPickupWrapper& boostPickupWrapper)
{
	// Own <= -2302
	// Opponent >= 2302
	// Mid >= -2302 && <= 2302
	// 
	// 1663 is between the furthest out defensive boost and the furthest out mid boost

	float boostYPos = boostPickupWrapper.GetLocation().Y;
	if (boostYPos <= -1663)
	{
		// Team1 side
		return FieldSide::Blue;
	}
	else if(boostYPos >= 1663)
	{
		// Team2 side
		return FieldSide::Orange;
	}
	else
	{
		// The rest is mid
		return FieldSide::Mid;
	}
}

float EventBoost::FindBoostPadSize(BoostPickupWrapper& boostPickupWrapper)
{
	float boostSize = 0.0f;
	if (boostPickupWrapper.GetBoostType() == '1' || boostPickupWrapper.GetBoostType() == 1)
	{
		// BIG PAD!
		// Boost amount is 9999. So we limit to 100 as the overfill would be insanely big otherwise.
		boostSize = 100.0f;
	}
	else
	{
		// SMOLL PAD! (0.12 boost)
		boostSize = boostPickupWrapper.GetBoostAmount() * 100.0f;
	}
	return boostSize;
}

BoostPadType EventBoost::FindBoostPadType(BoostPickupWrapper& boostPickupWrapper)
{
	if (boostPickupWrapper.GetBoostType() == '1' || boostPickupWrapper.GetBoostType() == 1)
	{
		return BoostPadType::BigPad;
	}
	return BoostPadType::SmallPad;
}

void EventBoost::HandleBoostCollected(BoostPickupWrapper& boostPickupWrapper)
{
	CarWrapper localPlayer = Util::GetLocalPlayer();
	if (localPlayer.IsNull())
		return;

	float lastBoost = (lastBoostRegistered * 100.0f);

	FieldSide fieldSide = FindBoostPadSide(boostPickupWrapper);
	float boostSize = FindBoostPadSize(boostPickupWrapper);
	float boostCollected = lastBoost + boostSize > 100.f ? (100.f - lastBoost) : boostSize;
	float overfill = lastBoost + boostSize > 100.f ? (lastBoost + boostSize - 100.f) : 0.f;
	Team team = (Team)localPlayer.GetTeamNum2();

	DEBUGLOG( "{} -- {} -- {} -- {} -- {} -- {}", (int) team, (int) fieldSide, lastBoost, boostSize, boostCollected, overfill );

	// For now just always update same stats
	UpdateCollectedOverfillStat(fieldSide, team, boostCollected, overfill);

}

void EventBoost::UpdateCollectedOverfillStat(FieldSide side, Team team, float collected, float overfill)
{
	if( ( team == Team::Blue && side == FieldSide::Blue ) || ( team == Team::Orange && side == FieldSide::Orange ) )
	{
		boostCollected.AddOwn(collected);
		boostOverfill.AddOwn(overfill);
	}
	else if( ( team == Team::Blue && side == FieldSide::Orange ) || ( team == Team::Orange && side == FieldSide::Blue ) )
	{
		boostCollected.AddOpponent(collected);
		boostOverfill.AddOpponent(overfill);
	}
	else
	{
		boostCollected.AddNeutral(collected);
		boostOverfill.AddNeutral(overfill);
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

FieldSidesData EventBoost::GetTotalBoostCollected() const
{
	return boostCollected;
}

FieldSidesData EventBoost::GetBoostCollectedPMinute(float totalTimeInMinutes) const
{
	if (totalTimeInMinutes == 0)
		return FieldSidesData(0,0,0);

	return (boostCollected * (1.0 / static_cast<double>(totalTimeInMinutes * 60.0))) * 60.0;
}

FieldSidesData EventBoost::GetTotalBoostOverfill() const
{
	return boostOverfill;
}

FieldSidesData EventBoost::GetBoostOverfillPMinute( float totalTimeInMinutes ) const
{
	if( totalTimeInMinutes == 0 )
		return FieldSidesData(0, 0, 0);

	return (boostOverfill * (1.0 / static_cast<double>(totalTimeInMinutes * 60.0))) * 60.0;
}