#include <stdio.h>
#include <string.h>
#include "../include/board.h"
#include "../include/input.h"
#include "../include/logging.h"

void boardMenu(BoardNodePtr *startPtr)
{
    long option;

    // Loops until the user chooses to exit the board menu with option 4

    while (1)
    {
        clearScreen();

        // Displays a board of just the names of each list

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

        // Clears the screen to allow the board to update once the choice
        // being executed in the switch statement has been run

        clearScreen();
        switch (option)
        {
            char target[80];
            BoardNodePtr targetPtr;

            case 1:
                {
                    while (1)
                    {
                        // Repeatedly runs until user enters a valid name i.e. an existing list name

                        printf("Enter the name of the list to edit, or leave blank to cancel: ");

                        if((fngets(target, 80)) == 1){       
                            printLog('i', "List edit cancelled.\n\n"); 
                            enterToContinue();              
                            break;
                        }
                        // Searches for the users input, and if it can not be found, an info warning is
                        // printed, prompting the user to check their input was valid

                        else if ((targetPtr = searchByListName(*startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that list. Have you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }

                    // If the input was valid, the user is asked to enter the new name

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

                    // Inserts a new list which the user named onto the board

                    else if (insertList(startPtr, target) == 0)
                    {
                        printLog('s', "New list created.\n\n");
                        enterToContinue();
                    }

                    break;
                }

            case 3:
                {
                    // Repeatedly runs until user enters a valid name i.e. an existing list name, similarly to case 1

                    while (1)
                    {
                        printf("Enter the name of the list to delete: ");

                        if((fngets(target, 80)) == 1){       
                            printLog('i', "You Cancelled\n\n"); 
                            enterToContinue();
                            break;
                        }
                        // Searches for the users input, and if it can not be found, an info warning is
                        // printed, prompting the user to check their input was valid

                        else if ((targetPtr = searchByListName(*startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that list. Are you sure you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }
                     // If the input was valid, the specified list is removed from the board
                    
                    if (target[0] != '\0')
                    {
                        removeList(startPtr, targetPtr);    
                        printLog('s', "List deleted.\n\n");     // Success message displayed notifying the user
                        enterToContinue();
                    }
                    break;
                }

            case 4:
                {
                    return;     // Exits from the board menu while loop, returning the user to the main menu
                }
        }
    }
}

void listMenu(BoardNodePtr nodePtr, BoardNodePtr *startPtr)
{
    long option;

    while (1)
    {

        // Clears screen of previous menus
        clearScreen();
        // Displays the chosen list name in light blue
        cprintf(37, "%s:\n", nodePtr->listName);
        // Displays the items in that list bellow
        displayList(nodePtr->startPtr);

        printf("\nOptions:\n");
        printf("1. Edit the name of an item\n");
        printf("2. Add a new item\n");
        printf("3. Delete an item\n");
        printf("4. Move an item to a different list\n");
        printf("5. Return to main menu\n");

        do
        {
            printf("\nEnter your option (1-5): ");
            getNum(&option);
        } while (option < 1 || option > 5);

        switch (option)
        {
            char target[80];
            ListNodePtr targetPtr;

            case 1:
                {
                    while (1)
                    {
                        // Repeatedly runs until user enters a valid name i.e. an existing item name

                        printf("\nEnter the name of the item to edit, or leave blank to cancel: ");
                        if ((fngets(target, 80)) == 1)
                        {       
                            printLog('i', "Item edit cancelled.\n\n"); 
                            enterToContinue();
                            break;
                        }
                        // Searches for the users input, and if it can not be found, an info warning is
                        // printed, prompting the user to check their input was valid

                        else if ((targetPtr = searchByListItemName(nodePtr->startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that item. Are you sure you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }

                     // If the input was valid, the user is asked to enter the new name

                    if (target[0] != '\0')
                    {
                        printf("\nEnter new name for item \"%s\": ", target);
                        while (fngets(target, 80) == 1)
                        {
                            continue;
                        }

                        strcpy(targetPtr->listItem, target);
                        printLog('s', "Item name changed.\n\n");  // Success message to notify the user
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
                    // Inserts a new iteam which the user named into the current list

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
                    // Repeatedly runs until user enters a valid name i.e. an existing item name
                    {
                        printf("\nEnter the name of the item to delete, or leave blank to cancel: ");
                        if ((fngets(target, 80)) == 1)
                        {       
                            printLog('i', "Delete item cancelled.\n\n"); 
                            enterToContinue();
                            break;
                        }
                        // Searches for the users input, and if it can not be found, an info warning is
                        // printed, prompting the user to check their input was valid

                        else if ((targetPtr = searchByListItemName(nodePtr->startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that item. Have you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }
                     // If the input was valid, the specified item is removed from the list

                    if (target[0] != '\0')
                    {
                        removeListItem(&(nodePtr->startPtr), targetPtr);
                        printLog('s', "Item deleted.\n\n");    // Success message
                        enterToContinue();
                    }
                    break;
                }

            case 4:
                {
                    char destBoard[80];
                    BoardNodePtr destBoardPtr;

                    while(1)
                    // Repeatedly runs until user enters a valid name i.e. an existing item name
                    {
                        printf("\nEnter the name of the item to move, or leave blank to cancel: ");
                        if (fngets(target, 80) == 1)
                        {
                            printLog('i', "Move item cancelled.\n\n");
                            enterToContinue();
                            break;
                        }
                        // Searches for the users input, and if it can not be found, an info warning is
                        // printed, prompting the user to check their input was valid

                        else if ((targetPtr = searchByListItemName(nodePtr->startPtr, target)) == NULL)
                        {
                            printLog('i', "Can't find that item. Are you sure you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (target[0] == '\0')    // if the user doesnt enter a name, break and go back to the list menu
                    {
                        break;
                    }

                    while (1)
                    {


                        printf("\nEnter the name of list to move this item to, or leave blank to cancel: ");
                        if (fngets(destBoard, 80) == 1)
                        {
                            printLog('i', "Move item cancelled.\n\n");
                            enterToContinue();
                            break;
                        }
                        // Searches for the users input, (in this case, the name of the target list)
                        // and if it can not be found, an info warning is printed
                        // prompting the user to check their input was valid

                        
                        else if ((destBoardPtr = searchByListName(*startPtr, destBoard)) == NULL)
                        {
                            printLog('i', "Can't find that list. Are you sure you spelled it right?\n");
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (destBoard[0] == '\0')  // if the user doesnt enter a name, break and go back to the list menu
                    {
                        break;
                    }

                    // Remove the item from its current position in the board
                    if (targetPtr->prevPtr != NULL)
                    {
                        targetPtr->prevPtr->nextPtr = targetPtr->nextPtr;
                    }
                    else
                    {
                        nodePtr->startPtr = targetPtr->nextPtr;  // Update the start pointer if the item is at the beginning
                    }

                    if (targetPtr->nextPtr != NULL)
                    {
                        targetPtr->nextPtr->prevPtr = targetPtr->prevPtr;
                    }

                    // Add the item to the beginning of the destination list

                    targetPtr->nextPtr = destBoardPtr->startPtr;
                    if (destBoardPtr->startPtr != NULL)
                    {
                        destBoardPtr->startPtr->prevPtr = targetPtr;
                    }
                    destBoardPtr->startPtr = targetPtr;

                    printLog('s', "Item successfully moved.\n");
                    enterToContinue();
                    break;
                }
            case 5:
                {
                    return;  // Exits from the list menu while loop, returning the user to the main menu
                }
        }
    }
}
