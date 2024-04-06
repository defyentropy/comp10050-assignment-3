#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        printf("    EMPTY LIST\n");
        return;
    }

    ListNodePtr currentPtr = startPtr;
    while (currentPtr != NULL)
    {
        cprintf(37, "    • ");
        printf("%s\n", currentPtr->listItem);
        currentPtr = currentPtr->nextPtr;
    }
}

void displayBoard(BoardNodePtr startPtr, int displayExpanded)
{
    if (startPtr == NULL)
    {
        printLog('i', "The board is currently empty.\n\n");
        return;
    }

    BoardNodePtr currentPtr = startPtr;
    while (currentPtr != NULL)
    {
        cprintf(37 , "%s", currentPtr->listName);

        if (displayExpanded != 0)
        {
            printf(":\n");
            displayList(currentPtr->startPtr);
            printf("\n");
        }
        else
        {
            printf("\n");
        }

        currentPtr = currentPtr->nextPtr;
    }
}

int insertListItem(ListNodePtr *startPtr, char listItem[80])
{
    ListNodePtr newListItem = malloc(sizeof(ListNode));
    if (newListItem == NULL)
    {
        printLog('e', "Could not allocate enough memory for a new list item.\n");
        return 1;
    }

    newListItem->nextPtr = *startPtr;
    newListItem->prevPtr = NULL;
    strcpy(newListItem->listItem, listItem);

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

void freeBoard(BoardNodePtr *startPtr)
{
    if (*startPtr == NULL)
    {
        return;
    }

    BoardNodePtr currentPtr = *startPtr;
    while (currentPtr->nextPtr != NULL)
    {
        currentPtr = currentPtr->nextPtr;
        BoardNodePtr prevNode = currentPtr->prevPtr;
        freeListItems(prevNode->startPtr);
        free(prevNode);
    }
    freeListItems(currentPtr->startPtr);
    free(currentPtr);

    *startPtr = NULL;
}

int insertList(BoardNodePtr *startPtr, char listName[80])
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
    strcpy(newList->listName, listName);

    if (*startPtr != NULL)
    {
        (*startPtr)->prevPtr = newList;
    }
    *startPtr = newList;

    return 0;
}

BoardNodePtr searchByListName(BoardNodePtr startPtr, char *listName)
{
    BoardNodePtr currentPtr = startPtr;
    while (currentPtr != NULL && strcmp(currentPtr->listName, listName) != 0)
    {
        currentPtr = currentPtr->nextPtr;
    }

    return currentPtr;
}

ListNodePtr searchByListItemName(ListNodePtr startPtr, char *listItem)
{
    ListNodePtr currentPtr = startPtr;
    while (currentPtr != NULL && strcmp(currentPtr->listItem, listItem) != 0)
    {
        currentPtr = currentPtr->nextPtr;
    }

    return currentPtr;
}

void removeList(BoardNodePtr *startPtr, BoardNodePtr targetPtr)
{
        BoardNodePtr prevList = targetPtr->prevPtr;
        BoardNodePtr nextList = targetPtr->nextPtr;
        
        if (prevList != NULL)
        {
            prevList->nextPtr = nextList;
        }
        else
        {
            *startPtr = nextList;
        }

        if (nextList != NULL)
        {
            nextList->prevPtr = prevList;
        }

        freeListItems(targetPtr->startPtr);
        free(targetPtr);
}

void removeListItem(ListNodePtr *startPtr, ListNodePtr targetPtr)
{
    ListNodePtr prevListItem = targetPtr->prevPtr;
    ListNodePtr nextListItem = targetPtr->nextPtr;

    if (prevListItem != NULL)
    {
        prevListItem->nextPtr = nextListItem;
    }
    else
    {
        *startPtr = nextListItem;
    }

    if (nextListItem != NULL)
    {
        nextListItem->prevPtr = prevListItem;
    }

    free(targetPtr);
}
