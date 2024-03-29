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

// All supported types (types that are consider worth tracking).
enum class PlaylistType
{
	PLAYLIST_UNKNOWN = -1,
	PLAYLIST_DUEL = 1,
	PLAYLIST_DOUBLES = 2,
	PLAYLIST_STANDARD = 3,
	PLAYLIST_CHAOS = 4,
	PLAYLIST_PRIVATEMATCH = 6,
	PLAYLIST_EXHIBITION = 8,
	PLAYLIST_RANKEDDUEL = 10,
	PLAYLIST_RANKEDDOUBLES = 11,
	PLAYLIST_RANKEDSOLOSTANDARD = 12,
	PLAYLIST_RANKEDSTANDARD = 13,
	PLAYLIST_RANKEDHOOPS = 27,
	PLAYLIST_RANKEDRUMBLE = 28,
	PLAYLIST_RANKEDDROPSHOT = 29,
	PLAYLIST_RANKEDSNOWDAY = 30,

	PLAYLIST_CASUAL = 998 // Used to store the casual playlist at one place (unofficial playlist)
};

static class PlaylistHelpers
{
public:
	inline static PlaylistType GetPlaylistFromInt(int playlistType);
	inline static PlaylistType ConvertToPluginPlaylist(PlaylistType playlistType);

	inline static std::string GetPlaylistName(PlaylistType playlist);
	inline static std::string GetPlaylistFileSuffix(PlaylistType playlist);

	inline static PlaylistType GetNextPlaylist(PlaylistType currentPlaylist);
	inline static PlaylistType GetPrevPlaylist(PlaylistType currentPlaylist);

};

inline PlaylistType PlaylistHelpers::GetPlaylistFromInt(int playlistType)
{
	switch (playlistType)
	{
		case int(PlaylistType::PLAYLIST_EXHIBITION) : return PlaylistType::PLAYLIST_EXHIBITION;
		case int(PlaylistType::PLAYLIST_PRIVATEMATCH) : return PlaylistType::PLAYLIST_PRIVATEMATCH;
		case int(PlaylistType::PLAYLIST_DUEL) : return PlaylistType::PLAYLIST_DUEL;
		case int(PlaylistType::PLAYLIST_DOUBLES) : return PlaylistType::PLAYLIST_DOUBLES;
		case int(PlaylistType::PLAYLIST_STANDARD) : return PlaylistType::PLAYLIST_STANDARD;
		case int(PlaylistType::PLAYLIST_CHAOS) : return PlaylistType::PLAYLIST_CHAOS;
		case int(PlaylistType::PLAYLIST_RANKEDDUEL) : return PlaylistType::PLAYLIST_RANKEDDUEL;
		case int(PlaylistType::PLAYLIST_RANKEDDOUBLES) : return PlaylistType::PLAYLIST_RANKEDDOUBLES;
		case int(PlaylistType::PLAYLIST_RANKEDSOLOSTANDARD) : return PlaylistType::PLAYLIST_RANKEDSOLOSTANDARD;
		case int(PlaylistType::PLAYLIST_RANKEDSTANDARD) : return PlaylistType::PLAYLIST_RANKEDSTANDARD;
		case int(PlaylistType::PLAYLIST_RANKEDHOOPS) : return PlaylistType::PLAYLIST_RANKEDHOOPS;
		case int(PlaylistType::PLAYLIST_RANKEDRUMBLE) : return PlaylistType::PLAYLIST_RANKEDRUMBLE;
		case int(PlaylistType::PLAYLIST_RANKEDDROPSHOT) : return PlaylistType::PLAYLIST_RANKEDDROPSHOT;
		case int(PlaylistType::PLAYLIST_RANKEDSNOWDAY) : return PlaylistType::PLAYLIST_RANKEDSNOWDAY;
		default: return PlaylistType::PLAYLIST_UNKNOWN;
	}
}

inline PlaylistType PlaylistHelpers::ConvertToPluginPlaylist(PlaylistType playlistType)
{
	switch (playlistType)
	{
		case PlaylistType::PLAYLIST_EXHIBITION: return PlaylistType::PLAYLIST_EXHIBITION;
		case PlaylistType::PLAYLIST_PRIVATEMATCH: return PlaylistType::PLAYLIST_PRIVATEMATCH;
		case PlaylistType::PLAYLIST_CASUAL:
		case PlaylistType::PLAYLIST_DUEL :
		case PlaylistType::PLAYLIST_DOUBLES :
		case PlaylistType::PLAYLIST_STANDARD :
		case PlaylistType::PLAYLIST_CHAOS : return PlaylistType::PLAYLIST_CASUAL;
		case PlaylistType::PLAYLIST_RANKEDDUEL : return PlaylistType::PLAYLIST_RANKEDDUEL;
		case PlaylistType::PLAYLIST_RANKEDDOUBLES : return PlaylistType::PLAYLIST_RANKEDDOUBLES;
		case PlaylistType::PLAYLIST_RANKEDSTANDARD : return PlaylistType::PLAYLIST_RANKEDSTANDARD;
		case PlaylistType::PLAYLIST_RANKEDHOOPS : return PlaylistType::PLAYLIST_RANKEDHOOPS;
		case PlaylistType::PLAYLIST_RANKEDRUMBLE : return PlaylistType::PLAYLIST_RANKEDRUMBLE;
		case PlaylistType::PLAYLIST_RANKEDDROPSHOT : return PlaylistType::PLAYLIST_RANKEDDROPSHOT;
		case PlaylistType::PLAYLIST_RANKEDSNOWDAY : return PlaylistType::PLAYLIST_RANKEDSNOWDAY;
		default: return PlaylistType::PLAYLIST_UNKNOWN;
	}
}

