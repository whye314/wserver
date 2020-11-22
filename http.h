
struct _http_head_struct{
    char * key;
    char * value;
};

typedef struct _http_head_struct http_head;

struct _http_body_struct{
    int len;
    char * val;
};
typedef struct _http_body_struct http_body;

struct _http_pack_struct {
    short version;
    union{
        short method;
        short status;
    };
    int head_num;
    union{
        char * uri;
        char * comment;
    };
    http_body body;
    union{
        http_head head[1];
    };
};

typedef struct _http_pack_struct http_pack;


int http_pack_free(http_pack * http);
char * http_get_head_val(http_pack * http, const char * key);
http_pack * str_to_http_head(char * str);
strlist * http_to_strlist(http_pack * http);
int http_to_str(char * buf, http_pack * http);//return response http pack len, include head and body
http_pack * http_prase(int fd, http_pack * http_request);


#ifndef HTTP10
//http_version
#define HTTP10 3
#define HTTP11 1
#define HTTP20 2



//http_error
#define HTTP_VERSION_ERROR 65
#define HTTP_URI_ERROR 66
#define HTTP_METHOD_ERROR 67
#define HTTP_HEAD_ERROR 68

#define HTTP_STR_VERSION_ERROR 75
#define HTTP_STR_HEAD_ERROR 76


//http_method
#define HTTP_METHOD_GET 6
#define HTTP_METHOD_POST 1
#define HTTP_METHOD_OPTION 2
#define HTTP_METHOD_PUT 3
#define HTTP_METHOD_HEAD 4
#define HTTP_METHOD_DELETE 5

#define HTTP_MAX_HEAD_SIZE 4096

#define ROOT_DOCUMENT "/var/www/html/"

#define DEFAULT_TARGET_FILE "index.html"

#endif