#include "pch.h"
#include <GameStats.h>
#include <EventData.h>
#include <DemoTouchRatio.h>

#include "Util.h"
#include "Params.h"

void GameStats::OnDemo(CarWrapper carWrapper, void* args, std::string eventName)
{
	if (!Util::CanTrack())
		return;

	ACar_TA_execEventDemolished_Params* castedParams = (ACar_TA_execEventDemolished_Params*)args;
	CarWrapper attacker = CarWrapper(castedParams->Attacker);

	// We only register demos from the player
	if (!Util::IsLocalPlayer(attacker))
		return;

	int demoKey = demoData.RegisterEvent(
		DemoTouchRatio::Instance().ShouldMatchAccolades()
		? 0.f
		: DemoTouchRatio::Instance().GetCustomDemoDelay()
	);
	demoData.Bump(demoKey);
}

int GameStats::GetDemos() const {
	return demoData.Count();
}

