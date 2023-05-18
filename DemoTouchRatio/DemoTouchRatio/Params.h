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

struct AVehiclePickup_Boost_TA_execPickup_Params
{
	uintptr_t car;
};