#include <stdio.h>
#include <string.h>
#include "../include/board.h"
#include "../include/input.h"
#include "../include/logging.h"

void boardMenu(BoardNodePtr *startPtr)
{
    long option;

    while (1)
    {
        clearScreen();
        displayBoard(*startPtr, 0);
        
        printf("\nOptions:\n");
        printf("1. Edit the name of a list\n");
        printf("2. Add a new list\n");
        printf("3. Delete a list\n");
        printf("4. Return to main menu\n");

        do
        {
            printf("\nEnter your option (1-4): ");
            getNum(&option);
        } while (option < 1 || option > 4);

        clearScreen();
        switch (option)
        {
            char target[80];
            BoardNodePtr targetPtr;

            case 1:
                {
                    while (1)
                    {
                        printf("Enter the name of the list to edit, or leave blank to cancel: ");

                        if((fngets(target, 80)) == 1){       
                            printLog('i', "List edit cancelled.\n\n"); 
                            enterToContinue();
                            break;
                        }
                        else if ((targetPtr = searchByListName(*startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that list. Have you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }

                    printf("\nEnter new name for the list \"%s\": ", target);
                    while (fngets(target, 80) == 1)
                    {
                        continue;
                    }
                    strcpy(targetPtr->listName, target);

                    printLog('s', "List name changed.\n\n");
                    enterToContinue();
                    break;
                }

            case 2:
                {
                    printf("Enter the name of the new list: ");

                    if ((fngets(target, 80)) == 1)
                    {       
                        printLog('i', "New list creation cancelled.\n\n"); 
                        enterToContinue();
                        break;
                    }
                    else if (insertList(startPtr, target) == 0)
                    {
                        printLog('s', "New list created.\n\n");
                        enterToContinue();
                    }

                    break;
                }

            case 3:
                {
                    while (1)
                    {
                        printf("Enter the name of the list to delete: ");

                        if((fngets(target, 80)) == 1){       
                            printLog('i', "You Cancelled\n\n"); 
                            enterToContinue();
                            break;
                        }
                        else if ((targetPtr = searchByListName(*startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that list. Are you sure you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }
                    
                    if (target[0] != '\0')
                    {
                        removeList(startPtr, targetPtr);    
                        printLog('s', "List deleted.\n\n");
                        enterToContinue();
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

void listMenu(BoardNodePtr nodePtr)
{
    long option;

    while (1)
    {
        clearScreen();
        cprintf(37, "%s:\n", nodePtr->listName);
        displayList(nodePtr->startPtr);

        printf("\nOptions:\n");
        printf("1. Edit the name of an item\n");
        printf("2. Add a new item\n");
        printf("3. Delete an item\n");
        printf("4. Return to main menu\n");

        do
        {
            printf("\nEnter your option (1-4): ");
            getNum(&option);
        } while (option < 1 || option > 4);

        switch (option)
        {
            char target[80];
            ListNodePtr targetPtr;

            case 1:
                {
                    while (1)
                    {
                        printf("\nEnter the name of the item to edit, or leave blank to cancel: ");
                        if ((fngets(target, 80)) == 1)
                        {       
                            printLog('i', "Item edit cancelled.\n\n"); 
                            enterToContinue();
                            break;
                        }
                        else if ((targetPtr = searchByListItemName(nodePtr->startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that item. Are you sure you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (target[0] != '\0')
                    {
                        printf("\nEnter new name for item \"%s\": ", target);
                        while (fngets(target, 80) == 1)
                        {
                            continue;
                        }

                        strcpy(targetPtr->listItem, target);
                        printLog('s', "Item name changed.\n\n");
                        enterToContinue();
                    }
                    break;
                }

            case 2:
                {
                    printf("\nEnter the name of the new item, or leave blank to cancel: ");
                    if ((fngets(target, 80)) == 1)
                    {       
					    printLog('i', "Adding new item cancelled.\n\n"); 
					    enterToContinue();
                        break;
                    }
                    else if (insertListItem(&(nodePtr->startPtr), target) == 0)
                    {
                        printLog('s', "New item created.\n\n");
                        enterToContinue();
                    }
                    break;
                }

            case 3:
                {
                    while (1)
                    {
                        printf("\nEnter the name of the item to delete, or leave blank to cancel: ");
                        if ((fngets(target, 80)) == 1)
                        {       
                            printLog('i', "Delete item cancelled.\n\n"); 
                            enterToContinue();
                            break;
                        }
                        else if ((targetPtr = searchByListItemName(nodePtr->startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that item. Have you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (target[0] != '\0')
                    {
                        removeListItem(&(nodePtr->startPtr), targetPtr);
                        printLog('s', "Item deleted.\n\n");
                        enterToContinue();
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
