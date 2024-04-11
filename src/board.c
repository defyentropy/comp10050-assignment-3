#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/logging.h"
#include "../include/board.h"


BoardNodePtr initialiseBoard()
{
    return NULL;
}


// Function to display a list by taking a pointer to the start of a list and
// displaying its info
void displayList(ListNodePtr startPtr)
{
    if (startPtr ==  NULL)        // If the list is empty, notify the user
    {
        printf("    EMPTY LIST\n");
        return;
    }


    // Iterates through the list and prints each element / item
    ListNodePtr currentPtr = startPtr;
    while (currentPtr != NULL)
    {
        cprintf(37, "    • ");
        printf("%s\n", currentPtr->listItem);
        currentPtr = currentPtr->nextPtr;
    }
}


// Function to display the board by taking a pointer to the start of the board
// and uses a flag to check whether to display the info expanded
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
        cprintf(37 , "%s", currentPtr->listName);  // Printing list name in light blue

        if (displayExpanded != 0)
        {
            printf(":\n");
            displayList(currentPtr->startPtr); // Displays the list items
            printf("\n");
        }
        else
        {
            printf("\n");
        }

        currentPtr = currentPtr->nextPtr;
    }
}

// Function to add a new item to a list
// It allocates memory for a new list item, copies the item name into it, and inserts it at the beginning of the list

int insertListItem(ListNodePtr *startPtr, char listItem[80])
{
    ListNodePtr newListItem = malloc(sizeof(ListNode));  // allocate memory for the new item
    if (newListItem == NULL)
    {
        printLog('e', "Could not allocate enough memory for a new list item.\n");
        return 1;
    }

    newListItem->nextPtr = *startPtr;  // Initialises new list item
    newListItem->prevPtr = NULL;
    strcpy(newListItem->listItem, listItem);


    // update prevPtr of the next item if any
    if (*startPtr != NULL)
    {
        (*startPtr)->prevPtr = newListItem;
    }
    // Update startPtr to point to the new item
    *startPtr = newListItem;
    return 0;
}


// Function to free the memory allocated for each list item
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
        free(currentPtr->prevPtr);  // free memory for the previous item
    }
    free(currentPtr);  // Free memory for the last item in the list
}


// Function to free memory allocated for each list in the board
void freeBoard(BoardNodePtr *startPtr)
{
    // if the board is already empty, then there is nothing to be freed
    if (*startPtr == NULL)
    {
        return;
    }

    BoardNodePtr currentPtr = *startPtr;
    while (currentPtr->nextPtr != NULL)
    {
        currentPtr = currentPtr->nextPtr;
        BoardNodePtr prevNode = currentPtr->prevPtr;
        freeListItems(prevNode->startPtr); // Free memory for list items at the current board node/list
        free(prevNode); // Free memory for the previous board node
    }

    // Free memory for the list items at the last board node/ list
    freeListItems(currentPtr->startPtr);
    free(currentPtr);

    *startPtr = NULL; // Set startPtr to NULL
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

// Function to take a users inputted name for a list and iterate through the board
// comparing the names of each board node (list) with the specified name
BoardNodePtr searchByListName(BoardNodePtr startPtr, char *listName)
{
    BoardNodePtr currentPtr = startPtr;
    while (currentPtr != NULL && strcmp(currentPtr->listName, listName) != 0)
    {
        currentPtr = currentPtr->nextPtr;
    }

    return currentPtr;
}


// Function to take a users inputted name for an item  and iterate through the current list
// comparing the names of each list item with the specified name
ListNodePtr searchByListItemName(ListNodePtr startPtr, char *listItem)
{
    ListNodePtr currentPtr = startPtr;
    while (currentPtr != NULL && strcmp(currentPtr->listItem, listItem) != 0)
    {
        currentPtr = currentPtr->nextPtr;
    }

    return currentPtr;
}

// Function to remove a list from the board and update the pointers of neighboring
// lists, also frees all the memory allocated for board node (list) and its items
void removeList(BoardNodePtr *startPtr, BoardNodePtr targetPtr)
{
        BoardNodePtr prevList = targetPtr->prevPtr;
        BoardNodePtr nextList = targetPtr->nextPtr;
        
        // Update pointers of neighboring board nodes / lists
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

        freeListItems(targetPtr->startPtr); // Freeing memory allocated for the list items
        free(targetPtr); // Freeing memory allocated for the list itslef
}

// Function to remove a list item from the list and update the pointers of neighboring
// list items, also frees all the memory allocated for the list item being removed
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
