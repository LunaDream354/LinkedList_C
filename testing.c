#include <stdio.h>
#include "linked_list.h"

int main(){
    Linked_head *list = linked_create(NULL,false);
    linked_push(list,NULL);
    return 0;
}