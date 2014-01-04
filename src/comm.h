#ifndef WROKU_COMM_H
#define WROKU_COMM_H

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

void app_message_init(void);
void send_command(wroku_command_t cmd);

#endif /* define WROKU_COMM_H */