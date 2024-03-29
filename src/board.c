#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/input.h"
#include "../include/logging.h"
#include "../include/board.h"

BoardNodePtr initialiseBoard()
{
    return NULL;
}

void displayList(ListNodePtr startPtr)
{
    if (startPtr ==  NULL)
    {
        return;
    }

    ListNodePtr currentPtr = startPtr;
    while (currentPtr->nextPtr != NULL)
    {
        printf("-> %s\n", currentPtr->listItem);
    
        currentPtr = currentPtr->nextPtr;
    }
    printf("-> %s\n", currentPtr->listItem);
}

void displayBoard(BoardNodePtr startPtr)
{
    if (startPtr == NULL)
    {
        return;
    }

    BoardNodePtr currentPtr = startPtr;
    while (currentPtr->nextPtr != NULL)
    {
        printf("%s\n", currentPtr->listName);
        displayList(currentPtr->startPtr);

        currentPtr = currentPtr->nextPtr;
    }
    printf("%s\n", currentPtr->listName);
    displayList(currentPtr->startPtr);
}

int insertListItem(ListNodePtr *startPtr)
{
    ListNodePtr newListItem = malloc(sizeof(ListNode));
    if (newListItem == NULL)
    {
        printLog('e', "Could not allocate enough memory for a new list item.\n");
        return 1;
    }

    newListItem->nextPtr = *startPtr;
    newListItem->prevPtr = NULL;
    printf("Enter the name of the new item:\n");
    fngets(newListItem->listItem, 80);

    if (*startPtr != NULL)
    {
        (*startPtr)->prevPtr = newListItem;
    }
    *startPtr = newListItem;
    return 0;
}

void freeListItems(ListNodePtr startPtr)
{
    // if the list is already empty, then there is nothing to be freed
    if (startPtr == NULL)
    {
        return;
    }

    ListNodePtr currentPtr = startPtr;
    while (currentPtr->nextPtr != NULL)
    {
        currentPtr = currentPtr->nextPtr;
        free(currentPtr->prevPtr);
    }
    free(currentPtr);
}

void freeBoard(BoardNodePtr startPtr)
{
    if (startPtr == NULL)
    {
        return;
    }

    BoardNodePtr currentPtr = startPtr;
    while (currentPtr->nextPtr != NULL)
    {
        currentPtr = currentPtr->nextPtr;
        BoardNodePtr prevNode = currentPtr->prevPtr;
        freeListItems(prevNode->startPtr);
        free(prevNode);
    }
    freeListItems(currentPtr->startPtr);
    free(currentPtr);
}

int insertList(BoardNodePtr *startPtr)
{
    // allocate new node
    BoardNodePtr newList = malloc(sizeof(BoardNode));
    if (newList == NULL)
    {
        printLog('e', "Could not allocate enough memory for a new list.\n");
        return 1;
    }

    // since these lists don't need to be stored in order, we're inserting
    // them at the very beginning of the list for optimal time complexity
    newList->nextPtr = *startPtr;
    newList->prevPtr = NULL;
    newList->startPtr = NULL;
    printf("Enter the name of the new list:\n");
    fngets(newList->listName, 80); 

    if (*startPtr != NULL)
    {
        (*startPtr)->prevPtr = newList;
    }
    *startPtr = newList;

    return 0;
}

int removeList(BoardNodePtr startPtr, char listName[80])
{
    if (startPtr == NULL)
    {
        printLog('e', "The list is already empty.\n");
        return 1;
    }

    BoardNodePtr currentPtr = startPtr;
    while (currentPtr->nextPtr != NULL && strcmp(currentPtr->listName, listName) != 0)
    {
        currentPtr = currentPtr->nextPtr;
    }

    if (currentPtr->nextPtr == NULL)
    {
        printLog('e', "The board does not contain a list with that name. Are you sure you spelled it correctly?\n");
        return 1;
    }
    else
    {
        BoardNodePtr prevNode = currentPtr->prevPtr;
        prevNode->nextPtr = currentPtr->nextPtr;
        freeListItems(currentPtr->startPtr);
        free(currentPtr);

        printLog('s', "List successfully deleted.\n");
        return 0;
    }
}