inline std::string PlaylistHelpers::GetPlaylistName(PlaylistType playlist)
{
	switch (playlist)
	{
		case PlaylistType::PLAYLIST_EXHIBITION: return "Exhibition";
		case PlaylistType::PLAYLIST_PRIVATEMATCH: return "Private match";
		case PlaylistType::PLAYLIST_CASUAL: return "Casual";
		case PlaylistType::PLAYLIST_RANKEDDUEL: return "Ranked Duel";
		case PlaylistType::PLAYLIST_RANKEDDOUBLES: return "Ranked Duos";
		case PlaylistType::PLAYLIST_RANKEDSTANDARD: return "Ranked Standard";
		case PlaylistType::PLAYLIST_RANKEDHOOPS: return "Ranked Hoops";
		case PlaylistType::PLAYLIST_RANKEDRUMBLE: return "Ranked Rumble";
		case PlaylistType::PLAYLIST_RANKEDDROPSHOT: return "Ranked Dropshot";
		case PlaylistType::PLAYLIST_RANKEDSNOWDAY: return "Ranked Snowday";
		default: return "";
	}
}

inline std::string PlaylistHelpers::GetPlaylistFileSuffix(PlaylistType playlist)
{
	switch (playlist)
	{
		case PlaylistType::PLAYLIST_EXHIBITION: return "_exhibition";
		case PlaylistType::PLAYLIST_PRIVATEMATCH: return "_privatematch";
		case PlaylistType::PLAYLIST_CASUAL: return "_casual";
		case PlaylistType::PLAYLIST_RANKEDDUEL: return "_comp_duel";
		case PlaylistType::PLAYLIST_RANKEDDOUBLES: return "_comp_duos";
		case PlaylistType::PLAYLIST_RANKEDSTANDARD: return "_comp_standard";
		case PlaylistType::PLAYLIST_RANKEDHOOPS: return "_comp_hoops";
		case PlaylistType::PLAYLIST_RANKEDRUMBLE: return "_comp_rumble";
		case PlaylistType::PLAYLIST_RANKEDDROPSHOT: return "_comp_dropshot";
		case PlaylistType::PLAYLIST_RANKEDSNOWDAY: return "_comp_snowday";
		default: return "";
	}
}

inline PlaylistType PlaylistHelpers::GetNextPlaylist(PlaylistType currentPlaylist)
{
	switch (currentPlaylist)
	{
		case PlaylistType::PLAYLIST_UNKNOWN: return PlaylistType::PLAYLIST_EXHIBITION;
		case PlaylistType::PLAYLIST_EXHIBITION: return PlaylistType::PLAYLIST_PRIVATEMATCH;
		case PlaylistType::PLAYLIST_PRIVATEMATCH: return PlaylistType::PLAYLIST_CASUAL;
		case PlaylistType::PLAYLIST_CASUAL: return PlaylistType::PLAYLIST_RANKEDDUEL;
		case PlaylistType::PLAYLIST_RANKEDDUEL: return PlaylistType::PLAYLIST_RANKEDDOUBLES;
		case PlaylistType::PLAYLIST_RANKEDDOUBLES: return PlaylistType::PLAYLIST_RANKEDSTANDARD;
		case PlaylistType::PLAYLIST_RANKEDSTANDARD: return PlaylistType::PLAYLIST_RANKEDHOOPS;
		case PlaylistType::PLAYLIST_RANKEDHOOPS: return PlaylistType::PLAYLIST_RANKEDRUMBLE;
		case PlaylistType::PLAYLIST_RANKEDRUMBLE: return PlaylistType::PLAYLIST_RANKEDDROPSHOT;
		case PlaylistType::PLAYLIST_RANKEDDROPSHOT: return PlaylistType::PLAYLIST_RANKEDSNOWDAY;
		case PlaylistType::PLAYLIST_RANKEDSNOWDAY: return PlaylistType::PLAYLIST_CASUAL;
		default: return PlaylistType::PLAYLIST_UNKNOWN;
	}
}

inline PlaylistType PlaylistHelpers::GetPrevPlaylist(PlaylistType currentPlaylist)
{
	switch (currentPlaylist)
	{
		case PlaylistType::PLAYLIST_UNKNOWN: return PlaylistType::PLAYLIST_RANKEDSNOWDAY;
		case PlaylistType::PLAYLIST_EXHIBITION: return PlaylistType::PLAYLIST_RANKEDSNOWDAY;
		case PlaylistType::PLAYLIST_PRIVATEMATCH: return PlaylistType::PLAYLIST_EXHIBITION;
		case PlaylistType::PLAYLIST_CASUAL: return PlaylistType::PLAYLIST_PRIVATEMATCH;
		case PlaylistType::PLAYLIST_RANKEDDUEL: return PlaylistType::PLAYLIST_CASUAL;
		case PlaylistType::PLAYLIST_RANKEDDOUBLES: return PlaylistType::PLAYLIST_RANKEDDUEL;
		case PlaylistType::PLAYLIST_RANKEDSTANDARD: return PlaylistType::PLAYLIST_RANKEDDOUBLES;
		case PlaylistType::PLAYLIST_RANKEDHOOPS: return PlaylistType::PLAYLIST_RANKEDSTANDARD;
		case PlaylistType::PLAYLIST_RANKEDRUMBLE: return PlaylistType::PLAYLIST_RANKEDHOOPS;
		case PlaylistType::PLAYLIST_RANKEDDROPSHOT: return PlaylistType::PLAYLIST_RANKEDRUMBLE;
		case PlaylistType::PLAYLIST_RANKEDSNOWDAY: return PlaylistType::PLAYLIST_RANKEDDROPSHOT;
		default: return PlaylistType::PLAYLIST_UNKNOWN;
	}
}