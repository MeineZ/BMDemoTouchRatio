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
#include <EventStatEvent.h>

#include "Util.h"

void EventStatEvent::HandleDemolition(CarWrapper attacker, PriWrapper victim)
{
	if (attacker.IsNull() || victim.IsNull())
		return;

	CarWrapper victimCar = victim.GetCar();
	if (victimCar.IsNull())
		return;

	if (Util::IsLocalPlayer(attacker))
	{
		if (Util::AreInSameTeam(attacker, victimCar))
		{
			// process team demo
			++teamDemos;
		}
		else
		{
			// process normal demo
			++demos;
		}
	}
	else
	{
		if (Util::IsLocalPlayer(victimCar))
		{
			// Process death
			++deaths;
		}
	}
}

int EventStatEvent::GetDemos() const
{
	return demos;
}

int EventStatEvent::GetTeamDemos() const
{
	return teamDemos;
}

int EventStatEvent::GetDeaths() const
{
	return deaths;
}