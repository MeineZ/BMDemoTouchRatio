#include "pch.h"
#include "DemoTouchRatio.h"
#include "Constants.h"

struct ACar_TA_execEventDemolished_Params
{
	uintptr_t Attacker; // ACar_TA
	uintptr_t Victim; // ARBActor_TA
	uintptr_t VictimData; // ACar_TA
	Vector AttackerVelocityData;
	Vector VictimVelocityData;
};

BAKKESMOD_PLUGIN(DemoTouchRatio, "Demo Touch Ratio Plugin", plugin_version, 0)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

//#define CVAR_NAME_DISPLAY "cl_session_plugin_display"

#define HOOK_CAR_BUMPED "Function TAGame.Car_TA.EventBumpedCar"
#define HOOK_CAR_Q_DEMO "Function TAGame.Car_TA.QueueDemolish"
#define HOOK_CAR_DEMO "Function TAGame.Car_TA.EventDemolished"

void DemoTouchRatio::onLoad()
{
	bumpCounter = 0;
	demoCounter = 0;
	ballHitCounter = 0;

	_globalCvarManager = cvarManager;

	DEBUGLOG("INITIALIZED PLUGIN!");

	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_BUMPED, [this](CarWrapper carWrapper, void* args, std::string eventName) {
		if (!carWrapper.IsPlayerOwned())
			return;

		unsigned long long local_car_id = gameWrapper->GetLocalCar().GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
		unsigned long long caller_id = carWrapper.GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
		if (local_car_id == ID_INVALID_VALUE || caller_id == ID_INVALID_VALUE || local_car_id != caller_id)
			return;

		DEBUGLOG("CAR BUMPED ({})!", ++bumpCounter);
	});

	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_DEMO, [this](CarWrapper carWrapper, void* args, std::string eventName) {

		ACar_TA_execEventDemolished_Params* castedParams = (ACar_TA_execEventDemolished_Params*)args;
		CarWrapper attacker = CarWrapper(castedParams->Attacker);

		if (!attacker.IsPlayerOwned())
			return;

		unsigned long long local_car_id = gameWrapper->GetLocalCar().GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
		unsigned long long caller_id = attacker.GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
		if (local_car_id == ID_INVALID_VALUE || caller_id == ID_INVALID_VALUE || local_car_id != caller_id)
			return;

		DEBUGLOG("CAR DEMO ({})!", ++demoCounter);
	});


	gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.OnHitBall",
		[this](CarWrapper carWrapper, void* params, std::string eventname) {
			if (!carWrapper.IsPlayerOwned())
				return;

			unsigned long long local_car_id = gameWrapper->GetLocalCar().GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
			unsigned long long caller_id = carWrapper.GetPlayerController().GetPRI().GetUniqueIdWrapper().GetUID();
			if (local_car_id == ID_INVALID_VALUE || caller_id == ID_INVALID_VALUE || local_car_id != caller_id)
				return;

			DEBUGLOG("BALL HIT ({})!", ++ballHitCounter);
		});
}
