#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <http.h>
#include <types.h>

int str_to_http(char * str){//receive a http package, must a start line, and end by \r\n\0
    char * start;
    start = str;
    int num = 0;
    strlist * slist;
    slist = (strlist *)calloc(1, sizeof(strlist));
    slist->fina = slist;


    while(1){
        int size = 0;
        char * temp;
        if(size = strstr(start, "\r\n") - start)
            break;
        temp = (char *)calloc(1, size+1);
        memcpy(temp, start, size);
        start = start + size + 2;
        num ++;

        strlist * node;
        node = (strlist *)calloc(1, sizeof(strlist));
        node->val = temp;
        add(slist, node);
    }
    start = start + 2;
    char * body;
    body = (char *)calloc(1, strlen(start) + 1);
    strcpy(body, start);

    http_pack * http;
    http = calloc(1, (sizeof(http_pack) + sizeof(http_head) * (num - 2)));
    http->body = start;

    strlist * node;
    node = slist;
    slist = slist->next;
    free(node);

    //first line
    //http version
    char * last;
    last = strrchr(slist->val, 0x20);
    if(strcmp(last+1, "HTTP/1.0") == 0){
        http->version = HTTP10;
    }
    else if(strcmp(last+1, "HTTP/1.1") == 0){
        http->version = HTTP11;
    }
    else if(strcmp(last+1, "HTTP/2.0") == 0){
        http->version = HTTP20;
    }
    else{
        return http_error(HTTP_VERSION_ERROR);
    }

    //http uri
    *last = 0;
    last = strrchr(slist->val, 0x20);
    if(!last){
        return http_error(HTTP_URI_ERROR);
    }
    //http->uri = last + 1;
    http->uri = calloc(1, strlen(last) + 1);
    strcpy(http->uri, last + 1);
    *last = 0;

    //the chunk of method/uri/version will nerver be recovered because the address has lost(sovled)
    //http method
    if(strcmp(slist->val, "GET") == 0){
        http->method = HTTP_METHOD_GET;
    }
    else if(strcmp(slist->val, "POST") == 0){
        http->method = HTTP_METHOD_POST;
    }
    else if(strcmp(slist->val, "OPTION") == 0){
        http->method = HTTP_METHOD_OPTION;
    }
    else if(strcmp(slist->val, "PUT") == 0){
        http->method = HTTP_METHOD_PUT;
    }
    else if(strcmp(slist->val, "HEAD") == 0){
        http->method = HTTP_METHOD_HEAD;
    }
    else if(strcmp(slist->val, "DELETE") == 0){
        http->method = HTTP_METHOD_DELETE;
    }
    else{
        return http_error(HTTP_METHOD_ERROR);
    }
    free(slist->val);

    node = slist;
    slist = slist->next;
    free(node);

    //http head
    num = 0;
    while(slist!=NULL){
        char * val;
        if(val = strstr(slist->val, ": ")){
            return http_error(HTTP_HEAD_ERROR);
        }
    http->head[num].value = (val + 2);
    *val = 0;
    http->head[num++].key = slist->val;
    node = slist;
    slist = slist->next;
    free(node);
    }
    return 0;

}



int http_error(int erron){
    error_log(erron);
    //exit(erron);
    return erron;
}