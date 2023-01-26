#include "pch.h"
#include <GameStats.h>
#include <EventData.h>
#include <DemoTouchRatio.h>

#include "Util.h"
#include "Params.h"

void GameStats::OnBallHit(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	// We care about ball hits from everyone, when A player touches a ball it seems like a global
	// timer is used to delay 2000 continious balltouches.
	int ballHitKey = ballHitData.RegisterEvent(
		DemoTouchRatio::Instance().ShouldMatchAccolades()
		? Util::GetCurrentPing()
		: DemoTouchRatio::Instance().GetCustomBallHitDelay()
	);

	if (!Util::IsLocalPlayer(carWrapper))
		return;

	ballHitData.Bump(ballHitKey);
}

int GameStats::GetBallHits() const {
	return ballHitData.Count();
}

