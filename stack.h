#ifndef __STACK_INCLUDED__
#define __STACK_INCLUDED__

#include <stdio.h>

typedef struct STACK STACK;

extern STACK *newSTACK(void);
extern void  setSTACKdisplay(STACK *,void (*)(void *,FILE *));
extern void  setSTACKfree(STACK *,void (*)(void *));
extern void  push(STACK *items,void *value);
extern void *pop(STACK *items);
extern void *peekSTACK(STACK *items);
extern void  displaySTACK(STACK *items,FILE *fp);
extern int   debugSTACK(STACK *items,int level);
extern int   sizeSTACK(STACK *items);
extern void  freeSTACK(STACK *items);

#endif
