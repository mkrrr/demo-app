#ifndef HANDLER_INC
#define HANDLER_INC

void handler_cmd(void *priv, const char *cmd, char *response, int resp_len);

void * cmd_handler_init();

void cmd_handler_deinit(void *priv);

#define MAX_CMD_SIZE 1024
#define RESP_MIN_CMD_SIZE 64

#endif   /* ----- #ifndef HANDLER_INC  ----- */

