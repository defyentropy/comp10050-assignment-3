// #include <stdio.h>
#include "../include/board.h"
// #include "../include/fileio.h"
// #include "../include/input.h"
#include "../include/boardMenu.h"

int main(void)
{
    BoardNodePtr startPtr = initialiseBoard();
    
    BoardMenu(&startPtr);
    displayBoard(startPtr);

    freeBoard(startPtr);
}
