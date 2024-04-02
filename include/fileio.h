#ifndef FILEIO_H
#define FILEIO_H

#include "board.h"

int saveToFile(char *fileName, BoardNodePtr startPtr);
int readFromFile(char *fileName, BoardNodePtr *startPtr);

#endif
