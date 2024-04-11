#include <stdio.h>
#include <string.h>
#include "../include/board.h"
#include "../include/fileio.h"
#include "../include/input.h"
#include "../include/logging.h"
#include "../include/menu.h"

void displayMenu();

int main(void)
{
    long choice;
    char fileName[80];
    BoardNodePtr startPtr = initialiseBoard();

    // loop until the user manually exits the application using the 6th
    // option
    while (1)
    {
        clearScreen();
		displayMenu();

        do
        {
            printf("\nEnter your choice (1-6): ");
            getNum(&choice);
        } while(choice < 1 || choice > 6);

        // remove the main menu from the screen, because other menus might
        // have to be displayed
        clearScreen(); 
        switch (choice)
        {    
            case 1:
                {
                    displayBoard(startPtr, 1);
                    enterToContinue();
                    break;
                }
            case 2:
                {
                    // repeatedly ask for input until the user either enters
                    // a valid input or they enter a blank line, indicating
                    // that they would like to cancel the operation
                    // this is an input pattern that will be repeated many,
                    // many times throughout this application
                    while (1)
                    {
                        printf("Enter the name of the file to load, or leave blank to cancel: ");

                        if((fngets(fileName, 80)) == 1)
                        {    
                            printLog('i', "File loading cancelled.\n\n"); 
                            break;
                        }
                        else if (readFromFile(fileName, &startPtr) == 0)
                        {
                            enterToContinue();
                            break;
                        }
                    }

                    break;
                }
            case 3:
                {
                    char listName[80];
                    BoardNodePtr targetPtr;

                    // display the board in a condensed format (just the list
                    // names) so users don't have to struggle to remember the 
                    // names of the lists they're working with
                    displayBoard(startPtr, 0);

                    while (1)
                    {
                        printf("\nEnter the name of the list to edit, or leave blank to cancel: ");

                        if((fngets(listName, 80)) == 1)
                        { 
                            printLog('i', "List edit cancelled.\n\n");
                            break;
                        }
                        else if ((targetPtr = searchByListName(startPtr, listName)) == NULL)
                        {
                            printLog('w', "That list doesn't exist. Are you sure you spelled it right?\n\n");
                        }
                        else
                        {
                            break;
                        }
                    }

                    // if they haven't cancelled
                    if (listName[0] != '\0')
                    {
                        clearScreen();
                        cprintf(37, "%s:\n", targetPtr->listName);
                        listMenu(targetPtr, &startPtr);
                    }

                    break;
                }
            case 4:
                {
                    clearScreen();
                    boardMenu(&startPtr);
                    break;
                }
            case 5:
                printf("Enter the name of the file to save to, or leave blank to cancel: ");
                
                if ((fngets(fileName, 80)) == 1)
                {       
					printLog('i', "Save to file cancelled.\n\n"); 
					enterToContinue();
                    break;
                }
                else if (saveToFile(fileName, startPtr) == 0)
                {
                    printLog('s', "File saved successfully.\n\n");
                    enterToContinue();
                }

                break;
            case 6:
                {
                    freeBoard(&startPtr);
                    return 0;
                }
        }
    }
}

void displayMenu()
{
    printf("Menu:\n");
    printf("1. Display board\n");
    printf("2. Load board from a file\n");
    printf("3. Edit list\n");
    printf("4. Edit Board\n");
    printf("5. Save board\n");
    printf("6. Quit\n");
}
