#include <chrono>
#include <cstdint>
#include <iostream>
#include <pch.h>

#include "Util.h"
#include "Constants.h"
#include "DemoTouchRatio.h"

bool Util::IsLocalPlayer(CarWrapper& carWrapper)
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return false;

	if (carWrapper.IsNull())
		return false;

	if (gameWrapper->GetLocalCar().IsNull())
		return false;

	return carWrapper.memory_address == gameWrapper->GetLocalCar().memory_address;
}

bool Util::CanTrack()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return false;

	if (gameWrapper->IsInOnlineGame() && !gameWrapper->IsInReplay() && !gameWrapper->IsInFreeplay())
	{
		ServerWrapper serverWrapper = gameWrapper->GetOnlineGame();
		return serverWrapper.IsOnlineMultiplayer();
	}
	return false;
}

int Util::GetCurrentPing()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	CarWrapper localCar = gameWrapper->GetLocalCar();
	if (localCar.IsNull())
		return 0;

	PriWrapper priWrapper = localCar.GetPRI();
	if (priWrapper.IsNull())
		return 0;

	return ((int)priWrapper.GetPing()) * 4;
}

uint64_t Util::TimestampInMS()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

bool Util::AreInSameTeam(CarWrapper carA, CarWrapper carB) {
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return false;

	if (gameWrapper->GetLocalCar().IsNull())
		return false;

	if (carA.IsNull() || carB.IsNull())
		return false;

	/*ServerWrapper serverWrapper = gameWrapper->GetOnlineGame();
	if (serverWrapper.IsNull())
		return false;

	ArrayWrapper<TeamWrapper> teams = serverWrapper.GetTeams();
	if (teams.IsNull())
		return false;*/


	/*unsigned char carATeamNum = 0;
	unsigned char carBTeamNum = 0;

	uintptr_t localCarAddress = gameWrapper->GetLocalCar().memory_address;
	uintptr_t carAAddress = carA.memory_address;

	if (localCarAddress == carAAddress)
	{
		carATeamNum = carA.GetTeamNum2();
		carBTeamNum = carB.GetTeamNum2();
	}
	else
	{
		carATeamNum = carA.GetTeamNum2();
		carBTeamNum = carB.GetTeamNum2();
	}

	bool result = carA.GetTeamNum2() == carB.GetTeamNum2();*/

	/*return result;*/
	return carA.GetTeamNum2() == carB.GetTeamNum2();
}