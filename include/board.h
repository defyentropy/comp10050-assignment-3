#ifndef BOARD_H
#define BOARD_H

typedef struct listNode {
    struct listNode *prevPtr;
    char listItem[80];
    struct listNode *nextPtr;
} ListNode;

typedef ListNode *ListNodePtr;

typedef struct boardNode {
    struct boardNode *prevPtr;
    char listName[80];
    ListNodePtr startPtr;
    struct boardNode *nextPtr;
} BoardNode;

typedef BoardNode *BoardNodePtr;

BoardNodePtr initialiseBoard();
int insertListItem(ListNodePtr *startPtr, char listItem[80]);
void freeListItems(ListNodePtr startPtr);
int insertList(BoardNodePtr *startPtr, char listName[80]);
BoardNodePtr searchByListName(BoardNodePtr startPtr, char *listName);
ListNodePtr searchByListItemName(ListNodePtr startPtr, char *listItem);
void removeList(BoardNodePtr *startPtr, BoardNodePtr targetPtr);
void removeListItem(ListNodePtr *startPtr, ListNodePtr targetPtr);
void freeBoard(BoardNodePtr *startPtr);
void displayList(ListNodePtr startPtr);
void displayBoard(BoardNodePtr startPtr, int displayExpanded);

#endif
