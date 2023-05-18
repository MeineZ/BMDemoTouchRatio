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

#include <PlaylistEnum.h>
#include "PersistentStats.h"

class GameStats;

class PlaylistStats
{
private:
	PlaylistType playlistToTrack;

	PersistentStats persistentStats;

	std::vector<GameStats*> playedGames;
	GameStats* lastGame;
	GameStats* currentGame;

	bool IsCurrentPlaylist();

public:
	PlaylistStats(PlaylistType playlistToTrack);

	void Initialize();
	void InitializePersistentStats();
	void Reset();

	void EndGame();
	void CreateNewGame();

	bool IsCurrent(PlaylistType currentPlaylist);
	GameStats* GetCurrentGame();
	GameStats* GetLastGame();
	std::vector<GameStats*>& GetPlayedGames();
	PersistentStats& GetPersistentStats();
};