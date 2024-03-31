#ifndef FILEIO_H
#define FILEIO_H

#include "board.h"

int saveToFile(BoardNodePtr startPtr);
int readFromFile(char *fileName, BoardNodePtr *startPtr);

#endif
