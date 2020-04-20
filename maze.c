#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "maze.h"
#include "cell.h"
#include "stack.h"
#include "queue.h"

long int random(void);
void srandom(unsigned int seed);
// unsigned int seed = 0;

struct MAZE{
	CELL **grid;
	int row;
	int col;
	int size;
	int solved;
	unsigned int seed;
};

MAZE *newMAZE(int row, int col){
	MAZE *new = malloc(sizeof(MAZE));
	new->row = row;
	new->col = col;
	new->size = row * col;
	new->solved = 0;
	new->seed = 1;
	new->grid = malloc(sizeof(CELL *) * new->size);
	for(int i = 0; i < new->row; i++){
		for(int j = 0; j < new->col; j++){
			// printf("newMAZE: making cell at index %d [%d,%d]\n",(i*new->col)+j,i,j);
			new->grid[(i*new->col)+j] = newCELL();
			setPos(new->grid[(i*new->col)+j],i,j);
			setIndex(new->grid[(i*new->col)+j],(i*new->col)+j);
		}
		/* old
		new->grid[i] = newCELL();
		setPos(new->grid[i],i/new->col,i%new->col);
		setIndex(new->grid[i],i);
		*/
	}
	return new;
}

void gridTest(MAZE *z){
	for(int i = 0; i < z->row; i++){
		for(int j = 0; j < z->col; j++){
			printf("(%d,%d)\t",getXpos(z->grid[(i*z->col)+j]),getYpos(z->grid[(i*z->col)+j]));
		}
		printf("\n");
	}
}

void setSeed(MAZE *z,unsigned int value){
	// printf("Seed was %d, now is %d\n",z->seed,value);
	z->seed = value;
}

int getCELLindex(int z,int x,int y){
	return ((x * z) + y);
}

CELL *getCELL(MAZE *z,int index){
	assert(index >= 0 && index < z->size);
	return z->grid[index];
}

void printCELL(int row,int col,CELL *z,FILE *fp){
	fprintf(fp," ");
	if(getSearched(z) >= 0){
		fprintf(fp,"%d",getSearched(z)%10);
	}
	else{
		fprintf(fp," ");
	}
	fprintf(fp," ");

	if(getXpos(z) == row-1 && getYpos(z) == col-1){ //is last cell
		fprintf(fp," ");
	}
	else if(isRight(z)){ //cell has broken right wall
		fprintf(fp," ");
	}
	else{
		fprintf(fp,"|");
	}
}

void printCELLbottom(CELL *z,FILE *fp){
	if(isBottom(z)){ //cell has broken right wall
		fprintf(fp,"   ");
	}
	else{
		fprintf(fp,"---");
	}
	fprintf(fp,"-");
}

void displayMAZE(MAZE *z,FILE *fp){
	fprintf(fp,"-");
	for(int i = 0; i < z->col * 4; i++){
		fprintf(fp,"-");
	}
	fprintf(fp,"\n");
	fprintf(fp," ");
	for(int x = 0; x < z->row; x++){
		for(int y = 0; y < z->col; y++){
			printCELL(z->row,z->col,z->grid[getCELLindex(z->col,x,y)],fp);
		}
		fprintf(fp,"\n-");
		for(int y = 0; y < z->col; y++){
			printCELLbottom(z->grid[getCELLindex(z->col,x,y)],fp);
		}
		if(x != z->row-1){
			fprintf(fp,"\n|");
		}
		else{
			fprintf(fp,"\n");
		}
	}
}

int checkNeighbors(MAZE *z,CELL *pointer){
	int tally = 0;
	if(getXpos(pointer) > 0 && !isVisited(z->grid[getIndex(pointer)-z->col])){
		tally += 1;
	}
	if(getYpos(pointer) > 0 && !isVisited(z->grid[getIndex(pointer)-1])){
		tally += 1;
	}
	if(getXpos(pointer) < z->row-1 && !isVisited(z->grid[getIndex(pointer)+z->col])){
		tally += 1;
	}
	if(getYpos(pointer) < z->col-1 && !isVisited(z->grid[getIndex(pointer)+1])){
		tally += 1;
	}
	return tally;
}

CELL **rankNeighbors(MAZE *z,CELL *pointer,int count){
	CELL **temp = malloc(sizeof(CELL *) * count);
	int rank = 0;
	if(getXpos(pointer) > 0 && !isVisited(z->grid[getIndex(pointer)-z->col])){
		//look up
		temp[rank] = getCELL(z,getIndex(pointer)-z->col);
		rank += 1;
	}
	if(getYpos(pointer) > 0 && !isVisited(z->grid[getIndex(pointer)-1])){
		//look left
		temp[rank] = getCELL(z,getIndex(pointer)-1);
		rank += 1;
	}
	if(getYpos(pointer) < z->col-1 && !isVisited(z->grid[getIndex(pointer)+1])){
		//look right
		temp[rank] = getCELL(z,getIndex(pointer)+1);
		rank += 1;
	}
	if(getXpos(pointer) < z->row-1 && !isVisited(z->grid[getIndex(pointer)+z->col])){
		//look down
		temp[rank] = getCELL(z,getIndex(pointer)+z->col);
		rank += 1;
	}
	return temp;
}

