/*
 * Copyright 2023 MeineZ@GitHub

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

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
	int totalAssists;

	void HandleShots();
	void HandleGoals();
	void HandleSaves();
	void HandleAssists();

	void HandleDemolition(CarWrapper attacker, PriWrapper victim);

public:
	EventStatEvent();
	EventStatEvent(int demos, int teamDemos, int deaths, int totalShots, int totalGoals, int totalSaves, int totalAssists);

	void RegisterStatTicker(std::string eventName, CarWrapper receiver, PriWrapper victim);
	void RegisterStatEvent(std::string eventName);

	int GetShots() const;
	int GetGoals() const;
	int GetSaves() const;
	int GetAssists() const;
	int GetDemos() const;
	int GetTeamDemos() const;
	int GetDeaths() const;
};