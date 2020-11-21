#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"
#include "types.h"
#include "log.h"
#include "error.h"

//we can not know when to end this request, so we should parse the head and find the Content-Length, then read(0, body, Content-Length) + '\0';
http_pack * str_to_http_head(char * str){//receive a http package, must a start line, and end by \r\n\0
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
        addstr(slist, node);
    }
    start = start + 2;
    //char * body;
    //body = (char *)calloc(1, strlen(start) + 1);
    //strcpy(body, start);

    http_pack * http;
    http = calloc(1, (sizeof(http_pack) + sizeof(http_head) * (num - 2)));
    http->body.len = 0;
    //we need parse the http head to get the content-length and then receive the body 

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
        http_pack_free(http);
        return error(HTTP_VERSION_ERROR);
    }

    //http uri
    *last = 0;
    last = strrchr(slist->val, 0x20);
    if(!last){
        http_pack_free(http);
        return error(HTTP_URI_ERROR);
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
        http_pack_free(http);
        return error(HTTP_METHOD_ERROR);
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
            http_pack_free(http);
            return error(HTTP_HEAD_ERROR);
        }
    http->head[num].value = (val + 2);
    *val = 0;
    http->head[num++].key = slist->val;
    node = slist;
    slist = slist->next;
    free(node);
    }
    http->head_num = num;


    return http;

}


strlist * http_to_strlist(http_pack * http){

    strlist * slist;
    slist = (strlist *)calloc(1, sizeof(strlist));
    slist->fina = slist;

    char * temp;
    temp = (char *)calloc(1, (strlen(http->comment) + 15));
    if(http->version == HTTP10){
        //*temp = "HTTP/1.0";
        memcpy(temp, "HTTP/1.0", 8);
    }
    else if(http->version == HTTP11){
        //*temp = "HTTP/1.1";
        memcpy(temp, "HTTP/1.1", 8);
    }
    else if(http->version == HTTP20){
        //*temp = "HTTP/2.0";
        memcpy(temp, "HTTP/2.0", 8);
    }
    else{
        free(temp);
        http_slist_free(slist);
        return error(HTTP_STR_VERSION_ERROR);
    }
    sprintf((temp + 8), " %hd %s\r\n", http->status, http->comment);

    strlist * node;
    node = (strlist *)calloc(1, sizeof(strlist));
    node->val = temp;
    addstr(slist, node);

    int num = 0;
    while(num < http->head_num){
        char * key, * value;
        key = http->head[num].key;
        value = http->head[num].value;
        temp = (char *)calloc(1, (strlen(key) + strlen(value) + 5));
        sprintf(temp, "%s: %s\r\n", key, value);
        node = (strlist *)calloc(1, sizeof(strlist));
        node->val = temp;
        addstr(slist, node);
        num++;
    }

    //body
    *temp = (char *)calloc(1, http->body.len + 2);
    sprintf(temp, "%s\r\n", http->body.val);
    node = (strlist *)calloc(1, sizeof(strlist));
    node->val = temp;
    addstr(slist, node);

    return slist;
}


char * http_get_head_val(http_pack * http, const char * key){//return pointer that already exists
    int num = http->head_num;
    while(num-- > 0){
        if(!strcmp(http->head[num].key, key)){
            // char *s;
            // s = (char *)calloc(1, strlen(http->head[num].value) + 1);
            // memcpy(s, http->head[num].value, strlen(http->head[num].value) + 1);
            // return s;
            return http->head[num].value;
        }
    }
    return 0;
}


char * http_to_str(char * buf, http_pack * http){
    strlist * slist;
    slist = http_to_strlist(http);
    buf = (char *)calloc(1, (HTTP_MAX_HEAD_SIZE + http->body.len) * sizeof(char));
    int writelen = 0;
    while(slist = slist->next != NULL){
        strcpy((buf+writelen), slist->val);
        strcat(buf, "\r\n");
        writelen += (strlen(slist->val)) + 2;
    }
    strcat(buf, "\r\n");
    writelen += 2;
    memcpy((buf+writelen), http->body.val, http->body.len);//http_head_len < HTTP_MAX_HEAD_SIZE - 1
    writelen += http->body.len;
    return writelen;
}


int http_pack_free(http_pack * http){
    int i = http->head_num;
    while(i-->=0){
        free(http->head[i].value);
        free(http->head[i].key);
    }
    free(http->body.val);
    free(http->uri);
    free(http);
    return 0;
}


http_pack * http_prase(http_pack * http_request){
    char * uri;
    int urilen = strlen(http_request->uri);
    uri = (char *)calloc(1, (urilen + 1) * sizeof(char));
    memcpy(uri, http_request->uri, urilen + 1);
    int i = 0;
    if(i = strstr(uri, "../") != NULL){
        int index = i;
        for(index = i; index <= urilen-3; index++){
            uri[index] = uri[index+3];
        }
    }
}
