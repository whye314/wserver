#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "listen.h"
#include "types.h"
#include "http.h"
#include "log.h"
#include "error.h"

int doit(int fd);

int main(int argc, char ** args){
    int listenfd, connfd;
    int listen_port = 80;
    listenfd = open_listen(listen_port);

    struct sockaddr_in * client_addr;
    client_addr = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));
    int clientlen = sizeof(struct sockaddr_in);

    while(1){
        if(connfd = accept(listenfd, (struct sockaddr *)client_addr, &clientlen) < 0){
            free(client_addr);
            return error(SOCKET_ACCEPT_ERROR);
        }
        doit(connfd);

        close(connfd);
    }

}

int doit(int fd){
    
}