#include <pebble.h>

Window *remote_window;
TextLayer *remote_text_layer;

enum {
	COMMAND_MESSAGE = 0x0,
};

typedef enum {
	REMOTE_HOME = 1,
	REMOTE_REV = 2,
	REMOTE_FWD = 3,
	REMOTE_PLAY = 4,
	REMOTE_SELECT = 5,
	REMOTE_LEFT = 6,
	REMOTE_RIGHT = 7,
	REMOTE_DOWN = 8,
	REMOTE_UP = 9,
	REMOTE_BACK = 10,
	REMOTE_INSTANTREPLAY = 11,
	REMOTE_INFO = 12,
	REMOTE_BACKSPACE = 13,
	REMOTE_SEARCH = 14,
	REMOTE_ENTER = 15,
} wroku_command_t;

void handle_deinit(void) {
	text_layer_destroy(remote_text_layer);
	window_destroy(remote_window);
}

void send_command(wroku_command_t cmd) {
	Tuplet command_tuple = TupletInteger(COMMAND_MESSAGE, cmd);
	
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	if (iter == NULL) return;

	dict_write_tuplet(iter, &command_tuple);
	dict_write_end(iter);

	app_message_outbox_send();
}

void remote_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	text_layer_set_text(remote_text_layer, "PlayPause");
	APP_LOG(APP_LOG_LEVEL_DEBUG, "PlayPause");
	send_command(REMOTE_PLAY);
}

void remote_up_click_handler(ClickRecognizerRef recognizer, void *context) {
	text_layer_set_text(remote_text_layer, "Up");
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Up");
	send_command(REMOTE_UP);
}

void remote_down_click_handler(ClickRecognizerRef recognizer, void *context) {
	text_layer_set_text(remote_text_layer, "Down");
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Down");
	send_command(REMOTE_DOWN);
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
	text_layer_set_text(remote_text_layer, message);
	APP_LOG(APP_LOG_LEVEL_DEBUG, message);
}

void remote_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, remote_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, remote_down_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, remote_up_click_handler);
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Received");
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
}

void app_message_init() {
	// Register message handlers
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_failed(out_failed_handler);
	// init buffers
	app_message_open(64, 64);
}

void handle_init(void) {
	remote_window = window_create();
	app_message_init();
	
	window_set_click_config_provider(remote_window, remote_config_provider);
	/* accel_tap_service_subscribe(&remote_accel_tap_handler); */

	Layer *window_layer = window_get_root_layer(remote_window);
	
	remote_text_layer = text_layer_create(GRect(10, 10, 144, 20));
	text_layer_set_text_color(remote_text_layer, GColorBlack);
	text_layer_set_text(remote_text_layer, "Init");
	layer_add_child(window_layer, text_layer_get_layer(remote_text_layer));
	
	window_stack_push(remote_window, true);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
