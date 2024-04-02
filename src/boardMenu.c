#include <stdio.h>
#include <string.h>
#include "../include/board.h"
#include "../include/input.h"
#include "../include/logging.h"

void BoardMenu(BoardNodePtr *startPtr)
{
    long option;

    printf("Options:\n");
    printf("1. Edit the name of a list\n");
    printf("2. Add a new list\n");
    printf("3. Delete a list\n");
    printf("4. Return to main menu\n");

    while (1)
    {
        printf("Enter your option:\n");
        
        do
        {
            getNum(&option);
        } while (option < 1 && option > 4);

        switch (option)
        {
            char target[80];
            BoardNodePtr targetPtr;

            case 1:
                {
                    printf("Enter the name of the list to edit:\n");
                    fngets(target, 80);
                    targetPtr = searchByListName(*startPtr, target);

                    if (targetPtr == NULL)
                    {
                        printLog('i', "Can't find that list. Have you spelled it right?\n");
                    }
                    else
                    {
                        printf("Enter new name for the list \"%s\":\n", target);
                        fngets(target, 80);
                        strcpy(targetPtr->listName, target);
                        printLog('s', "List name changed.\n");
                    }
                    break;
                }

            case 2:
                {
                    printf("Enter the name of the new list:\n");
                    fngets(target, 80);
                    if (insertList(startPtr, target) == 0)
                    {
                        printLog('s', "New list created.\n");
                    }
                    break;
                }

            case 3:
                {
                    printf("Enter the name of the list to delete:\n");
                    fngets(target, 80);
                    targetPtr = searchByListName(*startPtr, target);

                    if (targetPtr == NULL)
                    {
                        printLog('i', "Can't find that list. Have you spelled it right?\n");
                    }
                    else
                    {
                        removeList(targetPtr);
                        printLog('s', "List deleted.\n");
                    }
                    break;
                }
            case 4:
                {
                    return;
                }
        }
    }
}
