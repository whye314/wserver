#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "error.h"


#include "io.h"

int io_flush(io_buf * io);

// int io_read(io_buf * io, char * buf, int size){
//     if(size == 0) return 0;
//     if(io->len >= size){
//         memcpy(buf, io->s, size);
//         memset(io->s, 0, size);
//         int i;
//         for(i = 0; i < (io->len-size); i++){
//             io->s[i] = io->s[i+size];
//         }
//         io->len = io->len - size;
//         return size;
//     }
//     else{
//         int n = 0;
//         char * tmps;
//         tmps = (char *)calloc(1, size);
//         memcpy(tmps, io->s, io->len);
//         memset(io->s, 0, IO_MAX_BUF);
//         n = read(io->fd, tmps, size - io->len);
//         n = n + io->len;
//         io->len = 0;
//         if(n == size){
//             io->len = read(io->fd, io->s, IO_MAX_BUF);
//         }
//         return n;
//     }
// }

int io_readn(io_buf * io, char * buf, int size){
    if(size == 0) return 0;
    if(io->len >= size){
        memcpy(buf, (io->s + io->vernier), size);
        io->vernier = io->vernier + size;
        io->len = io->len - size;
        return size;
    }
    else{
        int len = io->len;
        int n = 0;
        memcpy(buf, (io->s + io->vernier), len);
        memset(io->s, 0, IO_MAX_BUF);
        io->vernier = 0;
        io->len = 0;
        
        if((n = read(io->fd, buf, size - len)) < 0)
            return n;
        n += len;
        if(n < size)
            return n;
        io->len = read(io->fd, io->s, IO_MAX_BUF - 1);
        
        return n;

    }
}

int io_readc(io_buf * io, char * buf, const char c){
    int readlen = 0;
    char s;
    while(io_readn(io, &s, 1)>0){
        *(buf + readlen) = s;
        readlen ++;
        if(c == s) return readlen;
    }
    return readlen;
}

int io_reads(io_buf * io, char * buf, const char * s){//len(s)<(IO_MAX_BUF)

    int len = strlen(s);
    if(len == 0) return 0;
    if(len > io->len){
        io_flush(io);
    }
    char * i = 0, readlen = 0;
    while(1)
    {
        if((i = (strstr(io->s, s) - io->s)) != NULL){
            readlen += io_readn(io, buf, ((int)i + len));
            return readlen;
        }
        int l = io_readn(io, (buf + readlen), (IO_MAX_BUF - len));
        if(l>0) readlen += l;
        else return readlen;
    }

}

int io_init(io_buf * io, int fd){
    memset(io, 0, sizeof(io_buf));
    io->fd = fd;
    return 0;
}

int io_flush(io_buf * io){
    int i = 0, vernier;
    vernier = io->vernier;
    for(i=0; i<io->len; i++){
        io->s[i] = io->s[i+vernier];
    }
    io->vernier = 0;
    i = read(io->fd, (io->s + vernier), (IO_MAX_BUF - io->len - 1));
    io->len += i;
    return 0;
}