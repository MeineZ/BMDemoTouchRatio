#include <chrono>
#include <cstdint>
#include <iostream>
#include <pch.h>

#include "Util.h"
#include "Constants.h"
#include "DemoTouchRatio.h"

bool Util::IsLocalPlayer(CarWrapper& carWrapper)
{
	GameWrapper* gameWrapper = DemoTouchRatio::GameWrapper();
	if (gameWrapper == nullptr)
		return false;

	if (carWrapper.IsNull() || !carWrapper.IsPlayerOwned())
		return false;

	if (gameWrapper->GetLocalCar().IsNull())
		return false;

	unsigned long long local_car_id = gameWrapper->GetLocalCar().GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
	unsigned long long caller_id = carWrapper.GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
	return !(local_car_id == ID_INVALID_VALUE || caller_id == ID_INVALID_VALUE || local_car_id != caller_id);
}

bool Util::CanTrack()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GameWrapper();
	if (gameWrapper == nullptr)
		return false;

	if (gameWrapper->IsInOnlineGame() && !gameWrapper->IsInReplay() && !gameWrapper->IsInFreeplay())
	{
		ServerWrapper serverWrapper = gameWrapper->GetOnlineGame();
		return serverWrapper.IsOnlineMultiplayer();
	}
	return false;
}

uint64_t Util::TimestampInMS()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}