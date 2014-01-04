#include <pebble.h>

#include "remote.h"
#include "comm.h"
#include "main.h"
	
Window *remote_window;
TextLayer *remote_text_grid[3][3];
TextLayer *header_text_layer;

const RemoteButtonAction nav_page[3][3] = {
	{ { "Up", REMOTE_UP }, { "OK ", REMOTE_SELECT }, { "Down", REMOTE_DOWN } },
	{ { "Left", REMOTE_LEFT }, { "Back", REMOTE_BACK }, { "Right", REMOTE_RIGHT } },
	{ { " Apps", WROKU_APPS }, { "Home", REMOTE_HOME }, { "Playbk", WROKU_PLAYBACK } }
};

const RemoteButtonAction playback_page[3][3] = {
	{ { "Rev", REMOTE_REV }, { "Play ", REMOTE_PLAY }, { "Fwd", REMOTE_FWD } },
	{ { "Replay", REMOTE_INSTANTREPLAY }, { "Info", REMOTE_INFO }, { "", 0 } },
	{ { " Apps", WROKU_APPS }, { "Home", REMOTE_HOME }, { "Playbk", WROKU_PLAYBACK } }
};

RemoteButtonAction current_page[3][3];

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
	APP_LOG(APP_LOG_LEVEL_DEBUG, "in remote_init");
	remote_window = window_create();
	current_page = pagedef;
	app_message_init();
	
	window_set_click_config_provider(remote_window, remote_config_provider);
	/* accel_tap_service_subscribe(&remote_accel_tap_handler); */

	Layer *window_layer = window_get_root_layer(remote_window);
	
	const int column_x[] = { 95, 45, 0 };
	const int row_y[] = { 13, 65, 120 };
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			/* APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw label %d, %d: %s", i, j, pagedef[i][j].label); */
			remote_text_grid[i][j] = text_layer_create(GRect(column_x[i], row_y[j], 48, 40));
			text_layer_set_font(remote_text_grid[i][j], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
			text_layer_set_text(remote_text_grid[i][j], pagedef[i][j].label);
			if ((i == 2 && (j == 0 || j == 2)) || (i == 0 && j == 1)) {
				text_layer_set_background_color(remote_text_grid[i][j], GColorBlack);
				text_layer_set_text_color(remote_text_grid[i][j], GColorWhite);
			}
			if (i == 2 && (j == 0 || j == 2)) {
				text_layer_set_text_alignment(remote_text_grid[i][j], GTextAlignmentLeft);
			} else {
				text_layer_set_text_alignment(remote_text_grid[i][j], GTextAlignmentRight);
			}
			layer_add_child(window_layer, text_layer_get_layer(remote_text_grid[i][j]));
		}
	}
	
	header_text_layer = text_layer_create(GRect(0,0,140,15));
	text_layer_set_font(header_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(header_text_layer, "  Long      Double   Single");
	layer_add_child(window_layer, text_layer_get_layer(header_text_layer));

	window_stack_push(remote_window, true);
	/* APP_LOG(APP_LOG_LEVEL_DEBUG, "remote_init complete"); */
}

void remote_init_nav() {
	remote_init(nav_page);
}

void remote_init_playback() {
	remote_init(playback_page);
}

void remote_deinit(void) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			text_layer_destroy(remote_text_grid[i][j]);
		}
	}
	text_layer_destroy(header_text_layer);
	window_destroy(remote_window);
}
