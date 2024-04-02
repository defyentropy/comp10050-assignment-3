#include <stdio.h>
#include <string.h>
#include "../include/board.h"
#include "../include/input.h"
#include "../include/logging.h"

// Board Menu Function

void BoardMenu(BoardNodePtr *startPtr)
{
    long option;

    printf("\n\nOptions:\n");
    printf("1. Edit the name of a list\n");
    printf("2. Add a new list\n");
    printf("3. Delete a list\n");
    printf("4. Return to main menu\n");

    while (1)
    {
        printf("Enter your option: ");
        
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
                    printf("\nEnter the name of the list to edit: ");
                    if((fngets(target, 80)) == 1){       
					    printLog('i', "You Cancelled\n\n"); 
					    enterToContinue();
					    printf("\e[1;1H\e[2J");      
                         break;
                    }
                    targetPtr = searchByListName(*startPtr, target);

                    if (targetPtr == NULL)
                    {
                        printLog('i', "Can't find that list. Have you spelled it right?\n");
                    }
                    else
                    {
                        printf("\nEnter new name for the list \"%s\":\n", target);
                        fngets(target, 80);       
                        strcpy(targetPtr->listName, target);
                        printf("\e[1;1H\e[2J");
                        // < Correct Call to displayBoard >
                        printLog('s', "List name changed.\n\n");
                        // < Correct Call to boardMenu >
                    }
                    break;
                }

            case 2:
                {
                    printf("\nEnter the name of the new list: ");
                    if((fngets(target, 80)) == 1){       
					    printLog('i', "You Cancelled\n\n"); 
					    enterToContinue();
					    printf("\e[1;1H\e[2J");      
                         break;
                    }
                    if (insertList(startPtr, target) == 0)
                    {
                        printf("\e[1;1H\e[2J");
                        // < Correct Call to displayBoard >
                        printLog('s', "New list created.\n\n");
                        // < Correct Call to boardMenu >
                    }
                    break;
                }

            case 3:
                {
                    printf("\nEnter the name of the list to delete: ");
                    if((fngets(target, 80)) == 1){       
					    printLog('i', "You Cancelled\n\n"); 
					    enterToContinue();
					    printf("\e[1;1H\e[2J");      
                         break;
                    }
                    targetPtr = searchByListName(*startPtr, target);

                    if (targetPtr == NULL)
                    {
                        printLog('i', "Can't find that list. Have you spelled it right?\n");
                    }
                    else
                    {
                        removeList(targetPtr);
                        printf("\e[1;1H\e[2J");
                        // < Correct Call to displayBoard >
                        printLog('s', "List deleted.\n\n");
                        // < Correct Call to Board Menu >
                    }
                    break;
                }
            case 4:
                {
                    printf("\e[1;1H\e[2J");
                    return;
                }
        }
    }
}


// List Menu Function

void listMenu(ListNodePtr *startPtr)
{
    long option;

    printf("\n\nOptions:\n");
    printf("1. Edit the name of an item\n");
    printf("2. Add a new item\n");
    printf("3. Delete an item\n");
    printf("4. Return to main menu\n");

    while (1)
    {
        printf("Enter your option: ");
        
        do
        {
            getNum(&option);
        } while (option < 1 && option > 4);

        switch (option)
        {
            char target[80];
            ListNodePtr targetPtr;

            case 1:
                {
                    printf("\nEnter the name of the item to edit: ");
                    if((fngets(target, 80)) == 1){       
					    printLog('i', "You Cancelled\n\n"); 
					    enterToContinue();
					    printf("\e[1;1H\e[2J");      
                        break;
                        }
                    targetPtr = searchByListItemName(*startPtr, target);

                    if (targetPtr == NULL)
                    {
                        printLog('i', "Can't find that item. Have you spelled it right?\n");
                    }
                    else
                    {
                        printf("\nEnter new name for the item \"%s\": ", target);
                        
                        if((fngets(target, 80)) == 1){       
					    printLog('i', "You Cancelled\n\n"); 
					    enterToContinue();
					    printf("\e[1;1H\e[2J");      
                        break;
                        }
                        strcpy(targetPtr->listItem, target);
                        printf("\e[1;1H\e[2J");
                        // < Correct Call to displayBoard >
                        printLog('s', "Item name changed.\n\n");
                        // < Correct Call to listMenu >
                    }
                    break;
                }

            case 2:
                {
                    printf("\nEnter the name of the new item: ");
                    if((fngets(target, 80)) == 1){       
					    printLog('i', "You Cancelled\n\n"); 
					    enterToContinue();
					    printf("\e[1;1H\e[2J");      
                         break;
                    }
                    if (insertListItem(startPtr, target) == 0)
                    {
                        printf("\e[1;1H\e[2J");
                        // < Correct Call to displayBoard >
                        printLog('s', "New item created.\n\n");
                        // < Correct Call to listMenu >
                    }
                    break;
                }

            case 3:
                {
                    printf("\nEnter the name of the item to delete: ");
                    if((fngets(target, 80)) == 1){       
					    printLog('i', "You Cancelled\n\n"); 
					    enterToContinue();
					    printf("\e[1;1H\e[2J");      
                         break;
                    }
                    targetPtr = searchByListItemName(*startPtr, target);

                    if (targetPtr == NULL)
                    {
                        printLog('i', "Can't find that item. Have you spelled it right?\n");
                    }
                    else
                    {
                        removeListItem(targetPtr);
                        printf("\e[1;1H\e[2J");
                        // < Correct Call to displayBoard >
                        printLog('s', "Item deleted.\n\n");
                        // < Correct Call to listMenu >
                    }
                    break;
                }
            case 4:
                {
                    printf("\e[1;1H\e[2J");
                    return;
                }
        }
    }
}
