/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <parser.h>
#include <lexer.h>

#define	MAXSYMTAB_SIZE	0x10000
int		symtab_nextentry = 0;
char	symtab[MAXSYMTAB_SIZE][MAXID_SIZE+1];
double	memtab[MAXSYMTAB_SIZE];
#define	MAXSTACK_SIZE	64
int		sp = -1;
double	stack[MAXSTACK_SIZE];
double	accumulator;
double	auxregister;

void print_mem() {
	int i = 0;

	for(; i < symtab_nextentry; ++i) {
		printf("\nPOS: %d | ID: %s | VAL: %f\n", i, symtab[i], memtab[i]);
	}
}

void push(double value) {
	if(sp < MAXSTACK_SIZE) {
		sp++;
		stack[sp] = value;
	}else{
		fprintf(stderr, "stack overflow... exiting\n");
		exit(-9);
	}
}

double pop(void) {
	if(sp > -1) {
		double value = stack[sp];
		sp--;
		return value;
	}else{
		fprintf(stderr, "stack underflow... exiting\n");
		exit(-8);
	}
}

void enter(double  value) {
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

void insert_symbol(char *identifier) {
	int index = search_symbol(identifier);

	if(index == -1) {
		strcpy(symtab[symtab_nextentry], identifier);
		memtab[symtab_nextentry] = accumulator;
		symtab_nextentry++;
	}else{
		memtab[symtab_nextentry] = accumulator;
	}
}

void operation(int op) {
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

/*************************** LL(1) grammar definition ******************************
 *
 * expr -> term {addop term}
 *
 * term -> fact {mulop fact}
 *
 * fact -> vrbl | cons | ( expr )
 *
 * vrbl -> ID
 *
 * cons -> DEC
 *
 * addop -> '+' | '-'
 *
 * mulop -> '*' | '/'
 *
 * ID = [A-Za-z][A-Za-z0-9]*
 *
 * DEC = [1-9][0-9]* | 0
 *
 **********************************************************************************/

/***************************** LL(1) grammar emulation *****************************
 *
 *      source language        ||          object language
 * -----------------------------------------------------------------------
 * expr -> term { addop term } || expr.pf := term.pf { term.pf addop.pf }
 * -----------------------------------------------------------------------
 * expr -> term { addop term [[ printf(addop.pf); ]] }
 */

void cmd(void) {
	expr();
	while(lookahead == ';') {
		match(';');
		if(lookahead > 0) expr();
	}
}

void expr (void) {
	/**/int op, neg = 0/**/;
	if(lookahead == '-'){
		match('-');
		/**/neg = '-'/**/;
	}
	term(); /**/if(neg) {printf("<+|->");}/**/
	while( op = addop() ) {
	/**/printf("<enter> ")/**/; enter(atof(lexeme)); term();/**/printf("<%c> ",op)/**/; operation(op);
	}
}
/*
 * term -> fact { mulop fact } || term.pf := fact.pf { fact.pf mulop.pf }
 */
void term (void) {
	/**/int op/**/;
	fact();
	while( op = mulop() ) {
	/**/printf("<enter> ")/**/; enter(atof(lexeme)); fact();/**/printf("<%c> ",op)/**/; operation(op);
	}
}
/* old: fact -> vrbl | cons | ( expr ) || fact.pf := expr.pf
 * new: fact -> ID[ = expr] | DEC | ( expr ) */
void fact (void) {
	/**/char bkplexeme[MAXID_SIZE+1]/**/;
		int i;
        switch (lookahead) {
        case ID:
			strcpy(bkplexeme, lexeme);
			match(ID);
			if(lookahead == '=') {
				match('='); expr();/**/printf("%s <store> ", bkplexeme)/**/;
				insert_symbol(bkplexeme);
			}else{
				/**/printf("%s ", bkplexeme)/**/;
				i = search_symbol(bkplexeme);
				accumulator = memtab[i];
			}
		break;

	    case DEC:
			/**/printf("\nMATCH: %s ", lexeme)/**/;
			accumulator = strtod(lexeme, NULL);
	    	match (DEC);
		break;

		case OCTAL:
			/**/printf("%s ", lexeme);/**/; match(OCTAL);
		break;

		case HEX:
			/**/printf("%s ", lexeme);/**/; match(HEX);
		break;

		case FLT:
			/**/printf("%s ", lexeme);/**/; match(FLT);
		break;

	    default:
	            match ('('); expr(); match (')');
    }
}
/*
 * vrbl -> ID
 *
 * cons -> DEC
*
 * ID = [A-Za-z][A-Za-z0-9]*
 *
 * DEC = [1-9][0-9]* | 0
 *
 * addop -> '+' | '-' */
int addop (void) {
        switch(lookahead){
	        case '+':
				match('+'); return '+';
	        case '-':
				match('-'); return '-';
        }
        return 0;
}

/*
 * mulop -> '*' | '/' */
int mulop (void) {
        switch(lookahead) {
	        case '*':
				match('*'); return '*';
	        case '/':
				match('/'); return '/';
        }
        return 0;
}
/***************************** lexer-to-parser interface **************************/

int lookahead; // @ local

void match (int expected) {
	if ( expected == lookahead) {
		lookahead = gettoken (source);
	} else {
		fprintf(stderr,"parser: token mismatch error. found # %d ", lookahead);
		fprintf(stderr,"whereas expected # %d\n", expected);
		exit (SYNTAX_ERR);
	}
}
