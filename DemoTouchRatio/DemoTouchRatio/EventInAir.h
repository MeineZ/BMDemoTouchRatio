#pragma once

class EventInAir
{
private:
	float minutesInAir;
	bool aWheelContacts;
	bool carHitsWorld;

public:
	EventInAir();
	EventInAir(float timeInMinutesInAir);

	void OnCarHitWorld();
	void RegisterAirTime(uint64_t deltaTime);

	float GetInAirPercentage(float totalTimeInMinutes) const;
	float GetTimeInAir() const;
};