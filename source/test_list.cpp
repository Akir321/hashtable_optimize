#include <stdio.h>

#include "../include/list.h" 

int main()
{
    List *lst = listCtor(2);
    Data data = {};

    LIST_DUMP(lst);

    dataAssign(&data, "a", 1);
    printf("<%s>\n", data.str);
    dataFprintf(&data, stdout);
    printf("\n");
    listPushBack(lst, &data);
    printf("%s\n", lst->nodes[1].data.str);
    LIST_DUMP(lst);

    dataAssign(&data, "b", 2);
    listPushFront(lst, &data);
    LIST_DUMP(lst);

    dataAssign(&data, "c", 3);
    listAddAfter(lst, 2, &data);
    LIST_DUMP(lst);

    listDel(lst, 3);
    LIST_DUMP(lst);

    dataAssign(&data, "18", 4);
    listPushFront(lst, &data);
    LIST_DUMP(lst);

    dataAssign(&data, "19", 5);
    listPushBack(lst, &data);
    LIST_DUMP(lst);

    printf("key = %3s, index = %3d\n", "a", listFindKey(lst, "a"));
    printf("key = %3s, index = %3d\n", "19", listFindKey(lst, "19"));
    printf("key = %3s, index = %3d\n", "bb", listFindKey(lst, "bb"));

    dataAssign(&data, "23", 6);
    listAddBefore(lst, 4, &data);
    LIST_DUMP(lst);

    listDel(lst, 3);
    LIST_DUMP(lst);

    listDtor(&lst);
    printf("list after destruction = %p\n", lst);
}
