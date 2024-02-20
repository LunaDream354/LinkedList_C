#include <stdio.h>
#include "linked_list.h"

char *list_text[]={
    "aweawe",
    "XDXDXDXD",
    "LULULULU"
};

int main(void){
    Linked_head *list = linked_create(NULL,false);
    for(size_t i = 0; i < sizeof(list_text)/sizeof(list_text[0]);i++){
        linked_push(list,list_text[i]);
    }
    for(size_t i = 0; i < list->size; i++){
        printf("%s\n",(char*)linked_get_at(list,i));
    }
    return 0;
}