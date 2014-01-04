#include <pebble.h>

#include "remote.h"
#include "comm.h"
#include "main.h"
	
Window *remote_window;
TextLayer *remote_text_up, *remote_text_center, *remote_text_down;

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

void remote_init(void) {
	remote_window = window_create();
	app_message_init();
	
	window_set_click_config_provider(remote_window, remote_config_provider);
	/* accel_tap_service_subscribe(&remote_accel_tap_handler); */

	Layer *window_layer = window_get_root_layer(remote_window);
	
	remote_text_up = text_layer_create(GRect(90, 5, 50, 50));
	text_layer_set_font(remote_text_up, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(remote_text_up, GTextAlignmentRight);
	text_layer_set_text(remote_text_up, "Up");
	layer_add_child(window_layer, text_layer_get_layer(remote_text_up));
	
	remote_text_center = text_layer_create(GRect(90, 60, 50, 50));
	text_layer_set_font(remote_text_center, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(remote_text_center, GTextAlignmentRight);
	text_layer_set_text(remote_text_center, "Play");
	layer_add_child(window_layer, text_layer_get_layer(remote_text_center));
	
	remote_text_down = text_layer_create(GRect(90, 120, 50, 50));
	text_layer_set_font(remote_text_down, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(remote_text_down, GTextAlignmentRight);
	text_layer_set_text(remote_text_down, "Down");
	layer_add_child(window_layer, text_layer_get_layer(remote_text_down));

	window_stack_push(remote_window, true);
}

void remote_deinit(void) {
	text_layer_destroy(remote_text_up);
	text_layer_destroy(remote_text_center);
	text_layer_destroy(remote_text_down);
	window_destroy(remote_window);
}
