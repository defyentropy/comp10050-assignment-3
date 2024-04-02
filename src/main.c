#include <stdio.h>
#include "../include/board.h"
#include "../include/fileio.h"
#include "../include/input.h"
#include "../include/logging.h"
#include "../include/menu.h"

int main(void)
{
    long choice;
    char fileName[80];
    BoardNodePtr startPtr = initialiseBoard();

    displayMenu();

    while (1)
    {
        printf("Enter your choice: ");
        do {
            getNum(&choice);
        } while(choice < 1 && choice > 6);

        switch (choice)
        {
            case 1:
                {
                    displayBoard(startPtr);
                    break;
                }
            case 2:
                {
                    printf("Enter the name of the file to load:\n");
                    fngets(fileName, 80);
                    readFromFile(fileName, &startPtr);
                    break;
                }
            case 3:
                {
                    char listName[80];
                    BoardNodePtr targetPtr;
                    printf("Enter the name of the list to edit:\n");
                    fngets(listName, 80);

                    if ((targetPtr = searchByListName(startPtr, listName)) == NULL)
                    {
                        printLog('w', "That list doesn't exist. Are you sure you spelled it right?\n");
                    }
                    else
                    {
                        listMenu(&(targetPtr->startPtr));
                    }
                    break;
                }
            case 4:
                {
                    BoardMenu(&startPtr); 
                    break;
                }
            case 5:
                printf("Enter the name of the file to save to:\n");
                fngets(fileName, 80);
                saveToFile(fileName, startPtr);
                break;
            case 6:
                {
                    freeBoard(&startPtr);
                    return 0;
                }
        }
    }
}
