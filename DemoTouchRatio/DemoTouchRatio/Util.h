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

#include "PlaylistEnum.h"

static class Util
{
public:
	static CarWrapper GetLocalPlayer();

	static bool IsLocalPlayer(CarWrapper& carWrapper);

	static float GetLocalBoost();

	static bool CanTrack();

	static int GetCurrentPing();

	static uint64_t TimestampInMS();

	static bool AreInSameTeam(CarWrapper carA, CarWrapper carB);

	static PlaylistType CurrentPlaylist();
};