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

struct AGFxHUD_TA_execStatTicker_Params
{
	uintptr_t Receiver;
	uintptr_t Victim;
	uintptr_t StatEvent;
};

struct AGFxHUD_TA_execStatEvent_Params
{
	uintptr_t PRI;
	uintptr_t StatEvent;
	// Count always is int_max. No idea why
	uintptr_t Count;
};