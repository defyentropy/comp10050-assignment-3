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

   printf("\e[1;1H\e[2J");

    while (1)
    {
		 displayMenu();

        printf("Enter your choice: ");
        do {
            getNum(&choice);
        } while(choice < 1 && choice > 6);

        switch (choice)
        {
            case 1:
                {
                    printf("\e[1;1H\e[2J"); 
                    displayBoard(startPtr);
                    break;
                }
            case 2:
                {
                    printf("\nEnter the name of the file to load: ");
                    if((fngets(fileName, 80)) == 1){    
						printLog('i', "You Cancelled\n\n"); 
						enterToContinue();
						printf("\e[1;1H\e[2J");     
                        break;
                    }

                    printf("\e[1;1H\e[2J");
                    readFromFile(fileName, &startPtr);
                    break;
                }
            case 3:
                {
                    char listName[80];
                    BoardNodePtr targetPtr;
                    printf("\nEnter the name of the list to edit: ");
                    if((fngets(listName, 80)) == 1){ 
						printLog('i', "You Cancelled\n\n");
						enterToContinue();
						printf("\e[1;1H\e[2J");             
                        break;
                    }

                    if ((targetPtr = searchByListName(startPtr, listName)) == NULL)
                    {
                        printLog('w', "That list doesn't exist. Are you sure you spelled it right?\n\n");
						enterToContinue();
						printf("\e[1;1H\e[2J");
                    }
                    else
                    {
                        printf("\e[1;1H\e[2J");
                        displayBoard(startPtr);
                        listMenu(&(targetPtr->startPtr));
                    }
                    break;
                }
            case 4:
                {
                    printf("\e[1;1H\e[2J");
                    displayBoard(startPtr);
                    BoardMenu(&startPtr); 
                    break;
                }
            case 5:
                printf("\nEnter the name of the file to save to: ");
                if((fngets(fileName, 80)) == 1){       
					printLog('i', "You Cancelled\n\n"); 
					enterToContinue();
					printf("\e[1;1H\e[2J");      
                    break;
                    }
                printf("\e[1;1H\e[2J"); 
                printLog('s', "File saved successfully.\n\n");
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


