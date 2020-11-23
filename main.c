#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "listen.h"
#include "types.h"
#include "http.h"
#include "log.h"
#include "error.h"
#include "io.h"

int doit(int fd);

int main(int argc, char ** args){
    int listenfd = 0, connfd = 0;
    int listen_port = PORT;
    if((listenfd = open_listen(listen_port)) < 0){//instead of if(listenfd = open_listen(listen_port) < 0)
        return error(SOCKET_OPEN_LISTEN_ERROR);
    }

    struct sockaddr_in * client_addr;
    client_addr = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));
    int clientlen = sizeof(struct sockaddr_in);

    while(1){
        if((connfd = accept(listenfd, (struct sockaddr *)client_addr, &clientlen)) < 0){
            perror("accept");
            free(client_addr);
            return error(SOCKET_ACCEPT_ERROR);
        }
        doit(connfd);

        close(connfd);
    }

}

int doit(int fd){
    char * tmps;
    tmps = (char *)calloc(1, HTTP_MAX_HEAD_SIZE * sizeof(char));
    io_buf * io;
    io = (io_buf *)calloc(1, sizeof(io_buf));
    io_init(io, fd);
    if(io_reads(io, tmps, "\r\n\r\n") > HTTP_MAX_HEAD_SIZE){
        
        free(io);
        free(tmps);
        close(fd);
        error(HTTP_HEAD_TO_LARGE);
        return -1;
    }
    http_pack * http_request;
    http_request = str_to_http_head(tmps);
    if(http_request->method == HTTP_METHOD_POST){
        int l = atoi(http_get_head_val(http_request, "Content-Length"));
        if(l > 0){
            http_request->body.len = l;
            char * body;
            body = (char *)calloc(1, l+1);
            int readlen = 0;
            while(1){//need alarm
                readlen += io_readn(io, (body+readlen), (l-readlen));
                if(readlen == l) break;
            }
            http_request->body.val = body;
        }

    }

    http_pack * http_response;
    http_response = http_prase(fd, http_request);
    // char * http_str;
    // int response_len = 0;
    // response_len = http_to_str(http_str, http_response);
    // write(fd, http_str, response_len);
    free(tmps);
    free(io);
    free(http_response);
    http_pack_free(http_request);
    //http_pack_free(http_response);
    close(fd);
    return 0;






    
        

}