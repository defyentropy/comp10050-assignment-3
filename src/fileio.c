#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/input.h"
#include "../include/board.h"
#include "../include/logging.h"

int containsSpecialCharacters(char *str)
{
    int strLen = strlen(str);
    int specialCharsPresent = 0;

    for (int i = 0; i < strLen; i++)
    {
        if (str[i] == ',' || str[i] == '\'' || str[i] == '"' || str[i] == ' ')
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
    normalisedString[j] = '"';

    return normalisedString;
}

char *csvParseString(FILE *fPtr)
{
    char thisChar = '\0';
    char *parsedString = malloc(384);
    int i = 0;
    int quoteOpened = 0;
    int lastCharacterWasQuote = 0;
    int malformedLine = 0;

    if (!feof(fPtr))
    {
        thisChar = getc(fPtr);
    }
    while (!feof(fPtr))
    {
        if (thisChar == '"')
        {
            if (!quoteOpened)
            {
                quoteOpened = 1;
            }
            else
            {
                if (lastCharacterWasQuote)
                {
                    parsedString[i++] = thisChar;
                    lastCharacterWasQuote = 0;
                }
                else
                {
                    lastCharacterWasQuote = 1;
                }
            }
        }
        else
        {
            if (lastCharacterWasQuote && (thisChar == ',' || thisChar == '\n'))
            {
                quoteOpened = 0;
                break;
            }
            else if (!quoteOpened && (thisChar == ',' || thisChar == '\n'))
            {
                break;
            }
            else if (lastCharacterWasQuote)
            {
                malformedLine = 1;
                lastCharacterWasQuote = 0;
                break;
            }
            else if (thisChar == '\n')
            {
                malformedLine = 1;
                break;
            }
            else
            {
                parsedString[i++] = thisChar;
            }
        }

        thisChar = getc(fPtr);
    }
    parsedString[i] = '\0';

    if (malformedLine || feof(fPtr))
    {
        free(parsedString);
        return NULL;
    }
    else
    {
        return parsedString;
    }
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

int readFromFile(char *fileName, BoardNodePtr *startPtr)
{
    FILE *fPtr;
    int lineNum = 0;

    if ((fPtr = fopen(fileName, "r")) == NULL)
    {
        printLog('e', "Could not open file.\n");
        return 1;
    }

    while(!feof(fPtr))
    {
        lineNum++;
        char *listName = csvParseString(fPtr);
        char *listItem = csvParseString(fPtr);

        if (listName != NULL && listItem != NULL)
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
                    fclose(fPtr);
                    return 1;
                }
            }

            if (insertListItem(&(listPtr->startPtr), listItem) == 1)
            {
                fclose(fPtr);
                return 1;
            }

            free(listName);
            free(listItem);
        }
        else
        {
            if (!feof(fPtr))
            {
                printLog('e', "Error parsing CSV on line %d.\n", lineNum);
                fclose(fPtr);
                return 2;
            }
        }
    }

    fclose(fPtr);
    return 0;
}
