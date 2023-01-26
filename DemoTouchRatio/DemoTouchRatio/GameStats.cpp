#include "pch.h"
#include <GameStats.h>
#include <EventData.h>
#include <DemoTouchRatio.h>

#include "Util.h"
#include "Constants.h"
#include "Params.h"

GameStats::GameStats():
	bumpData(EventData()),
	teamBumpData(EventData()),
	demoData(EventData()),
	ballHitData(EventData())
{ }

GameStats::GameStats(int bumps, int teamBumps, int demos, int ballHits) :
	bumpData(EventData(bumps)),
	teamBumpData(EventData(teamBumps)),
	demoData(EventData(demos)),
	ballHitData(EventData(ballHits))
{ }

void GameStats::BindEvents()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return;

	gameWrapper->HookEvent(HOOK_SET_VEHICLE_INPUT, std::bind(&GameStats::OnPhysicsTick, this, std::placeholders::_1));
	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_BUMPED, std::bind(&GameStats::OnBump, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_DEMO, std::bind(&GameStats::OnDemo, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_BALL_HIT, std::bind(&GameStats::OnBallHit, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void GameStats::UnbindEvents()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return;

	gameWrapper->UnhookEvent(HOOK_CAR_BUMPED);
	gameWrapper->UnhookEvent(HOOK_CAR_DEMO);
	gameWrapper->UnhookEvent(HOOK_BALL_HIT);
}

void GameStats::OnPhysicsTick(std::string eventName)
{
	HandleBoostUsage();
}
