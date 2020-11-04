#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

#include "error.h"
#include "listen.h"

int open_listen(unsigned short port){
    int listen_fd;
    
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) <0){
        return error(SOCKET_CREATE_ERROR);
    }
    struct sockaddr_in * my_addr;
    my_addr = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));
    my_addr->sin_family = AF_INET;
    my_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr->sin_port = htons(port);

    if(bind(listen_fd, (struct sockaddr *)my_addr, sizeof(struct sockaddr)) < 0){
        free(my_addr);
        return error(SOCKET_BIND_ERROR);
    }
    if(listen(listen_fd, MAX_LISTEN) < 0){
        free(my_addr);
        return error(SOCKET_LISTEN_ERROR);
    }

    return listen_fd;
}