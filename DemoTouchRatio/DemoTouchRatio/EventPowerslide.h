#pragma once

class EventPowerslide
{
private:
	int count;
	float minutesInPowerslide;
	bool processedPress;

public:
	EventPowerslide();
	EventPowerslide(int count, float duration);

	void RegisterPowerslide(uint64_t deltaTime, bool inAir);

	float GetPowerslideDuration() const;
	float GetPowerslideCount() const;
};