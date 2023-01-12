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

	ACar_TA_execBumpCar_Params* castedParams = (ACar_TA_execBumpCar_Params*)args;
	CarWrapper bumpedCar = CarWrapper(castedParams->bumpedCar);

	// We only register bumps from the player
	if (!Util::IsLocalPlayer(carWrapper) && !Util::IsLocalPlayer(bumpedCar))
		return;

	// Check whether we're allowed to track it and if it's a team bump
	bool canTrackTeamBumps = DemoTouchRatio::Instance().CanTrackTeamBumps();
	bool sameTeam = Util::AreInSameTeam(carWrapper, bumpedCar);
	if (canTrackTeamBumps && sameTeam) {
		int teamBumpKey = teamBumpData.RegisterEvent(Util::GetCurrentPing());
		teamBumpData.Bump(teamBumpKey);
		return;
	}
	// Return if it was a team bump but we're not allowed to track it
	else if(!canTrackTeamBumps && sameTeam)
	{
		return;
	}

	int bumpKey = bumpData.RegisterEvent(Util::GetCurrentPing());
	bumpData.Bump(bumpKey);
}

void GameStats::OnDemo(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	ACar_TA_execEventDemolished_Params* castedParams = (ACar_TA_execEventDemolished_Params*)args;
	CarWrapper attacker = CarWrapper(castedParams->Attacker);

	// We only register demos from the player
	if (!Util::IsLocalPlayer(attacker))
		return;

	int demoKey = demoData.RegisterEvent(0);
	demoData.Bump(demoKey);
}

void GameStats::OnBallHit(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	// We care about ball hits from everyone, when A player touches a ball it seems like a global
	// timer is used to delay 2000 continious balltouches.
	int ballHitKey = ballHitData.RegisterEvent(Util::GetCurrentPing());

	if (!Util::IsLocalPlayer(carWrapper))
		return;

	ballHitData.Bump(ballHitKey);
}

int GameStats::GetBumps() {
	return bumpData.Count();
}
int GameStats::GetTeamBumps() {
	return teamBumpData.Count();
}
int GameStats::GetDemos() {
	return demoData.Count();
}
int GameStats::GetBallHits() {
	return ballHitData.Count();
}

