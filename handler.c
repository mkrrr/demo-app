
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "handler.h"

#define LIKE_CHEESE "Do you like cheese?"
#define LIKE_COFFEE "Do you like coffee?"
#define LIKE_CHOCOLATE "Do you like chocolate?"
#define FIRST_DAY "How was your first day at altran?"
#define FIX "Will you fix this bug on the master branch now please?"
#define LIKE_TWIX "Do you like Twix?"
#define LIke_BOUNTY "Do you like Bounty?"

struct handler{
    int cmds_rcv;
    int cmds_rcv_err;
};

void handler_cmd(void *priv, const char *cmd, char *response, int resp_len)
{
    struct handler *h = (struct handler *) priv;

    if(!response || resp_len < RESP_MIN_CMD_SIZE)
        return;

    h->cmds_rcv++;

    if(strncmp(cmd, LIKE_CHEESE, strlen(LIKE_CHEESE)) == 0){
        sprintf(response, "Yes, especially blue cheese");
    }else if(strncmp(cmd, LIKE_COFFEE, strlen(LIKE_COFFEE)) == 0){
        sprintf(response, "Of course!");
    }else if(strncmp(cmd, FIX, strlen(FIX)) == 0){
        sprintf(response, "Alright then");
    }else if(strncmp(cmd, LIKE_CHOCOLATE, strlen(FIX)) == 0){
        sprintf(response, "Yes, I prefer milk chocolate over pure");
    }else if(strncmp(cmd, FIRST_DAY, strlen(FIX)) == 0){
        sprintf(response, "It was very interesting. Especially the demo's");
    }else if(strncmp(cmd, FIX, strlen(FIX)) == 0){
        sprintf(response, "Twixxxx for life");
    }else if(strncmp(cmd, FIX, strlen(FIX)) == 0){
        sprintf(response, "Bountyyyyyyyy");
    }else{
        h->cmds_rcv_err++;
        sprintf(response, "Wuuuuut?");
    }
}

void * cmd_handler_init()
{
    struct handler *h = malloc(sizeof(*h));
    if(!h)
        return NULL;
    return h;
}

void cmd_handler_deinit(void *priv)
{
    struct handler *h = (struct handler *) priv;
    free(h);
}

