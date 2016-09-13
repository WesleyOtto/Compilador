/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <lexer.h>
#include <parser.h>
#include <interp.h>

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

int iscmdsep (void) {

	if(lookahead == ';') {
		match(lookahead);
		if(lookahead == '\n') match(lookahead);
		return 1;
	}
	if(lookahead == '\n'){
		match(lookahead);
		return 1;
	}
	return 0;
}

void cmd (void) {
	expr();	printf("%f\n", accumulator);
	while(iscmdsep() && lookahead != -1) {
			expr();
			printf("%f\n", accumulator);
	}
	match(EOF);
}

void expr (void) {
	/**/int op, neg = 0/**/;
	if(lookahead == '-') {
		match('-');
		/**/neg = '-'/**/;
	}
	term(); /**/if(neg) {printf("<+|->");}/**/
	while(op = addop() ) {
		enter(atof(lexeme));
		term();
		operation(op);
	}
}
/* term -> fact { mulop fact } || term.pf := fact.pf { fact.pf mulop.pf }*/
void term (void) {
	/**/int op/**/;
	fact();
	while(op = mulop() ) {
		enter(atof(lexeme));
		fact();
		operation(op);
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
				match('='); expr();
				insert_symbol(bkplexeme);
			}else{
				i = search_symbol(bkplexeme);
				accumulator = memtab[i];
			}
		break;

		case DEC:
			accumulator = strtod(lexeme, NULL);
			match (DEC);
		break;

		case OCTAL:
			accumulator = strtol(lexeme, NULL, 0);
			match(OCTAL);
		break;

		case HEX:
			accumulator = strtol(lexeme, NULL, 0);
			match(HEX);
		break;

		case FLT:
			accumulator = strtod(lexeme, NULL);
			match(FLT);
		break;

		default:
			match ('('); expr(); match (')');
    }
}
/* vrbl -> ID
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
/* mulop -> '*' | '/' */
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
