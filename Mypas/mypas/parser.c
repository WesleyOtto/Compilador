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
 * mypas -> body'.'
 *
 * body -> declarative imperative
 *
 * declarative -> [ VAR namelist ':' vartype ';' { namelist ':' vartype ';' } ]
 *               { sbpmod sbpname parmdef [ ':' fnctype ] ';' body ';' }  
 *                                        '- só se chamar FUNCTION ai chama fnctype 
 * sbpmod -> PROCEDURE | FUNTCION
 *
 * sbpname -> ID
 *
 * parmdef -> [ ( [VAR] namelist ':' vartype { ';' [VAR] namelist ':' vartype } ) ]
 *
 * namelist -> ID {, ID}
 *
 * imperative -> BEGIN stmtlist END
 *
 * stmtlist -> stmt { ';' stmt }
 *
 * stmt -> imperative
 *	| IF expr THEN { stmt } [ ELSE stmt ]
 *	| WHILE expr DO stmt
 *	| REPEAT stmtlist UNTIL expr // mudar la nos token
 *	| expr
 *	| <empty>
 *************************************************************************************/
//mypas -> body'.'
void mypas(void){
	body();
	match('.');
}

//body -> declarative imperative
void body(void){
	declarative();
	imperative();
}

void namelist(void){
	match(ID);
	while(lookahead == ',') {
		match(',');
		match(ID);
	}
}

void vartype(void){
	switch(lookahead){
		case DEC:
			match(DEC);
		break;

		case FLT:
			match(FLT);
		break;

		case HEX:
			match(HEX);
		break;

		case OCTAL:
			match(OCTAL);
		break;
	}
}
// parmdef -> [ ( [VAR] namelist ':' vartype { ';' [VAR] namelist ':' vartype } ) ]
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

//declarative -> [ VAR namelist ':' vartype ';' { namelist ':' vartype ';' } ]
//               { sbpmod sbpname parmdef [ ':' fnctype ] ';' body ';' }  
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
		if(lookahead == ':'){
			if(sbpmod == FUNCTION && lookahead == ':'){
				match(':');
				fnctype();
			}			

			match(';');
			body();
			match(';');
		}
	}
}
/*stmt -> imperative
	| IF expr THEN { stmt } [ ELSE stmt ]
*	| WHILE expr REPEAT stmt
*	| REPEAT stmtlist UNTIL expr // mudar la nos token
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
void repstmt(void){
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
