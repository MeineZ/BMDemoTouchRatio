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

	void BindEvents();
	void UnbindEvents();

	int GetBumps();
	int GetTeamBumps();
	int GetDemos();
	int GetBallHits();
};