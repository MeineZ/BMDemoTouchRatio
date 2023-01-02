#include <pch.h>
#include <GameStats.h>
#include <EventData.h>
#include <DemoTouchRatio.h>

#include "Util.h"
#include "Constants.h"
#include "Params.h"

GameStats::GameStats():
	bumpData(EventData()),
	demoData(EventData()),
	ballHitData(EventData())
{
}

void GameStats::BindEvents()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GameWrapper();
	if (gameWrapper == nullptr)
		return;

	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_BUMPED, std::bind(&GameStats::OnBump, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_DEMO, std::bind(&GameStats::OnDemo, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_BALL_HIT, std::bind(&GameStats::OnBallHit, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void GameStats::UnbindEvents()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GameWrapper();
	if (gameWrapper == nullptr)
		return;

	gameWrapper->UnhookEvent(HOOK_CAR_BUMPED);
	gameWrapper->UnhookEvent(HOOK_CAR_DEMO);
	gameWrapper->UnhookEvent(HOOK_BALL_HIT);
}


void GameStats::OnBump(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	if (!Util::IsLocalPlayer(carWrapper))
		return;

	// We only register bumps from the player
	int bumpKey = bumpData.RegisterEvent();
	bumpData.Bump(bumpKey);

	DEBUGLOG("CAR BUMPED ({})!", bumpData.Count());
}


void GameStats::OnDemo(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	ACar_TA_execEventDemolished_Params* castedParams = (ACar_TA_execEventDemolished_Params*)args;
	CarWrapper attacker = CarWrapper(castedParams->Attacker);

	if (!Util::IsLocalPlayer(attacker))
		return;

	// We only register demos from the player
	int demoKey = bumpData.RegisterEvent();
	demoData.Bump(demoKey);

	DEBUGLOG("CAR DEMO ({})!", demoData.Count());
}


void GameStats::OnBallHit(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	// We care about ball hits from everyone, when A player touches a ball it seems like a global
	// timer is used to delay 2000 continious balltouches.
	int ballHitKey = bumpData.RegisterEvent();

	if (!Util::IsLocalPlayer(carWrapper))
		return;

	ballHitData.Bump(ballHitKey);

	DEBUGLOG("BALL HIT ({})!", ballHitData.Count());
}

