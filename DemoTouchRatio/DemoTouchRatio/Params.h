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

struct ACar_TA_execEventBumpedCar_Params 
{
	uintptr_t Car;
	uintptr_t HitCar;
	Vector HitLocation;
};

struct ACar_TA_execBumpCar_Params
{
	float arg1;
	uintptr_t bumpedCar;
};

struct AVehicle_TA_execNumWheelWorldContacts_Params
{
	int32_t ReturnValue;
};