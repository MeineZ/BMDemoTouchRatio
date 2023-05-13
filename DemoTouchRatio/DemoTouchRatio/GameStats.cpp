#include "pch.h"
#include <GameStats.h>
#include <EventData.h>
#include <DemoTouchRatio.h>

#include "Util.h"
#include "Constants.h"
#include "Params.h"

GameStats::GameStats():
	lastTimeStamp(0),
	totalPlayedTime(0),
	bumpData(EventData()),
	teamBumpData(EventData()),
	ballHitData(EventData()),
	// [STAT_ADD] 8. Add constructor
	boostData(EventBoost()),
	inAirData(EventInAir()),
	powerslideData(EventPowerslide()),
	statEventData(EventStatEvent())
{ }

GameStats::GameStats(int bumps, int teamBumps, int demos, int ballHits, float totalTime, float totalBoost, float airTimeInMinutes, int powerslideCount, float powerslideDuration, int shots, int goals, int saves, int teamDemos, int deaths, int assists, FieldSidesData boostCollected, FieldSidesData boostOverfill) :
	lastTimeStamp(0),
	totalPlayedTime(totalTime),
	bumpData(EventData(bumps)),
	teamBumpData(EventData(teamBumps)),
	ballHitData(EventData(ballHits)),
	// [STAT_ADD] 9. Add constructor with params
	boostData(EventBoost(totalBoost, boostCollected, boostOverfill)),
	inAirData(EventInAir(airTimeInMinutes)),
	powerslideData(EventPowerslide(powerslideCount, powerslideDuration)),
	statEventData(EventStatEvent(demos, teamDemos, deaths, shots, goals, saves, assists))
{ }

void GameStats::BindEvents()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return;

	// [STAT_ADD] 10. Bind function or add to physics tick
	gameWrapper->HookEvent(HOOK_SET_VEHICLE_INPUT, std::bind(&GameStats::OnPhysicsTick, this, std::placeholders::_1));
	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_BUMPED, std::bind(&GameStats::OnBump, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_BALL_HIT, std::bind(&GameStats::OnBallHit, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEventWithCaller<CarWrapper>(HOOK_CAR_HIT_WORLD, std::bind(&GameStats::OnCarWorldHit, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	gameWrapper->HookEventWithCaller<ServerWrapper>(HOOK_STAT_TICKER_MESSAGE, std::bind(&GameStats::OnStatTicker, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEventWithCaller<ServerWrapper>(HOOK_HANDLE_STAT_EVENT, std::bind(&GameStats::OnStatEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	gameWrapper->HookEventWithCaller<ActorWrapper>(HOOK_EVENT_ON_PICK_UP, std::bind(&GameStats::OnEventPickedUp, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void GameStats::UnbindEvents()
{
	GameWrapper* gameWrapper = DemoTouchRatio::GetGameWrapper();
	if (gameWrapper == nullptr)
		return;

	gameWrapper->UnhookEvent(HOOK_SET_VEHICLE_INPUT);
	gameWrapper->UnhookEvent(HOOK_CAR_BUMPED);
	gameWrapper->UnhookEvent(HOOK_CAR_DEMO);
	gameWrapper->UnhookEvent(HOOK_BALL_HIT);
	gameWrapper->UnhookEvent(HOOK_CAR_HIT_WORLD);
	gameWrapper->UnhookEvent(HOOK_STAT_TICKER_MESSAGE);
	gameWrapper->UnhookEvent(HOOK_HANDLE_STAT_EVENT);
	gameWrapper->UnhookEvent(HOOK_EVENT_ON_PICK_UP);
}

void GameStats::OnPhysicsTick(std::string eventName)
{
	uint64_t dt = 0;
	if (lastTimeStamp == 0)
	{
		lastTimeStamp = Util::TimestampInMS();
		dt = 1;
	}
	else
	{
		dt = Util::TimestampInMS() - lastTimeStamp;
		lastTimeStamp = Util::TimestampInMS();
	}

	// Limit the delta time to 200ms. This may cause inaccurate results, 
	// but this number equals 5 fps. If the player gets above 10fps you have
	// a shit pc and we don't care about that here.
	totalPlayedTime += (static_cast<float>(static_cast<double>(dt > 200 ? 200 : dt) / 60000.0));

	HandleBoost(dt);
	HandleInAir(dt);
	HandlePowerslide(dt);
}

float GameStats::GetTotalPlayedTime() const
{
	return totalPlayedTime;
}