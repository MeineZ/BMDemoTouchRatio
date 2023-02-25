#pragma once

#include <EventData.h>
#include <EventBoost.h>
#include <EventInAir.h>
#include <EventPowerslide.h>

class GameStats
{
private:

	uint64_t lastTimeStamp;
	float totalPlayedTime;

	// [STAT_ADD] 5. Add data tracking property
	EventData bumpData;
	EventData teamBumpData;
	EventData demoData;
	EventData ballHitData;
	EventBoost boostData;
	EventInAir inAirData;
	EventPowerslide powerslideData;

	// [STAT_ADD] 6. Add data tracking method
	void OnBump(CarWrapper carWrapper, void* args, std::string eventName);
	void OnDemo(CarWrapper carWrapper, void* args, std::string eventName);
	void OnBallHit(CarWrapper carWrapper, void* args, std::string eventName);
	void OnCarWorldHit(CarWrapper carWrapper, void* args, std::string eventName);

	void OnPhysicsTick(std::string eventName);
	void HandleBoost(uint64_t deltaTime);
	void HandleInAir(uint64_t deltaTime);
	void HandlePowerslide(uint64_t deltaTime);

public:
	GameStats();
	const GameStats(int bumps, int teamBumps, int demos, int ballHits, float totalTime, float totalBoost, float airTimeInMinutes, int powerslideCount, float powerslideDuration);

	void BindEvents();
	void UnbindEvents();

	float GetTotalPlayedTime() const;

	// [STAT_ADD] 7. Add data getter
	int GetBumps() const;
	int GetTeamBumps() const;
	int GetDemos() const;
	int GetBallHits() const;
	float GetBoostUsed() const;
	float GetBoostPMinute() const;
	float GetTimeInAir() const;
	float GetInAirPercentage() const;
	int GetPowerslideCount() const;
	float GetPowerslideTimeInMinutes() const;
	float GetPowerslideTimeInSeconds() const;
};