void breakWall(CELL *first,CELL *second,int col){
	int x = getIndex(first);
	int y = getIndex(second);

	if(y == x - col){
		breakBottom(second);
	}
	else if(y == x - 1){
		breakRight(second);
	}
	else if(y == x + 1){
		breakRight(first);
	}
	else if(y == x + col){
		breakBottom(first);
	}
}

void dig(MAZE *z,CELL *pointer,STACK *line){
	int count = checkNeighbors(z,pointer);
	if(count > 0){
		CELL **neighbors;
		neighbors = rankNeighbors(z,pointer,count);
		CELL *newspot = neighbors[random()%count];
		breakWall(pointer,newspot,z->col);
		push(line,newspot);
		free(neighbors);
	}
	else{
		pop(line);
	}
}

void buildMAZE(MAZE *z){
	// printf("Calling srandomw with %d\n",z->seed);
	srandom(z->seed);
	CELL *pointer = z->grid[0];
	STACK *line = newSTACK();
	push(line,pointer);
	while(sizeSTACK(line) > 0){
		pointer = peekSTACK(line);
		setVisited(pointer);
		dig(z,pointer,line);
	}
	freeSTACK(line);
}

void getAvailable(MAZE *z,QUEUE *list,CELL *pointer){
	if(getXpos(pointer) > 0 && isBottom(z->grid[getIndex(pointer)-z->col])){
		if(getSearched(z->grid[getIndex(pointer)-z->col]) == -1){
			enqueue(list,z->grid[getIndex(pointer)-z->col]);
			setSearched(z->grid[getIndex(pointer)-z->col],getSearched(pointer)+1);
		}
	}
	if(getYpos(pointer) > 0 && isRight(z->grid[getIndex(pointer)-1])){
		if(getSearched(z->grid[getIndex(pointer)-1]) == -1){
			enqueue(list,z->grid[getIndex(pointer)-1]);
			setSearched(z->grid[getIndex(pointer)-1],getSearched(pointer)+1);
		}
	}
	if(/*getYpos(pointer) < z->col-1 &&*/ isRight(pointer)){
		if(getSearched(z->grid[getIndex(pointer)+1]) == -1){
			enqueue(list,z->grid[getIndex(pointer)+1]);
			setSearched(z->grid[getIndex(pointer)+1],getSearched(pointer)+1);
		}
	}
	if(/*getXpos(pointer) < z->row-1 &&*/ isBottom(pointer)){
		if(getSearched(z->grid[getIndex(pointer)+z->col]) == -1){
			enqueue(list,z->grid[getIndex(pointer)+z->col]);
			setSearched(z->grid[getIndex(pointer)+z->col],getSearched(pointer)+1);
		}
	}
}

void solveMAZE(MAZE *z){
	int end = z->size-1;
	// printf("end index: %d\n",end);
	QUEUE *solving = newQUEUE();
	enqueue(solving,getCELL(z,0));
	setSearched(getCELL(z,0),0);
	while(sizeQUEUE(solving) > 0 && getIndex(peekQUEUE(solving)) != end){
		getAvailable(z,solving,dequeue(solving));
		// if(sizeQUEUE(solving) <= 0){
		// 	printf("Queue went empty.\n");
		// }
	}
	if(sizeQUEUE(solving) > 0){
		dequeue(solving);
		while(sizeQUEUE(solving) > 0){
			setSearched(dequeue(solving),-1);
		}
	}
	freeQUEUE(solving);
	z->solved = 1;
}

MAZE *readMAZE(FILE *fp){
	int seed;
	int rows;
	int col;
	int solved;
	fscanf(fp,"%d",&seed);
	fscanf(fp,"%d",&rows);
	fscanf(fp,"%d",&col);
	fscanf(fp,"%d",&solved);
	MAZE *temp = newMAZE(rows,col);
	setSeed(temp,seed);
	buildMAZE(temp);
	if(solved == 1){
		solveMAZE(temp);
	}
	return temp;
}

void writeMAZE(MAZE *z,FILE *fp){
	fprintf(fp,"%d %d %d %d",z->seed,z->row,z->col,z->solved);
}

void freeMAZE(MAZE *z){
	for(int i = 0; i < z->size; i++){
		freeCELL(z->grid[i]);
	}
	free(z->grid);
	free(z);
}
