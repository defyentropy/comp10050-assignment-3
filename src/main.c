#include <stdio.h>
#include "../include/board.h"
#include "../include/fileio.h"
#include "../include/input.h"
#include "../include/logging.h"


int main(void)
{

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
    
    // Main Menu Display

	long choice = 0; // Initialize choice to a value outside the menu options


	        displayMenu();
	        printf("Enter your choice: ");

	        if (getNum(&choice) != 0) {
	            // If getNum returns a non-zero value, it means the user canceled the input operation
	            // In this case, we don't want to proceed further
	            printf("\nOperation canceled.\n");
	        }

	        switch (choice) {
	            case 1: {
	                cprintf(2, "\nDisplaying board...\n\n"); // Print in green color
	                // Call function to display board
	                BoardNodePtr startPtr = initialiseBoard();

	                if (readFromFile("example.csv", &startPtr) == 0)
	                {
	                    displayBoard(startPtr);
	                }
	                freeBoard(startPtr);
	                break;
	            }
	            case 2:
	                cprintf(2, "\nLoading board from a file...\n\n"); // Print in green color
	                // Call function to load board from file
	                break;
	            case 3:
	                cprintf(2, "\nEditing list...\n\n"); // Print in green color
	                // Call function to edit list
	                break;
	            case 4:
	                cprintf(2, "\nEditing Board...\n\n"); // Print in green color
	                // Call function to edit board
	                break;
	            case 5:
	                cprintf(2, "\nSaving board...\n\n"); // Print in green color
	                // Call function to save board
	                break;
	            case 6:
	                cprintf(2, "\nExiting...\n\n"); // Print in green color
	                break;
	            default:
	                printf("\nInvalid choice. Please enter a number between 1 and 6.\n\n");
	                break;
	        }

	        // Options to return to menu or edit to be implemented


	    return 0;
	}

>>>>>>> main

