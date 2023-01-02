#pragma once

#include <EventData.h>

class GameStats
{
private:
	EventData bumpData;
	EventData demoData;
	EventData ballHitData;

public:
	GameStats();

	void BindEvents();
	void UnbindEvents();


	void OnBump(CarWrapper carWrapper, void* args, std::string eventName);
	void OnDemo(CarWrapper carWrapper, void* args, std::string eventName);
	void OnBallHit(CarWrapper carWrapper, void* args, std::string eventName);

};