#ifndef WROKU_REMOTE_H
#define WROKU_REMOTE_H

#include "comm.h"
	
typedef struct {
	char *label;
	wroku_command_t cmd;
} RemoteButtonAction;

void remote_init_nav();
void remote_init_playback();
void remote_deinit();

#endif /* define WROKU_REMOTE_H */