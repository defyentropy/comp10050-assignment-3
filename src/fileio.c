#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/input.h"
#include "../include/board.h"
#include "../include/logging.h"

int csvParseString(FILE *fPtr, char *dest, char *thisChar)
{
    int charsWritten = 0;
    int charsRead = 0;
    int quoteOpened = 0;
    int lastCharacterWasQuote = 0;
    int malformedField = 0;

    if (!feof(fPtr))
    {
        *thisChar = fgetc(fPtr);
        charsRead++;
    }
    while (!feof(fPtr))
    {
        if (charsWritten > 79)
        {
            malformedField = 1;
            break;
        }
        else if (*thisChar == '"')
        {
            if (charsRead == 1)
            {
                quoteOpened = 1;
            }
            else if (!quoteOpened)
            {
                malformedField = 1;
                break;
            }
            else if (lastCharacterWasQuote)
            {
                dest[charsWritten++] = *thisChar;
                lastCharacterWasQuote = 0;
            }
            else
            {
                lastCharacterWasQuote = 1;
            }
        }
        else
        {
            if ((lastCharacterWasQuote || !quoteOpened)  && (*thisChar == ',' || *thisChar == '\n'))
            {
                break;
            }
            else if (lastCharacterWasQuote)
            {
                malformedField = 1;
                break;
            }
            else if (!quoteOpened && *thisChar == ' ')
            {
                malformedField = 1;
                break;
            }
            else
            {
                dest[charsWritten++] = *thisChar;
            }
        }

        *thisChar = fgetc(fPtr);
        charsRead++;
    }
    dest[charsWritten++] = '\0';

    if (feof(fPtr))
    {
        if (lastCharacterWasQuote)
        {
            return 0;
        }
        else if (charsWritten > 1)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    else if (malformedField || charsWritten == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int csvParseLine(FILE *fPtr, char *listName, char *listItem)
{
    int lineStatus = 0;
    char thisChar;

    lineStatus = csvParseString(fPtr, listName, &thisChar);

    if (!lineStatus)
    {
        if (thisChar != ',')
        {
            lineStatus = 1;
        }

        lineStatus = csvParseString(fPtr, listItem, &thisChar);

        if (thisChar != '\n')
        {
            lineStatus = 1;
        }
    }

    return lineStatus;
}

int readFromFile(char *fileName, BoardNodePtr *startPtr)
{
    FILE *fPtr;
    int lineNum = 0;
    int err = 0;
    int lineStatus = 0;
    char *listName = malloc(80), *listItem = malloc(80);

    if ((fPtr = fopen(fileName, "r")) == NULL)
    {
        printLog('e', "Could not open file.\n");
        return 1;
    }

    while(!feof(fPtr))
    {
        lineNum++;
        lineStatus = csvParseLine(fPtr, listName, listItem);

        if (lineStatus == 0)
        {
            BoardNodePtr listPtr = searchByListName(*startPtr, listName);
            if (listPtr == NULL)
            {
                if (insertList(startPtr, listName) == 0)
                {
                    listPtr = *startPtr;
                }
                else
                {
                    err = 2;
                    break;
                }
            }

            if (insertListItem(&(listPtr->startPtr), listItem) == 1)
            {
                err = 2;
                break;
            }
        }
        else if (lineStatus == 1) 
        {
                printLog('e', "Error parsing CSV on line %d.\n", lineNum);
                err = 1;
                break;
        }
    }

    free(listName);
    free(listItem);
    fclose(fPtr);

    return err;
}

int containsSpecialCharacters(char *str)
{
    int strLen = strlen(str);
    int specialCharsPresent = 0;

    for (int i = 0; i < strLen; i++)
    {
        if (str[i] == ',' || str[i] == '"' || str[i] == ' ')
        {
            specialCharsPresent = 1;
            break;
        }
    }

    return specialCharsPresent;
}

char *csvNormaliseString(char *plainString)
{
    int i, j;
    int strLen = strlen(plainString);
    char *normalisedString = malloc(2 * strLen + 3);

    normalisedString[0] = '"';
    for (i = 0, j = 1; plainString[i] != '\0'; i++)
    {
        if (plainString[i] == '"')
        {
            normalisedString[j++] =  '"';
        }
        normalisedString[j++] = plainString[i];
    }
    normalisedString[j++] = '"';
    normalisedString[j++] = '\0';

    return normalisedString;
}

int saveToFile(BoardNodePtr startPtr)
{
    if (startPtr == NULL)
    {
        printLog('i', "The board is empty. There is nothing to write to file.\n");
        return 1;
    }

    char fileName[84];
    FILE *fPtr;

    printf("What is the name of the file you want to write to?\n");
    printf("(Please do not include the filename)\n");
    fngets(fileName, 80);

    if ((fPtr = fopen(fileName, "w")) == NULL)
    {
        printLog('e', "Could not open file.\n");
        return 2;
    }

    BoardNodePtr currentListPtr = startPtr;
    while (currentListPtr != NULL)
    {
        ListNodePtr currentListItemPtr = currentListPtr->startPtr;
        while (currentListItemPtr != NULL)
        {
            if (containsSpecialCharacters(currentListPtr->listName))
            {
                char *normalisedString = csvNormaliseString(currentListPtr->listName);
                fprintf(fPtr, "%s,", normalisedString);
                free(normalisedString);
            }
            else
            {
                fprintf(fPtr, "%s,", currentListPtr->listName);
            }

            if (containsSpecialCharacters(currentListItemPtr->listItem))
            {
                char *normalisedString = csvNormaliseString(currentListItemPtr->listItem);
                fprintf(fPtr, "%s\n", normalisedString);
                free(normalisedString);
            }
            else
            {
                fprintf(fPtr, "%s\n", currentListItemPtr->listItem);
            }

            currentListItemPtr = currentListItemPtr->nextPtr;
        }

        currentListPtr = currentListPtr->nextPtr;
    }

    fclose(fPtr);
    return 0;
}
