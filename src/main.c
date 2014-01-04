#include <pebble.h>

#include "main.h"
#include "remote.h"

AppTimer *timeout_quit_timer;

void timeout_quit_handler(void *data) {
	window_stack_pop_all(true);
}

int main(void) {
	remote_init();
	timeout_quit_timer = app_timer_register(TIMEOUT_QUIT_MS, timeout_quit_handler, NULL);
	app_event_loop();
	remote_deinit();
}
