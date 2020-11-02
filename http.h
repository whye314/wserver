
int http_error(int erron);

struct _http_pack_struct {
    int version;
    union{
        int method;
        int status;
    };
    union{
        char * uri;
        char * comment;
    };
    char * body;
    http_head head[1];
};

struct _http_head_struct{
    char * key;
    char * value;
};

struct _http_body_struct{
    
};
typedef struct _http_pack_struct http_pack;
typedef struct _http_head_struct http_head;
typedef struct _http_body_struct http_body;


//http_version
#define HTTP10 0
#define HTTP11 1
#define HTTP20 2

//http_error
#define HTTP_VERSION_ERROR 65
#define HTTP_URI_ERROR 66
#define HTTP_METHOD_ERROR 67
#define HTTP_HEAD_ERROR 68


//http_method
#define HTTP_METHOD_GET 0
#define HTTP_METHOD_POST 1
#define HTTP_METHOD_OPTION 2
#define HTTP_METHOD_PUT 3
#define HTTP_METHOD_HEAD 4
#define HTTP_METHOD_DELETE 5