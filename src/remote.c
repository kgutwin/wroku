#include <pebble.h>

#include "remote.h"
#include "comm.h"
#include "main.h"
	
Window *remote_window;
TextLayer *remote_text_grid[3][3];

const RemoteButtonAction navPage[3][3] = {
	{ { "Up", REMOTE_UP }, { "OK", REMOTE_SELECT }, { "Down", REMOTE_DOWN } },
	{ { "Left", REMOTE_LEFT }, { "Back", REMOTE_BACK }, { "Right", REMOTE_RIGHT } },
	{ { "Apps", WROKU_APPS }, { "Home", REMOTE_HOME }, { "Playbk", WROKU_PLAYBACK } }
};

const RemoteButtonAction playbackPage[3][3] = {
	{ { "Rev", REMOTE_REV }, { "Play", REMOTE_PLAY }, { "Fwd", REMOTE_FWD } },
	{ { "Replay", REMOTE_INSTANTREPLAY }, { "Info", REMOTE_INFO }, { "", 0 } },
	{ { "Apps", WROKU_APPS }, { "Home", REMOTE_HOME }, { "Playbk", WROKU_PLAYBACK } }
};

void remote_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	/* text_layer_set_text(remote_text_layer, "PlayPause"); */
	APP_LOG(APP_LOG_LEVEL_DEBUG, "PlayPause");
	send_command(REMOTE_PLAY);
	app_timer_reschedule(timeout_quit_timer, TIMEOUT_QUIT_MS);
}

void remote_up_click_handler(ClickRecognizerRef recognizer, void *context) {
	/* text_layer_set_text(remote_text_layer, "Up"); */
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Up");
	send_command(REMOTE_UP);
	app_timer_reschedule(timeout_quit_timer, TIMEOUT_QUIT_MS);
}

void remote_down_click_handler(ClickRecognizerRef recognizer, void *context) {
	/* text_layer_set_text(remote_text_layer, "Down"); */
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Down");
	send_command(REMOTE_DOWN);
	app_timer_reschedule(timeout_quit_timer, TIMEOUT_QUIT_MS);
}

void remote_accel_tap_handler(AccelAxisType axis, int32_t direction) {
	char *message = "None";
	switch (axis) {
		case ACCEL_AXIS_X:
			message = direction == -1 ? "Left" : "Right";
			break;
		case ACCEL_AXIS_Y:
			message = direction == -1 ? "Down" : "Up";
			break;
		case ACCEL_AXIS_Z:
			message = direction == -1 ? "OK" : "Back";
			break;
	}
	/* text_layer_set_text(remote_text_layer, message); */
	APP_LOG(APP_LOG_LEVEL_DEBUG, message);
}

void remote_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, remote_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, remote_down_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, remote_up_click_handler);
}

void remote_init(const RemoteButtonAction pagedef[3][3]) {
	remote_window = window_create();
	app_message_init();
	
	window_set_click_config_provider(remote_window, remote_config_provider);
	/* accel_tap_service_subscribe(&remote_accel_tap_handler); */

	Layer *window_layer = window_get_root_layer(remote_window);
	
	const int column_x[] = { 90, 40, 5 };
	const int row_y[] = { 5, 60, 120 };
	
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 3; j++) {
			remote_text_grid[i][j] = text_layer_create(GRect(column_x[i], row_y[j], 50, 50));
			text_layer_set_font(remote_text_grid[i][j], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
			text_layer_set_text_alignment(remote_text_grid[i][j], GTextAlignmentRight);
			text_layer_set_text(remote_text_grid[i][j], pagedef[i][j].label);
			layer_add_child(window_layer, text_layer_get_layer(remote_text_grid[i][j]));
		}
	}

	window_stack_push(remote_window, true);
}

void remote_init_nav() {
	remote_init(navPage);
}

void remote_init_playback() {
	remote_init(playbackPage);
}

void remote_deinit(void) {
	/*
	text_layer_destroy(remote_text_up);
	text_layer_destroy(remote_text_center);
	text_layer_destroy(remote_text_down);
	*/
	window_destroy(remote_window);
}
