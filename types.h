




struct _strlist_struct{
    union{
        char * val;
        struct _strlist_struct * fina;
    };
    struct _strlist_struct * next;
};

typedef struct _strlist_struct strlist;

int addstr(strlist * head, strlist * node);
