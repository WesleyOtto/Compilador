/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <keywords.h>


/*mypas -> body'.' */
void mypas(void){
	body();
	match('.');
}

/* body -> declarative imperative */
void body(void){
	declarative();
	imperative();
}

/* namelist -> ID {, ID} */
void namelist(void){
	match(ID);
	while(lookahead == ',') {
		match(',');
		match(ID);
	}
}

void vartype(void){
	switch(lookahead){
		case INT:
			match(INT);
		break;

		case FLOAT:
			match(FLOAT);
		break;

		case HEXA:
			match(HEXA);
		break;

		case OCTA:
			match(OCTA);
		break;
	}
}

/* parmdef -> [ ( [VAR] namelist ':' vartype { ';' [VAR] namelist ':' vartype } ) ] */
void parmdef(void) {
	if(lookahead == '('){
		match('(');
		par_begin:
		if(lookahead == VAR) match(VAR);
		namelist();
		match(':');
		vartype();

		if(lookahead == ';'){
			match(';');
			goto par_begin;
		}
		match(')');
	}
}

/* declarative -> [ VAR namelist ':' vartype ';' { namelist ':' vartype ';' } ]
				{ sbpmod sbpname parmdef [ ':' fnctype ] ';' body ';' } */
void declarative(void){
	int sbpmod;
	if(lookahead == VAR){
		match(VAR);
		do{
			namelist();
			match(':');
			vartype();
			match(';');
		}while(lookahead == ID);
	}

	while(lookahead == PROCEDURE || lookahead == FUNCTION){
		sbpmod = lookahead;
		match(lookahead);
		match(ID);
		parmdef();
		if(sbpmod == FUNCTION && lookahead == ':'){
			match(':');
			vartype();//vartype == fnctype
		}
		match(';');
		body();
		match(';');
	}
}

/* imperative -> BEGIN stmtlist END */
void imperative(void) {
	match(BEGIN);
	stmtlist();
	match(END);
}

/* stmtlist -> stmt { ';' stmt } */
void stmtlist(void) {
	stmt();
	while(lookahead == ';') {
		match(';');
		stmt();
	}
}

/*stmt -> imperative
*	| IF expr THEN stmt [ ELSE stmt ]
*	| WHILE expr REPEAT stmt
*	| REPEAT stmtlist UNTIL expr
*	| expr
*	| <empty>
*/
void stmt(void){
	switch (lookahead) {
		case BEGIN:
			imperative();
			break;
		case IF:
			ifstmt();
			break;
		case WHILE:
			whilestmt();
			break;
		case REPEAT:
			repstmt();
			break;
		case ID: case DEC: case '(':
			expr();
			break;
	}
}

/* IF expr THEN stmt [ ELSE stmt ] */
void ifstmt(void){
	match(IF);
	expr();
	match(THEN);
	stmt();

	if(lookahead == ELSE){
		match(ELSE);
		stmt();
	}
}

/* WHILE expr REPEAT stmt */
void whilestmt(void){
	match(WHILE);
	expr();
	match(REPEAT);
	stmt();
}

/* REPEAT stmtlist UNTIL expr */
void repstmt(void){
	match(REPEAT);
	stmtlist();
	match(UNTIL);
	expr();
}

/***************************** LL(1) grammar emulation *****************************
 *
 *      source language        ||          object language
 * -----------------------------------------------------------------------
 * expr -> term { addop term } || expr.pf := term.pf { term.pf addop.pf }
 * -----------------------------------------------------------------------
 * expr -> term { addop term [[ printf(addop.pf); ]] }
 */
void expr (void){
	if (lookahead == '-'){
		match('-');
	}

	T_entry:
	F_entry:
	switch (lookahead){
	case ID :
		match(ID);
		if ( lookahead == '='){
			match('=');
			expr();
		}
		break;
	case DEC :
		match(DEC);
		break;
	case OCTAL:
		match(OCTAL);
		break;
	case HEX:
		match(HEX);
		break;
	case FLT:
		match(FLT);
		break;
	default :
		match('(');
		expr();
		match(')');
	}

	if ( mulop() ) goto F_entry;
	if ( addop() ) goto T_entry;

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

/*mulop -> '*' | '/' */
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
