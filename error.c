#include "log.h"

int error(int erron){
    error_log(erron);
    //exit(erron);
    return erron;
}