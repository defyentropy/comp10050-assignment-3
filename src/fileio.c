#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/board.h"
#include "../include/logging.h"

// parses one field of a CSV, where a field is defined as a length
// of characters delimited by a comma
// checks that the field is properly formatted according to the CSV
// spec and and also that it is not more than 80 chractters in length
// _after_ being parsed
static int csvParseString(FILE *fPtr, char *dest, char *thisChar)
{
    // we won't write all the characters we read to the buffers
    // e.g. escape characters, so we need to keep track of them
    // separately
    int charsWritten = 0;
    int charsRead = 0;
    // a field with an opening quote must also be closed by a quote
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
        // check if the field that is currently being read
        // has exceeded the maximum length of 80 characters
        if (charsWritten > 79)
        {
            malformedField = 1;
            break;
        }
        else if (*thisChar == '"')
        {
            // the quote appears at the beginning, i.e. it is a wrapper
            if (charsRead == 1)
            {
                quoteOpened = 1;
            }
            // if a quote appears in the middle of a field that was not
            // wrapped by quotes, that is a formatting error
            else if (!quoteOpened)
            {
                malformedField = 1;
                break;
            }
            // if the last character was a quote, then this one is part of
            // the field itself
            else if (lastCharacterWasQuote)
            {
                dest[charsWritten++] = *thisChar;
                lastCharacterWasQuote = 0;
            }
            // set a flag and wait and see what the next character is
            else
            {
                lastCharacterWasQuote = 1;
            }
        }
        else
        {
            // if we reach a comma or a newline and all the text we've read in
            // so far is valid, then that must mean we have reached the end of
            // the field
            if ((lastCharacterWasQuote || !quoteOpened)  && (*thisChar == ',' || *thisChar == '\n'))
            {
                break;
            }
            // the last character must have been an unescaped quote
            else if (lastCharacterWasQuote)
            {
                malformedField = 1;
                break;
            }
            // unescaped space character
            else if (!quoteOpened && *thisChar == ' ')
            {
                malformedField = 1;
                break;
            }
            // valid characters are copied over to the destination
            else
            {
                dest[charsWritten++] = *thisChar;
            }
        }

        *thisChar = fgetc(fPtr);
        charsRead++;
    }

    // null-termiate whatever string we've read, valid or invalid
    if (charsWritten > 79)
    {
        charsWritten--;    
    }
    dest[charsWritten++] = '\0';

    if (feof(fPtr))
    {
        // valid field parsed and end of file reached
        if (lastCharacterWasQuote)
        {
            return 0;
        }
        // reached EOF in the middle of a field, i.e. missing closing quote
        else if (charsWritten > 1)
        {
            return 1;
        }
        // reached EOF, but no errors
        else
        {
            return 2;
        }
    }
    // empty field or field with unescaped characters
    else if (malformedField || charsWritten == 1)
    {
        return 1;
    }
    // valid field read
    else
    {
        return 0;
    }
}

