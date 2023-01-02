#include "pch.h"
#include "DemoTouchRatio.h"
#include "Util.h"
#include "Constants.h"
#include "Params.h"

BAKKESMOD_PLUGIN(DemoTouchRatio, "Demo Touch Ratio Plugin", plugin_version, 0)

DemoTouchRatio* DemoTouchRatio::instance_ = nullptr;
std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

//#define CVAR_NAME_DISPLAY "cl_session_plugin_display"

#define HOOK_CAR_BUMPED "Function TAGame.Car_TA.EventBumpedCar"
#define HOOK_CAR_DEMO "Function TAGame.Car_TA.EventDemolished"
#define HOOK_BALL_HIT "Function TAGame.Car_TA.OnHitBall"

void DemoTouchRatio::onLoad()
{
	instance_ = this;
	_globalCvarManager = cvarManager;

	bumpCounter = 0;
	demoCounter = 0;
	ballHitCounter = 0;

	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_BUMPED, [this](CarWrapper carWrapper, void* args, std::string eventName) {
		if (!Util::IsLocalPlayer(carWrapper))
			return;

		DEBUGLOG("CAR BUMPED ({})!", ++bumpCounter);
	});

	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_DEMO, [this](CarWrapper carWrapper, void* args, std::string eventName) {

		ACar_TA_execEventDemolished_Params* castedParams = (ACar_TA_execEventDemolished_Params*)args;
		CarWrapper attacker = CarWrapper(castedParams->Attacker);

		if (!Util::IsLocalPlayer(attacker))
			return;

		DEBUGLOG("CAR DEMO ({})!", ++demoCounter);
	});


	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_BALL_HIT,
		[this](CarWrapper carWrapper, void* params, std::string eventname) {
			if (!Util::IsLocalPlayer(carWrapper))
				return;

			DEBUGLOG("BALL HIT ({})!", ++ballHitCounter);
		});
}

void DemoTouchRatio::onUnload()
{
	gameWrapper->UnregisterDrawables();

	gameWrapper->UnhookEventPost(HOOK_CAR_BUMPED);
	gameWrapper->UnhookEventPost(HOOK_CAR_DEMO);
	gameWrapper->UnhookEventPost(HOOK_BALL_HIT);
}

DemoTouchRatio& DemoTouchRatio::Instance()
{
	return *instance_;
}

GameWrapper& DemoTouchRatio::GameWrapper()
{
	return *(instance_->gameWrapper);
}
