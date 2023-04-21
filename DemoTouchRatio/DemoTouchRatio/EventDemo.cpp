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