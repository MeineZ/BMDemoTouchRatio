#pragma once

#define EVENT_NAME_DEMO "Demos"
#define EVENT_NAME_BUMP "Bumps"
#define EVENT_NAME_BALL_HIT "Ball hits"

#define ID_INVALID_VALUE 0

//#define CVAR_NAME_DISPLAY "cl_session_plugin_display"

#define HOOK_COUNTDOWN_BEGINSTATE "Function GameEvent_TA.Countdown.BeginState"
#define HOOK_ON_WINNER_SET "Function TAGame.GameEvent_Soccar_TA.EventMatchWinnerSet"
#define HOOK_ON_MAIN_MENU "Function TAGame.GFxData_MainMenu_TA.MainMenuAdded"

#define HOOK_CAR_BUMPED "Function TAGame.Car_TA.EventBumpedCar"
#define HOOK_CAR_DEMO "Function TAGame.Car_TA.EventDemolished"
#define HOOK_BALL_HIT "Function TAGame.Car_TA.OnHitBall"