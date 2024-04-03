#include <stdio.h>

#include "../include/list.h" 

int main()
{
    List *lst = listCtor(2);

    LIST_DUMP(lst);

    listPushBack(lst, 3);
    LIST_DUMP(lst);

    listPushFront(lst, 6);
    LIST_DUMP(lst);

    listAddAfter(lst, 2, 7);
    LIST_DUMP(lst);

    listDel(lst, 3);
    LIST_DUMP(lst);

    listPushFront(lst, 18);
    LIST_DUMP(lst);

    listPushBack(lst, 19);
    LIST_DUMP(lst);

    listAddBefore(lst, 4, 23);
    LIST_DUMP(lst);

    listDel(lst, 3);
    LIST_DUMP(lst);

    listDtor(&lst);
    printf("list after destruction = %p\n", lst);
}
