/*Written by Andrew Duerr
 *For Project0
 *In class CS201
 *Taught by Dr. Lusth
 *Last Updated: 8/31/18		*/

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "da.h"

struct STACK{
	DA *array;
	int debug;
	void (*display)(void *,FILE *);
	void (*freedom)(void *);
};

STACK *newSTACK(void){
	STACK *new = malloc(sizeof(STACK));
	new->array = newDA();
	new->debug = 0;
	new->display = NULL;
	new->freedom = NULL;
	return new;
}

void setSTACKdisplay(STACK *items,void (*d)(void *,FILE *)){
	items->display = d;
	setDAdisplay(items->array,d);
}

void setSTACKfree(STACK *items,void (*f)(void *)){
	items->freedom = f;
	setDAfree(items->array,f);
}

void push(STACK *items,void *value){
	insertDA(items->array,sizeDA(items->array),value);
}

void *pop(STACK *items){
	return removeDA(items->array,sizeDA(items->array)-1);
}

void *peekSTACK(STACK *items){
	return getDA(items->array,sizeDA(items->array)-1);
}

void displaySTACK(STACK *items,FILE *fp){
	if(items->debug == 1){
		displayDA(items->array,fp);
	}
	else if(items->debug == 2){
		int old = debugDA(items->array,1);
		displayDA(items->array,fp);
		debugDA(items->array,old);
	}
	else{
		if(sizeDA(items->array) == 0 || items->display == NULL){
			fprintf(fp,"||");
		}
		else{
			fprintf(fp,"|");
			for(int i = sizeDA(items->array)-1; i > 0; i--){
				items->display(getDA(items->array,i),fp);
				fprintf(fp,",");
			}
			items->display(getDA(items->array,0),fp);
			fprintf(fp,"|");
		}
	}
}

int debugSTACK(STACK *items,int level){
	int old = items->debug;
	items->debug = level;
	return old;
}

int sizeSTACK(STACK *items){
	return sizeDA(items->array);
}

void freeSTACK(STACK *items){
	freeDA(items->array);
	free(items);
}
