

struct _io_buf_struct{
    unsigned int fd;
    unsigned int len;
    char s[100];
};


typedef struct _io_buf_struct io_buf;