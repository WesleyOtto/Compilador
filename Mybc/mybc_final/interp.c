/**@<interp.c>::**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <interp.h>

int		symtab_nextentry = 0; //@ interp.h
char	symtab[MAXSYMTAB_SIZE][MAXID_SIZE+1]; //@ interp.h
double	memtab[MAXSYMTAB_SIZE]; //@ interp.h

int		sp = -1; //@ interp.h
double	stack[MAXSTACK_SIZE]; //@ interp.h
double	accumulator; //@ interp.h
double	auxregister; //@ interp.h


void push (double value) {
	if(sp < MAXSTACK_SIZE) {
		sp++;
		stack[sp] = value;
	}else{
		fprintf(stderr, "Stack overflow... exiting\n");
		exit(-9);
	}
}

double pop (void) {
	if(sp > -1) {
		double value = stack[sp];
		sp--;
		return value;
	}else{
		fprintf(stderr, "Stack underflow... exiting\n");
		exit(-8);
	}
}

void enter (double  value) {
	push(accumulator);
	accumulator = value;
}

int search_symbol(char *identifier) {
	int i = 0;

	for(; i < symtab_nextentry; ++i) {
		if(! strcmp(identifier, symtab[i])) return i;
	}
	return -1;
}

void insert_symbol (char *identifier) {
	int index = search_symbol(identifier);

	if(index == -1) {
		strcpy(symtab[symtab_nextentry], identifier);
		memtab[symtab_nextentry] = accumulator;
		symtab_nextentry++;
	}else{
		memtab[index] = accumulator;
	}
}

void operation (int op) {
	auxregister = pop();
	switch(op) {
		case '+':
			accumulator = auxregister + accumulator;
		break;

		case '-':
			accumulator = auxregister - accumulator;
		break;

		case '*':
			accumulator = auxregister * accumulator;
		break;

		case '/':
			accumulator = auxregister / accumulator;
		break;
	}
}
