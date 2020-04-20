#ifndef _CELL_INCLUDED_
#define _CELL_INCLUDED_

#include <stdio.h>
#include <stdbool.h>

typedef struct CELL CELL;

extern CELL *newCELL(void);
extern int getXpos(CELL *z);
extern int getYpos(CELL *z);
extern int getIndex(CELL *z);
extern int getSearched(CELL *z);
extern bool isRight(CELL *z);
extern bool isBottom(CELL *z);
extern bool isVisited(CELL *z);
extern void setPos(CELL *z, int x, int y);
extern void setIndex(CELL *z, int value);
extern void setSearched(CELL  *z,int value);
extern void setVisited(CELL *z);
extern void breakRight(CELL *z);
extern void breakBottom(CELL *z);
extern void freeCELL(CELL *z);

#endif
