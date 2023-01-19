#pragma once

#include <EventData.h>

class GameStats
{
private:
	EventData bumpData;
	EventData teamBumpData;
	EventData demoData;
	EventData ballHitData;

	void OnBump(CarWrapper carWrapper, void* args, std::string eventName);
	void OnDemo(CarWrapper carWrapper, void* args, std::string eventName);
	void OnBallHit(CarWrapper carWrapper, void* args, std::string eventName);

public:
	GameStats();
	GameStats(int bumps, int teamBumps, int demos, int ballHits);

	void BindEvents();
	void UnbindEvents();

	int GetBumps() const;
	int GetTeamBumps() const;
	int GetDemos() const;
	int GetBallHits() const;
};