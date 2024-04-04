#ifndef  __LIST_H__
#define  __LIST_H__

#include <stddef.h>

typedef int val_t;

struct Data
{
    char *str;
    int   strLength;

    val_t value;
};



typedef Data elem_t;

const int ListReallocRate =  2;

#define LIST_DUMP(list) listDump(list, __FILE__,  __LINE__, __func__)

struct ListNode
{
    elem_t data;
    int    next;
    int    prev;
};

/**
 * @brief a structure that contains the buffers of list and its parameters
 * 
 */
struct List
{
    int size;
    int capacity;

    int freeHead;

    ListNode *nodes;
};

enum listErrors
{
    LIST_MEMORY_ERROR   = -2,
    LIST_NODES_NULL     = -3,
    

    LIST_OVERFLOW       = -5,
    LIST_BAD_VARIABLES  = -6,
    LIST_BAD_POINTERS   = -7,
    LIST_INVALID_INDEX  = -8,
    LIST_BAD_CAPACITY   = -9
};

void dataFprintf(const Data *data, FILE *file);

void dataAssign(Data *data, const char *key, val_t val);

void dataDtor  (Data *data);

void dataCopy  (Data *dst, const Data *src);

int listFindKey(List *list, const char *key);

/**
 * @brief creates a List object and returns a pointer to it
 * 
 * @param [in] capacity the capacity of the newly created List
 * @return List* pointer to the created List
 */
List *listCtor(int capacity);

/**
 * @brief destroys the List object
 * 
 * @param [in,out] listPtr a pointer to the List* variable, which points to the List to destroy 
 * @return int 
 */
int listDtor (List **listPtr);
/**
 * @brief determines if an error occurred to the List
 * 
 * @param [in] list a pointer to the List to verify
 * @return 0 if no errors occurred
 * @return negative int value (error code) if an error occurred
 */
int listError(List *list);
/**
 * @brief writes to logfile all the info about the List
 * 
 * @param [in] list a pointer to the List
 * @param [in] file a str with the name of the file from which listDump() is called
 * @param [in] line the number of line from which listDump() is called
 * @param [in] function a str with the name of function from which listDump() is called
 * 
 * @return 0 if no errors occurred
 * @return negative int value (error code) if an error occurred
 */
int listDump (List *list, const char *file, int line, const char *function);



/**
 * @brief returns the capacity of List
 * 
 * @param [in] list a pointer to the List which capacity is to be determined
 * @return int the capacity 
 */
#define listCapacity(lst) \
        lst->capacity
//int listCapacity(List *list);

/**
 * @brief returns the size of List
 * 
 * @param [in] list a pointer to the List which size is to be determined
 * @return int the size 
 */
#define listSize(lst) \
        lst->size
//int listSize    (List *list);

/**
 * @brief returns the smallest index of a free element in arrays of the List
 * 
 * @param [in] list a pointer to the List which first free element is to be determined
 * @return int index of the first free element
 */
#define listFreeHead(lst) \
        lst->freeHead
//int listFreeHead    (List *list);

/**
 * @brief returns index of the head of the List
 * 
 * @param [in] list a pointer to the List which head is to be determined
 * @return int index of the head
 */
#define listHeadIndex(lst) \
        lst->nodes[0].next
//int listHeadIndex   (List *list);

/**
 * @brief returns index of the tail of the List
 * 
 * @param [in] list a pointer to the List which tail is to be determined
 * @return int index of the tail
 */
#define listTailIndex(lst) \
        lst->nodes[0].prev
//int listTailIndex   (List *list);


/**
 * @brief returns index of the next element in the List
 * 
 * @param [in] list a pointer to the List in which we determine next index
 * @param [in] arrayIndex index of element for which next is to be determined
 * @return int index of the next element
 */
#define listNextIndex(lst, arrIndex) \
        lst->nodes[arrIndex].next
//int listNextIndex   (List *list, int arrayIndex);


/**
 * @brief returns index of the previous element in the List
 * 
 * @param [in] list a pointer to the List in which we determine previous index
 * @param [in] arrayIndex index of element for which previous is to be determined
 * @return int index of the next element
 */
#define listPrevIndex(lst, arrIndex) \
        lst->nodes[arrIndex].prev
//int listPrevIndex   (List *list, int arrayIndex);
/**
 * @brief returns data of the given element in a List
 * 
 * @param [in] list a pointer to the List in which data is to be determined 
 * @param [in] arrayIndex index of the elem which data is to be determined 
 * @return elem_t data of the given element
 */
#define listValuePtrByIndex(lst, arrIndex) \
        &lst->nodes[arrIndex].data
//elem_t listValueByIndex(List *list, int arrayIndex);

/**
 * @brief adds a new element to List, it becomes new head
 * 
 * @param [in] list a pointer to the List in which to add new element
 * @param [in] value data of the new element
 * @return int index of the new element in arrays of the List
 */
int listPushFront(List *list, elem_t *value);
/**
 * @brief adds a new element to List, it becomes new tail
 * 
 * @param [in] list a pointer to the List in which to add new element
 * @param [in] value data of the new element
 * 
 * @return int index of the new element in arrays of the List
 * @return negative int if an error occurred
 */
int listPushBack (List *list, elem_t *value);
/**
 * @brief adds a new element to the List after the anchor element
 * 
 * @param [in] list a pointer to the List in which to add new element
 * @param [in] arrayAnchorIndex index of the element in arrays  of the List after which to add
 * @param [in] value data of the new element
 * 
 * @return int index of the new element in arrays of the List
 * @return negative int if an error occurred
 */
int listAddAfter(List *list, int arrayAnchorIndex, elem_t *value);
/**
 * @brief adds a new element to the List before the anchor element
 * 
 * @param [in] list a pointer to the List in which to add new element
 * @param [in] arrayAnchorIndex index of the element in arrays  of the List before which to add
 * @param [in] value data of the new element
 * 
 * @return int index of the new element in arrays of the List
 * @return negative int if an error occurred
 */
int listAddBefore(List *list, int arrayAnchorIndex, elem_t *value);
/**
 * @brief deletes an element by its index in arrays of the List
 * 
 * @param [in] list a pointer to the List in which to delete an element
 * @param [in] arrElemIndex index of the element to delete in arrays  of the List
 * 
 * @return int index of the deleted element
 * @return negative int if an error occurred
 */
int listDel      (List *list, int arrElemIndex);

/**
 * @brief increases capacity of the List multiplying it by reallocRate
 * 
 * @param [in] list a pointer to the List which capacity to increase
 * @param [in] reallocRate int constant by which to multiply the capacity
 * 
 * @return 0 if no error occurred
 * @return negative int (error code) if an error occurred
 */
int listReallocUp(List *list, int reallocRate);



#endif //__LIST_H__
