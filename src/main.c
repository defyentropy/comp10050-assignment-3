// #include <stdio.h>
#include "../include/board.h"

int main(void)
{
    BoardNodePtr startPtr = initialiseBoard();
    insertList(&startPtr);
    insertList(&startPtr);

    insertListItem(&(startPtr->nextPtr->startPtr));
    insertListItem(&(startPtr->nextPtr->startPtr));

    displayBoard(startPtr);

    freeBoard(startPtr);
}