// parses one line of a CSV file, where each line is of the format
// <list name>,<list item>
// and a line must only have two fields
static int csvParseLine(FILE *fPtr, char *listName, char *listItem)
{
    int lineStatus = 0;
    char thisChar;

    lineStatus = csvParseString(fPtr, listName, &thisChar);

    // if the first field was parsed correctly
    if (!lineStatus)
    {
        // if the first field is not immediately followed by a comma, then
        // the file formatting is wrong
        if (thisChar != ',')
        {
            lineStatus = 1;
        }

        lineStatus = csvParseString(fPtr, listItem, &thisChar);

        // if the second field is not immediately followed by a newline, then
        // that is also an error
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

    // safely open file
    if ((fPtr = fopen(fileName, "r")) == NULL)
    {
        printLog('e', "Could not open file.\n\n");
        return 1;
    }

    while(!feof(fPtr))
    {
        lineNum++;
        // parse a line from the CSV
        lineStatus = csvParseLine(fPtr, listName, listItem);

        // if the two fields were read successfully, insert the data into
        // the board
        if (lineStatus == 0)
        {
            BoardNodePtr listPtr = searchByListName(*startPtr, listName);
            // create the list if it does not exist already
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

            // if the list item is a single space character, that indicates
            // that the list it belongs to is empty, so the list item is
            // not inserted
            if (strcmp(listItem, " ") == 0)
            {
                break;
            }
            else if (insertListItem(&(listPtr->startPtr), listItem) == 1)
            {
                err = 2;
                break;
            }
        }
        // if the CSV file could not be read successfully, report it to the
        // user so they may manually inspect and correct the file
        else if (lineStatus == 1) 
        {
               printLog('e', "Error parsing CSV on line %d.\n\n", lineNum);
                err = 1;
                break;
        }
    }

    free(listName);
    free(listItem);
    fclose(fPtr);

    if (err == 0)
    {
        printLog('s', "Board imported from file \"%s\".\n\n", fileName);
    }

    return err;
}

// checks if a string contains special characters, i.e.
// if it needs to be normalised before being written to a file
static int containsSpecialCharacters(char *str)
{
    int strLen = strlen(str);
    int specialCharsPresent = 0;

    // simply iterate through each character in the string
    // and count how many are commas, spaces, or quotes,
    // which are characters that will cause problems when written
    // to a CSV file without being escaped first
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

// normalises a CSV string by wrapping it in quotes and escaping
// any special characters so it can be safely written to file
static char *csvNormaliseString(char *plainString)
{
    int i, j;
    int strLen = strlen(plainString);
    // the worst case scenario for a string that needs to be
    // escaped is one that consists of all quotes, because all
    // of them have to be escaped and then wrapped in double quotes
    // 2 * strLen for the quotes + escapes, +3 for wrap quotes and \0
    char *normalisedString = malloc(2 * strLen + 3);

    // start off with a double quote
    normalisedString[0] = '"';
    // copy over the characters of the unescaped string one-by-one,
    // adding an extra quote before each quote in the unescaped string
    for (i = 0, j = 1; plainString[i] != '\0'; i++)
    {
        if (plainString[i] == '"')
        {
            normalisedString[j++] =  '"';
        }
        normalisedString[j++] = plainString[i];
    }
    // finish off by closing the wrapper quotes and null-terminating
    normalisedString[j++] = '"';
    normalisedString[j++] = '\0';

    return normalisedString;
}

int saveToFile(char *fileName, BoardNodePtr startPtr)
{
    // no point in saving an empty board
    if (startPtr == NULL)
    {
        printLog('i', "The board is empty. There is nothing to write to file.\n\n");
        return 1;
    }

    // safely open file
    FILE *fPtr;
    if ((fPtr = fopen(fileName, "w")) == NULL)
    {
        printLog('e', "Could not open file.\n\n");
        return 2;
    }

    // move to the last list on the board
    // this is because saving from and reading to the top of  list
    // means that it will behave like a stack, and the lists and 
    // their elements will get reversed on every read/write
    BoardNodePtr currentListPtr = startPtr;
    while (currentListPtr->nextPtr != NULL)
    {
        currentListPtr = currentListPtr->nextPtr;
    }
        
    // iterate to the beginning of the list
    while (currentListPtr != NULL)
    {
        ListNodePtr currentListItemPtr = currentListPtr->startPtr;

        // if a list is empty, write a single space to the file
        // when parsing a CSV file, a field that contains only a space
        // indicates that a list must be created, but no item is to be
        // inserted for that line in the CSV file
        if (currentListItemPtr == NULL)
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

            fprintf(fPtr, "\" \"\n");
        }
        else
        {
            // navigate to the end of the list
            while (currentListItemPtr->nextPtr != NULL)
            {
                currentListItemPtr = currentListItemPtr->nextPtr;
            }

            // iterate back to the beginning of the list
            while (currentListItemPtr != NULL)
            {
                // if the list name contains special characters,
                // then normalise it and then write it to the file
                if (containsSpecialCharacters(currentListPtr->listName))
                {
                    char *normalisedString = csvNormaliseString(currentListPtr->listName);
                    fprintf(fPtr, "%s,", normalisedString);
                    free(normalisedString);
                }
                // otherwise just write it directly
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

                currentListItemPtr = currentListItemPtr->prevPtr;
            }
        }
        
        currentListPtr = currentListPtr->prevPtr;
    }

    fclose(fPtr);
    
    return 0;
}
