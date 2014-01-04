#include <pebble.h>

#include "comm.h"

void send_command(wroku_command_t cmd) {
	Tuplet command_tuple = TupletInteger(COMMAND_MESSAGE, cmd);
	
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	if (iter == NULL) return;

	dict_write_tuplet(iter, &command_tuple);
	dict_write_end(iter);

	app_message_outbox_send();
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
