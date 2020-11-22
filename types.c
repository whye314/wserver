#include <string.h>
#include "types.h"

int addstr(strlist * head, strlist * node){
    head->fina->next = node;
    node->next = NULL;
    head->fina = node;
    return 0;
}

int http_slist_free(strlist * slkist){
    return 0;
}