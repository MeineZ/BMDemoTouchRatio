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

#include <pch.h>
#include <PlaylistStats.h>

#include <Util.h>
#include <DemoTouchRatio.h>

PlaylistStats::PlaylistStats(PlaylistType playlistType) :
	playlistToTrack(playlistType),
	persistentStats(),
	currentGame(nullptr),
	lastGame(nullptr),
	playedGames(std::vector<GameStats*>())
{ }

void PlaylistStats::Initialize()
{
	InitializePersistentStats();
}

void PlaylistStats::InitializePersistentStats()
{
	persistentStats.Initialize(PlaylistHelpers::GetPlaylistFileSuffix(playlistToTrack));
}

void PlaylistStats::Reset()
{
	if (lastGame != nullptr)
	{
		delete lastGame;
		lastGame = nullptr;
	}

	for (int i = 0; i < playedGames.size(); ++i)
	{
		delete playedGames[i];
	}
	playedGames.clear();
}

void PlaylistStats::CreateNewGame()
{
	if (!IsCurrentPlaylist())
		return;

	if (currentGame == nullptr)
	{
		currentGame = new GameStats();
		currentGame->BindEvents();
	}
}

void PlaylistStats::EndGame()
{
	if (currentGame != nullptr)
	{
		if (lastGame != nullptr)
		{
			playedGames.push_back(lastGame);
			lastGame = nullptr;
		}

		if (DemoTouchRatio::Instance().UsesPersistentStats())
		{
			persistentStats.Update(currentGame);
		}

		lastGame = currentGame;
		lastGame->UnbindEvents();
		currentGame = nullptr;
	}
}

bool PlaylistStats::IsCurrentPlaylist()
{
	return DemoTouchRatio::Instance().GetCurrentShownPlaylist() == playlistToTrack;
}

GameStats* PlaylistStats::GetCurrentGame()
{
	return currentGame;
}

GameStats* PlaylistStats::GetLastGame()
{
	return lastGame;
}

std::vector<GameStats*>& PlaylistStats::GetPlayedGames()
{
	return playedGames;
}

PersistentStats& PlaylistStats::GetPersistentStats()
{
	return persistentStats;
}

bool PlaylistStats::IsCurrent(PlaylistType currentPlaylist)
{
	return PlaylistHelpers::ConvertToPluginPlaylist(currentPlaylist) == playlistToTrack;
}
