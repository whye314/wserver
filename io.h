

#define IO_MAX_BUF 516

 
struct _io_buf_struct{
    unsigned int fd;
    unsigned int len;
    unsigned int vernier;
    char s[IO_MAX_BUF];
};


typedef struct _io_buf_struct io_buf;