/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <keywords.h>


/*************************** LL(1) grammar definition ******************************
 *
 * progcalc -> cmdblock EOF
 *
 * cmdblock -> cmd { <cmdsep> cmd }
 *
 * cmdsep -> ; | EOL
 *
 * cmd -> IF expr THEN { cmdblock } [ ELSE cmdblock ] ENDIF
 *      | WHILE expr DO cmdblock ENDDO
 *	| DO cmdblock WHILE expr
 *	| expr
 *	| <empty>
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

//progcalc -> cmdblock EOF
void progcalc(void){
	cmdblock();
	match(EOF);
}

//cmdblock -> cmd { <cmdsep> cmd }
void cmdblock(void){
	do{
		cmd();
	}while(cmdsep());
}

//cmdsep -> ; | EOF
int cmdsep(void){
	switch(lookahead){
		case ';': 
			match(lookahead); return ';';
			break;
		case '\n':
			match(lookahead); return '\n';
			break;
	}
	return 0;
}

/*cmd -> IF expr THEN { cmdblock } [ ELSE cmdblock ] ENDIF
*      | WHILE expr DO cmdblock ENDWHILE
*			 | DO cmdblock WHILE expr
*			 | expr
*			 | <empty>
*/
void cmd(void){
	switch (lookahead) {
		case IF:
			ifstmt();
			break;
		case WHILE:
			whilestmt();
			break;
		case DO:
			dostmt();
			break;
		case ID: case DEC: case '(': 
			expr();
			break;
		
	}
}


//IF expr THEN cmdblock { ELIF expr THEN cmdblock } [ ELSE cmdblock ] ENDIF
void ifstmt(void){
	match(IF);
	expr();
	match(THEN);
	cmdblock();
	while (lookahead == ELIF) {
		match(ELIF);
		expr();
		match(THEN);
		cmdblock();
	}

	if(lookahead == ELSE){
		match(ELSE);
		cmdblock();
	}
	match(ENDIF);

}

//| WHILE expr DO cmdblock DONE
void whilestmt(void){
	match(WHILE);
	expr();
	match(DO);
	cmdblock();
	match(DONE);
}

//| DO cmdblock WHILE expr
void dostmt(void){
	match(DO);
	cmdblock();
	match(WHILE);
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
		match(ID); // atribuição
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
int addop (void)
{
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
