#pragma once

class EventStatEvent
{
private:
	int demos;
	int teamDemos;
	int deaths;
	int totalShots;
	int totalGoals;
	int totalSaves;

	void HandleShots();
	void HandleGoals();
	void HandleSaves();

	void HandleDemolition(CarWrapper attacker, PriWrapper victim);

public:
	EventStatEvent();
	EventStatEvent(int demos, int teamDemos, int deaths, int totalShots, int totalGoals, int totalSaves);

	void RegisterStatTicker(std::string eventName, CarWrapper receiver, PriWrapper victim);
	void RegisterStatEvent(std::string eventName);

	int GetShots() const;
	int GetGoals() const;
	int GetSaves() const;
	int GetDemos() const;
	int GetTeamDemos() const;
	int GetDeaths() const;
};