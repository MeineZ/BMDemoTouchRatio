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

#include <string>

#include <GameStats.h>
#include <GameStatsSummary.h>

class PersistentStats {
private:
	std::string suffix;

	GameStats total;
	GameStatsSummary::SummarizedStats average;
	int nOfGames;

	std::filesystem::path GetFullPath();
	void LoadFile();
	bool UpdateFile();

	void UpdateAverageStats();

public:
	PersistentStats();

	void Initialize(std::string fileSuffix);
	const GameStats& Update(GameStats* stats);
	void Clear();

	void Add(PersistentStats& other);

	const GameStats& GetTotalStats() const;
	const GameStatsSummary::SummarizedStats& GetAverageStats() const;
	int GetNumberOfGames() const;

};