#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "maze.h"


/* ======= Options ======= *
-d [Filename] : display given maze file
-s [input maze] [output maze] : solve given maze file
-r [int] : set seed
-c [rows] [cols] [new maze name] : create maze of given size
*  ======================= */

int ProcessOptions(int,char **);
void Fatal(char *,...);
MAZE *drawing = NULL;
unsigned int seed = 1;
int row;
int col;
unsigned int solveSeed = 1;
int solveRow;
int solveCol;
FILE *buildOUT = NULL;
FILE *solveIN = NULL;
FILE *solveOUT = NULL;
FILE *displayIN = NULL;
FILE *testIN = NULL;

int main(int argc, char **argv) {
	if (argc == 1) Fatal("%d arguments!\n",argc-1);
	ProcessOptions(argc,argv);

	if(buildOUT != NULL){
		fprintf(buildOUT,"%d %d %d %d",seed,row,col,0);
		fclose(buildOUT);
	}

	if(solveIN != NULL){
		fscanf(solveIN,"%d",&solveSeed);
		fscanf(solveIN,"%d",&solveRow);
		fscanf(solveIN,"%d",&solveCol);
		fprintf(solveOUT,"%d %d %d %d",solveSeed,solveRow,solveCol,1);
		fclose(solveIN);
		fclose(solveOUT);
	}

	if(displayIN != NULL){
		drawing = readMAZE(displayIN);
		displayMAZE(drawing,stdout);
		freeMAZE(drawing);
		fclose(displayIN);
	}
	return 0;
}

void
Fatal(char *fmt, ...)
    {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
    }

/* only -oXXXX  or -o XXXX options */

int
ProcessOptions(int argc, char **argv)
    {
    int start,argIndex;
    int argsUsed;
    char *arg;

    argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-')
        {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        argsUsed = 0;
        start = argIndex;


        /* advance argIndex to point to the first argument to the option */
        if (argv[start][2] == '\0')
            {
            arg = argv[start+1];
            ++argIndex;
            }
        else
            {
            /* first arg is connected to option, so don't advance */
            arg = argv[start]+2;
            }

        switch (argv[start][1])
            {
            case 'v':
                printf("Written by Andrew Duerr\n");
                break;
            case 's':	;
								solveIN = fopen(arg,"r");
								solveOUT = fopen(argv[argIndex+1],"w");
                argsUsed = 2;
                break;
            case 'c':	;
								buildOUT = fopen(argv[argIndex+2],"w");
								row = atoi(arg);
								col = atoi(argv[argIndex+1]);
								argsUsed = 3;
                break;
						case 'r':	;
								seed = atoi(arg);
								argsUsed = 1;
                break;
						case 'd':	;
								displayIN = fopen(arg,"r");
								argsUsed = 1;
                break;
            default:
                fprintf(stderr,"option %s not understood\n",argv[start]);
                exit(-1);
            }

        argIndex += argsUsed;
        }

    return argIndex;
    }
