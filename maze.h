#ifndef _MAZE_INCLUDED_
#define _MAZE_INCLUDED_

#include <stdio.h>
#include "cell.h"
#include "stack.h"
#include "queue.h"

typedef struct MAZE MAZE;

extern MAZE *newMAZE(int x, int y);
extern void gridTest(MAZE *z);
extern void setSeed(MAZE *z,unsigned int value);
extern int getCELLindex(int z,int x, int y);
extern CELL *getCELL(MAZE *z,int index);
extern int checkNeighbors(MAZE *z,CELL *pointer);
extern CELL **rankNeighbors(MAZE *z,CELL *pointer,int count);
extern void printCELL(int row,int col,CELL *z,FILE *fp);
extern void printCELLbottom(CELL *z,FILE *fp);
extern void displayMAZE(MAZE *z,FILE *fp);
extern void breakWall(CELL *first,CELL *second,int col);
extern void dig(MAZE *z,CELL *pointer,STACK *line);
extern void buildMAZE(MAZE *z);
extern void getAvailable(MAZE *z,QUEUE *list,CELL *pointer);
extern void solveMAZE(MAZE *z);
extern MAZE *readMAZE(FILE *fp);
extern void writeMAZE(MAZE *z,FILE *fp);
extern void freeMAZE(MAZE *z);

#endif
