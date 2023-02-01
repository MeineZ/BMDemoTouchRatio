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