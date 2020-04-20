#include <stdio.h>
#include <stdlib.h>
#include "da.h"

struct DA{
	int size;
	int capacity;
	int debug;
	void **data;
	void (*display)(void *,FILE *);
	void (*freedom)(void *);
};

DA *newDA(void){
	DA *new = malloc(sizeof(DA));
	new->size = 0;
	new->capacity = DA_INITIAL_CAPACITY;
	new->debug = 0;
	new->data = malloc(sizeof(void *) * new->capacity);
	new->display = NULL;
	new->freedom = NULL;
	return new;
}

void setDAdisplay(DA *array,void (*d)(void *,FILE *)){
	array->display = d;
}

void setDAfree(DA *array,void (*f)(void *)){
	array->freedom = f;
}

void insertDA(DA *items,int index, void *value){
	if(items->size < items->capacity){
		if(index < items->size){
			void *temp1 = items->data[index];
			void *temp2 = temp1;
			items->data[index] = value;
			for(int i = index+1; i < items->size; i++){
				temp2 = items->data[i];
				items->data[i] = temp1;
				temp1 = temp2;
			}
			items->data[items->size] = temp1;
		}
		else{
			items->data[index] = value;
		}
		items->size += 1;
	}
	else{ //no space -> resize -> retry
		items->capacity *= 2;
		items->data = realloc(items->data,sizeof(void *) * items->capacity);
		insertDA(items,index,value);
	}
}

void *removeDA(DA *items,int index){ // remove rightmost in array
	if(items->size > 0){
		void *temp = items->data[index];
		items->size -= 1;
		for(int i = index; i < items->size; i++){
			items->data[i] = items->data[i+1];
		}
		if((double)items->size/(double )items->capacity < 0.25){
			int newcap = items->capacity / 2;
			if(newcap == 0){newcap = 1;}
			if(items->size == 0){newcap = 1;}
			items->data = realloc(items->data,sizeof(void *) * newcap);
			items->capacity = newcap;
		}
		return temp;
	}
	else{
		return NULL;
	}
}

void unionDA(DA *recipient,DA *donor){
	if(donor->size != 0){
		recipient->capacity = recipient->capacity+donor->capacity;
		recipient->data = realloc(recipient->data,sizeof(void *) * (recipient->capacity));
		for(int i = 0; i < donor->size; i++){
			insertDA(recipient,recipient->size,donor->data[i]); //give each element
			donor->data[i] = NULL; //clear each element
		}
		free(donor->data);
		donor->data = malloc(sizeof(void *));
		donor->capacity = 1;
		donor->size = 0; //donor is empty

	}
}

void *getDA(DA *items,int index){
	if(index >= items->size || index < 0){ //out of bounds
		printf("Index %d is out of bounds for DA size %d\n",index,items->size);
		exit(1);
	}
	// printf("Index is: %d\n",index); //debug
	return items->data[index];
}

void *setDA(DA *items,int index,void *value){
	void *temp = NULL;
	if(index > items->size || index < 0){ //out of bounds
		printf("Index %d is out of bounds for DA size %d\n",index,items->size);
		exit(1);
	}
	else if(index == items->size){ //inserted at end
		insertDA(items,index,value);
		return NULL;
	}
	else{
		temp = items->data[index];
		items->data[index] = value; //change value
		return temp; //return old value
	}
}

int sizeDA(DA *items){
	return items->size;
}

void displayDA(DA *items,FILE *fp){
	fprintf(fp,"[");
	if(items->size > 0){
		for(int i = 0; i < items->size; i++){
			if(i == items->size-1){
				if(items->display == NULL){
					fprintf(fp,"%p",items->data[i]);
				}
				else{
					items->display(items->data[i],fp);
				}
			}
			else{
				if(items->display == NULL){
					fprintf(fp,"%p",items->data[i]);
				}
				else{
					items->display(items->data[i],fp);
				}
				fprintf(fp,","); //comma after each one
			}
		}
	}
	if(items->debug == 1){
		if(items->size == 0){
			fprintf(fp,"[%d]",items->capacity-items->size);
		}
		else{
			fprintf(fp,",[%d]",items->capacity-items->size);
		}
	}
	fprintf(fp,"]");
}

int debugDA(DA *items,int level){
	int temp = items->debug;
	items->debug = level;
	return temp;
}

void freeDA(DA *items){
	if(items->freedom != NULL){
		for(int i = 0; i < items->size; i++){
			items->freedom(items->data[i]);
		}
	}
	free(items->data);
	free(items);
}
