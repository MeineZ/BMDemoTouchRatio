#pragma once

class EventStatEvent
{
private:
	int totalShots;
	int totalGoals;
	int totalSaves;

	void IncreaseShots();
	void IncreaseGoals();
	void IncreaseSaves();

public:
	EventStatEvent();
	EventStatEvent(int totalShots, int totalGoals, int totalSaves);

	void RegisterStatTicker(std::string eventName);
	void RegisterStatEvent(std::string eventName);

	int GetShots() const;
	int GetGoals() const;
	int GetSaves() const;
};