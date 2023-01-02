#include <pch.h>

#include "Util.h"
#include "Constants.h"
#include "DemoTouchRatio.h"

bool Util::IsLocalPlayer(CarWrapper& carWrapper)
{
	if (!carWrapper.IsPlayerOwned())
		return false;

	unsigned long long local_car_id = DemoTouchRatio::GameWrapper().GetLocalCar().GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
	unsigned long long caller_id = carWrapper.GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
	return !(local_car_id == ID_INVALID_VALUE || caller_id == ID_INVALID_VALUE || local_car_id != caller_id);
}