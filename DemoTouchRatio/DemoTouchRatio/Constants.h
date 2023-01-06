#pragma once

#define EVENT_NAME_DEMO "Demos"
#define EVENT_NAME_BUMP "Bumps"
#define EVENT_NAME_BALL_HIT "Ball hits"

#define ID_INVALID_VALUE 0

#define TIMEOUT_DEMO 0
#define TIMEOUT_BALL_HIT 100

#define CVAR_NAME_ENABLED "cl_demotouch_plugin_enabled"
#define CVAR_NAME_IN_MATCHES "cl_demotouch_plugin_in_matches"
#define CVAR_NAME_IN_FREEPLAY "cl_demotouch_plugin_in_freeplay"
#define CVAR_NAME_IN_CUSTOM_TRAINING "cl_demotouch_plugin_in_custom_training"
#define CVAR_NAME_IN_REPLAY "cl_demotouch_plugin_in_replay"
#define CVAR_NAME_DISPLAY_X "cl_demotouch_plugin_display_x"
#define CVAR_NAME_DISPLAY_Y "cl_demotouch_plugin_display_y"
#define CVAR_NAME_DISPLAY_SCALE "cl_demotouch_plugin_display_scale"
#define CVAR_NAME_DISPLAY_SCALE_RESET "cl_demotouch_plugin_display_scale_reset"
#define CVAR_NAME_COLOR_BACKGROUND "cl_demotouch_plugin_background_color"
#define CVAR_NAME_COLOR_TEXT "cl_demotouch_plugin_text_color"
#define CVAR_NAME_RESET_COLORS "cl_demotouch_plugin_reset_colors"
#define CVAR_NAME_RESET "cl_demotouch_plugin_reset"

#define CVAR_NAME_SHOW_BUMPS "cl_demotouch_plugin_show_bumps"
#define CVAR_NAME_SHOW_TEAMBUMPS "cl_demotouch_plugin_show_teambumps"
#define CVAR_NAME_SHOW_DEMOS "cl_demotouch_plugin_show_demos"
#define CVAR_NAME_SHOW_BALLHITS "cl_demotouch_plugin_show_ballhits"

#define HOOK_COUNTDOWN_BEGINSTATE "Function GameEvent_TA.Countdown.BeginState"
#define HOOK_ON_WINNER_SET "Function TAGame.GameEvent_Soccar_TA.EventMatchWinnerSet"
#define HOOK_ON_MAIN_MENU "Function TAGame.GFxData_MainMenu_TA.MainMenuAdded"

#define HOOK_CAR_BUMPED_OLD "Function TAGame.Car_TA.EventBumpedCar"
#define HOOK_CAR_BUMPED "Function TAGame.Car_TA.BumpCar"
#define HOOK_CAR_DEMO "Function TAGame.Car_TA.EventDemolished"
#define HOOK_BALL_HIT "Function TAGame.Car_TA.OnHitBall"