#pragma once

#define NUMBER_OF_STATS_TRACKING 4

#define ID_INVALID_VALUE 0

#define CVAR_NAME_IN_MATCHES "cl_demotouch_plugin_in_matches"
#define CVAR_NAME_ON_SCOREBOARD "cl_demotouch_plugin_on_scoreboard"
#define CVAR_NAME_IN_FREEPLAY "cl_demotouch_plugin_in_freeplay"
#define CVAR_NAME_IN_CUSTOM_TRAINING "cl_demotouch_plugin_in_custom_training"
#define CVAR_NAME_IN_REPLAY "cl_demotouch_plugin_in_replay"
#define CVAR_NAME_DISPLAY_X "cl_demotouch_plugin_display_x"
#define CVAR_NAME_DISPLAY_Y "cl_demotouch_plugin_display_y"
#define CVAR_NAME_DISPLAY_SCALE "cl_demotouch_plugin_display_scale"
#define CVAR_NAME_COLOR_BACKGROUND "cl_demotouch_plugin_background_color"
#define CVAR_NAME_COLOR_TEXT "cl_demotouch_plugin_text_color"
#define CVAR_NAME_RENDER_TITLE "cl_demotouch_plugin_render_title"
#define CVAR_NAME_RENDER_HORIZONTAL "cl_demotouch_plugin_render_horizontal"
#define CVAR_NAME_CUSTOM_DESC_SIZE "cl_demotouch_plugin_custom_desc_size"
#define CVAR_NAME_ROW_SIZE "cl_demotouch_plugin_row_size"
#define CVAR_NAME_COLUMN_SIZE "cl_demotouch_plugin_column_size"
#define CVAR_NAME_MATCH_ACCOLADES "cl_demotouch_plugin_match_accolades"
#define CVAR_NAME_CUSTOM_DELAY_BUMP "cl_demotouch_plugin_custom_delay_bump"
#define CVAR_NAME_CUSTOM_DELAY_DEMO "cl_demotouch_plugin_custom_delay_demo"
#define CVAR_NAME_CUSTOM_DELAY_BALLHIT "cl_demotouch_plugin_custom_delay_ballhit"
#define CVAR_NAME_CUSTOM_DELAY_INAIR "cl_demotouch_plugin_custom_delay_inair"
#define CVAR_NAME_PERSISTENT_STATS "cl_demotouch_plugin_persistent_stats"

// [STAT_ADD] 1. Add display cvar name
#define CVAR_NAME_SHOW_BUMPS "cl_demotouch_plugin_show_bumps"
#define CVAR_NAME_SHOW_TEAMBUMPS "cl_demotouch_plugin_show_teambumps"
#define CVAR_NAME_SHOW_DEMOS "cl_demotouch_plugin_show_demos"
#define CVAR_NAME_SHOW_BALLHITS "cl_demotouch_plugin_show_ballhits"
#define CVAR_NAME_SHOW_BOOSTUSAGE "cl_demotouch_plugin_show_boostusage"
#define CVAR_NAME_SHOW_BOOSTPMINUTE "cl_demotouch_plugin_show_boostpminute"
#define CVAR_NAME_SHOW_INAIRPERCENTAGE "cl_demotouch_plugin_show_inairpercentage"
#define CVAR_NAME_SHOW_POWERSLIDE_COUNT "cl_demotouch_plugin_show_powerslide_count"
#define CVAR_NAME_SHOW_POWERSLIDE_DURATION "cl_demotouch_plugin_show_powerslide_duration"

#define CVAR_NAME_SHOW_TOTAL "cl_demotouch_plugin_show_total"
#define CVAR_NAME_SHOW_AVERAGE "cl_demotouch_plugin_show_average"
#define CVAR_NAME_SHOW_PLAYLISTS_TOTAL "cl_demotouch_plugin_show_playlists_total"
#define CVAR_NAME_SHOW_PLAYLISTS_AVERAGE "cl_demotouch_plugin_show_playlists_average"
#define CVAR_NAME_SHOW_PERSISTENT_TOTAL "cl_demotouch_plugin_show_persistent_total"
#define CVAR_NAME_SHOW_PERSISTENT_AVERAGE "cl_demotouch_plugin_show_persistent_average"
#define CVAR_NAME_SHOW_PLAYLISTS_PERSISTENT_TOTAL "cl_demotouch_plugin_show_playlists_persistent_total"
#define CVAR_NAME_SHOW_PLAYLISTS_PERSISTENT_AVERAGE "cl_demotouch_plugin_show_playlists_persistent_average"

// Cvar notifiers
#define CVAR_NOT_RESET_STATS "cl_demotouch_plugin_reset_stats"
#define CVAR_NOT_FORCE_SHOW_STATS "cl_demotouch_plugin_force_show_stats"
#define CVAR_NOT_RESET_FORCE_PLAYLIST "cl_demotouch_plugin_reset_force_playlist"
#define CVAR_NOT_FORCE_NEXT_PLAYLIST "cl_demotouch_plugin_force_next_playlist"
#define CVAR_NOT_FORCE_PREV_PLAYLIST "cl_demotouch_plugin_force_prev_playlist"

// Global plugin hooks
#define HOOK_COUNTDOWN_BEGINSTATE "Function GameEvent_TA.Countdown.BeginState"
#define HOOK_MATCH_STARTED "Function TAGame.GameEvent_TA.EventMatchStarted"
#define HOOK_ON_WINNER_SET "Function TAGame.GameEvent_Soccar_TA.EventMatchWinnerSet"
#define HOOK_ON_LEAVE_MATCH "Function TAGame.GFxShell_TA.LeaveMatch"
#define HOOK_ON_MAIN_MENU "Function TAGame.GFxData_MainMenu_TA.MainMenuAdded"
#define HOOK_ON_SCOREBOARD_OPENED "Function TAGame.GFxData_GameEvent_TA.OnOpenScoreboard"
#define HOOK_ON_SCOREBOARD_CLOSED "Function TAGame.GFxData_GameEvent_TA.OnCloseScoreboard"

// Game stats hooks
#define HOOK_SET_VEHICLE_INPUT "Function TAGame.Car_TA.SetVehicleInput"
#define HOOK_CAR_BUMPED "Function TAGame.Car_TA.BumpCar"
#define HOOK_CAR_DEMO "Function TAGame.Car_TA.EventDemolished"
#define HOOK_BALL_HIT "Function TAGame.Car_TA.OnHitBall"
#define HOOK_NUM_WHEEL_CONTACTS "Function TAGame.Vehicle_TA.GetNumWheelWorldContacts"
#define HOOK_CAR_HIT_WORLD "Function TAGame.Car_TA.EventHitWorld"
