/*
 * =====================================================================================
 *
 * Demo app 
 *
 * =====================================================================================
 */
      
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <getopt.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
      
#include <sys/wait.h>
#include <signal.h>

#include "os.h"
#include "handler.h"
                                       
static void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}     

static void print_usage()
{
    printf("Usage:\n-p [PORT], TCP port to listen for commands\n"
            "-h, Print this help message\n"
            "-b, Run as a daemon in background\n");
}
          
#define MAX_CONN 10

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

static int init_sock(int port)
{
    struct addrinfo hints, *res;
    char pbuf[8];
    int sock;

    sprintf(pbuf, "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, pbuf, &hints, &res);
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sock <= 0){
        freeaddrinfo(res);
        perror("socket");
        return -1;
    }
    if(bind(sock, res->ai_addr, res->ai_addrlen)){
        freeaddrinfo(res);
        perror("bind");
        return -1;
    }
    listen(sock, MAX_CONN);
    freeaddrinfo(res);

    return sock;
}

static void accept_conn(int sock, void *cmd_handler)
{
    struct sockaddr_storage peer;
    char s[INET6_ADDRSTRLEN];
    socklen_t peer_sz;
    int fd;

    while(1){
        peer_sz = sizeof peer;
        fd = accept(sock, (struct sockaddr *) &peer, &peer_sz);
        if(fd == -1){
            perror("accept");
            continue;            
        }
        inet_ntop(peer.ss_family, get_in_addr((struct sockaddr *)&peer), s, sizeof s);
        printf("Got connection from %s\r\n", s);
        if(!fork()){
            char buf[MAX_CMD_SIZE];
            char response[MAX_CMD_SIZE];
            close(sock);  
            while(1){
                int nbytes = 0;
                memset(buf, 0, sizeof buf);
                nbytes = recv(fd, buf, sizeof buf, 0);
                if(nbytes < 0){
                    perror("recv");
                    break;
                }else if(nbytes == 0){
                    printf("Remote end closed the connection\r\n");
                    break;
                }else{
                    printf("Received %d bytes\r\n", nbytes);
                    /* strip off line feeds and cr*/
                    char *pos = buf;
                    while(pos < (buf + sizeof buf)){
                        if(*pos == 0)
                            break;
                        if(*pos == '\r' || *pos == '\n')
                            *pos = 0;
                        pos++;
                    }
                    printf("Received cmd: [%s ]\r\n", buf);
                    handler_cmd(cmd_handler, buf, response, sizeof response);
                    printf("Sending response: [%s]\r\n", response);
                    if(send(fd, response, strlen(response), 0) < 0)
                        perror("send");
                }
            }
            close(fd);
            exit(0);
        }
        close(fd);
    }
}
 
int main(int argc, char **argv)
{   
    void *cmd_handler;
    struct sigaction sa;
    int app_opts = 0;
    int port = 0;
    int bg = 0;
    int s;

    while((app_opts = getopt(argc, argv, "p:bh")) != -1){
        switch(app_opts) {
            case 'p':
                sscanf(optarg, "%d", &port);
                break;
            case 'b':
                bg = 1;
                break;
            case 'h':
                print_usage();
                exit(0);
                break;
        }
    }
    if(port == 0){
        printf("Port not specified\n");
        exit(0);
    }

    if(bg){
        if(os_daemon(0, 0)){
            printf("Error, could not daemonize\r\n");
            exit(EXIT_FAILURE);
        }
    } 
    
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    if((s = init_sock(port)) <= 0){
        printf("Error, could not listen on port %d\r\n", port);
        exit(EXIT_FAILURE);
    }else{
        printf("Listening on port %d\n", port);
    }

    cmd_handler = cmd_handler_init();

    accept_conn(s, cmd_handler);

    cmd_handler_deinit(cmd_handler);

    return 0;
}              
