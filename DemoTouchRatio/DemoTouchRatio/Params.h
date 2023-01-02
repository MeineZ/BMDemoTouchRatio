#pragma once

#include "pch.h"

struct ACar_TA_execEventDemolished_Params
{
	uintptr_t Attacker; // ACar_TA
	uintptr_t Victim; // ARBActor_TA
	uintptr_t VictimData; // ACar_TA
	Vector AttackerVelocityData;
	Vector VictimVelocityData;
};