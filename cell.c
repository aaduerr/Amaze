#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cell.h"

struct CELL{
	int xpos;
	int ypos;
	int index;
	int searched;
	bool rightwall;
	bool bottomwall;
	bool visited;
};

CELL *newCELL(void){
	CELL *new = malloc(sizeof(CELL));
	new->xpos = 0;
	new->ypos = 0;
	new->index = -1;
	new->searched = -1;

	//For walls, 0 is closed 1 is open/broken
	new->rightwall = 0;
	new->bottomwall = 0;
	new->visited = 0;
	return new;
}

int getXpos(CELL *z){
	return z->xpos;
}

int getYpos(CELL *z){
	return z->ypos;
}

int getIndex(CELL *z){
	return z->index;
}

int getSearched(CELL *z){
	return z->searched;
}

bool isRight(CELL *z){
	return z->rightwall;
}

bool isBottom(CELL *z){
	return z->bottomwall;
}

bool isVisited(CELL *z){
	return z->visited;
}

void setPos(CELL *z,int x, int y){
	z->xpos = x;
	z->ypos = y;
}

void setIndex(CELL *z,int value){
	z->index = value;
}

void setSearched(CELL *z, int value){
	z->searched = value;
}

void setVisited(CELL *z){
	z->visited = 1;
}

void breakRight(CELL *z){
	z->rightwall = 1;
}

void breakBottom(CELL *z){
	z->bottomwall = 1;
}

void freeCELL(CELL *z){
	free(z);
}
