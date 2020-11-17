

#define IO_MAX_BUF 512

 
struct _io_buf_struct{
    unsigned int fd;
    unsigned short len;
    unsigned short vernier;
    char s[IO_MAX_BUF];
};


typedef struct _io_buf_struct io_buf;

int io_init(io_buf * io, int fd);
int io_readn(io_buf * io, char * buf, int size);
int io_readc(io_buf * io, char * buf, const char c);
int io_reads(io_buf * io, char * buf, const char * s);