#include <stdio.h>
#include "../include/board.h"
#include "../include/fileio.h"
#include "../include/input.h"

int main(void)
{
    BoardNodePtr startPtr = initialiseBoard();
    /* char listName[80];
    char listItem[80];

    printf("Enter the new list name:\n");
    fngets(listName, 80);
    insertList(&startPtr, listName);

    printf("Enter the new list name:\n");
    fngets(listName, 80);
    insertList(&startPtr, listName);

    printf("Enter the new list item:\n");
    fngets(listItem, 80);
    insertListItem(&(startPtr->startPtr), listItem);

    printf("Enter the new list item:\n");
    fngets(listItem, 80);
    insertListItem(&(startPtr->startPtr), listItem); */
    
    readFromFile("shoppinglist.csv", &startPtr); 
    displayBoard(startPtr);

    // saveToFile(startPtr);

    freeBoard(startPtr);
}
