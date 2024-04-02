#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/logging.h"

// fngets is essentially a stricter version of fgets, in that it detects if the
// user has entered more characters than expected, clears the stdin buffer, and
// then re-prompts them until they enter a valid input
int fngets(char *dest, int length)
{
    // NOTE: This function is reused from Hari Mohan's assignment 1

    // length + 1 to account for \n character
    char inputBuffer[length + 1]; // string to read input into
    char *lineFeed;               // pointer to newline character

    while (true)
    {
        printf("> ");
        fgets(inputBuffer, length + 1, stdin);

        // strchr is used to search for the location of a character in a string
        // it returns a pointer to the character if it exists, else NULL
        lineFeed = strchr(inputBuffer, '\n');

        // If there isn't a newline in the string, that means that the user
        // has typed in more than the allowed number of characters, and fgets
        // has copied in only part of it.
        if (lineFeed == NULL)
        {
            // Warn the user
            printLog('w', "Your input has exceeded the maximum length of %d. Please try again.\n\n", length - 1);

            // Flush stdin by fgets-ing until we find the newline
            while (!strchr(inputBuffer, '\n'))
            {
                fgets(inputBuffer, length, stdin);
            }
        }
        else
        {
            *lineFeed = '\0'; // remove the newline
            strcpy(dest, inputBuffer);

            // these return status codes are used to detect in the main
            // program if the user wants to cancel an input operation
            if (lineFeed == inputBuffer)
            // i.e. if the user has entered a blank input
            {
                return 1;
            }
            else
            // The user has entered valid input
            {
                return 0;
            }
        }
    }
}

// function modified from https://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html
// I emailed Abey and he said it was okay as long as I cited the source
// this function safely obtains integer input from the user, without the risk of
// buffer overflow ocurring (which can happen with scanf)
int getNum(long *dest)
{
    long result;
    // it's a bit disruptive to see WARNING's about input length, so the input
    // buffer is large enough to hold any reasonable number, and error
    // checking is left to strtol
    char inputBuffer[128]; 
    int valid;            // flag for successful conversion

    do
    {
        // does not accept blank input
        while (fngets(inputBuffer, 128) == 1)
        {
            continue;
        }

        char *endptr;

        // errno is an enum variable defined in <errno.h> that contains data
        // about runtime errors in the program so different parts of the code
        // can perform error-handling in a uniform way
        errno = 0; // reset error number

        // strtol converts a string to a long
        result = strtol(inputBuffer, &endptr, 10);

        if (errno == ERANGE)
        {
            // ERANGE signifies that the number in the string is either too big
            // or too small to be stored in a long
            printf("Sorry, this number is too small or too large.\n");
            valid = 0;
        }
        else if (endptr == inputBuffer)
        {
            // no character was read
            valid = 0;
        }
        else if (*endptr != '\0' && *endptr != '\n')
        {
            // checks if strtol has read the entire string 
            valid = 0;
        }
        else
        {
            valid = 1;
        }
    } while (!valid); // repeat until we get a valid number

    *dest = result;
    return 0;
}

// obtains y/n input from the user.
int  getYesOrNo(void)
{
    char yesOrNo[128];

    // prompt the user for an answer until they enter either y or n
    do
    {
        while(fngets(yesOrNo, 128) == 1)
        {
            continue;
        }
    } while (strlen(yesOrNo) != 1 || (yesOrNo[0] != 'n' && yesOrNo[0] != 'y'));

    if (yesOrNo[0] == 'y')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



void enterToContinue(void){
    printf("Enter to Continue: ");
    char dummy[1024]; 
    fngets(dummy, sizeof(dummy));
    
}