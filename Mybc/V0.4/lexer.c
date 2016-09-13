/**@<lexer.c>::**/

/*

1: Tue Aug 16 20:49:40 BRT 2016

 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>

void skipspaces (FILE *dish)
{
        int cake;

        while ( isspace ( cake = getc (dish) ) );

        ungetc ( cake, dish );
}

char lexeme[MAXID_SIZE+1];//@ lexer.c

int is_identifier(FILE *dish) {
        int i = 0;

        lexeme[i] = getc(dish);
        if (isalpha (lexeme[i]) ) {
                for (i++; isalnum (lexeme[i] = getc(dish)); i++) {
			if(i > MAXID_SIZE) i = MAXID_SIZE;
		}
                ungetc (lexeme[i], dish);
                lexeme[i] = '\0';
                return ID;
        }
        ungetc (lexeme[i], dish);
        return 0;
}

int is_hexadecimal(FILE *tape) {
	int i = 0;

	lexeme[i] = getc(tape);
	if(lexeme[i] == '0') {
		++i;

		lexeme[i] = getc(tape);
		if( (toupper(lexeme[i])) == 'X') {
			++i;
			lexeme[i] = getc(tape);
			if(isdigit(lexeme[i]) || (toupper(lexeme[i]) >= 'A' && toupper(lexeme[i]) <= 'F') ) {
				for(++i; ( isdigit( lexeme[i] = getc(tape) ) ) || (toupper(lexeme[i]) >= 'A' && toupper(lexeme[i]) <= 'F'); ++i) {
					if(i >= MAXID_SIZE) i = MAXID_SIZE-1;
				}
				ungetc(lexeme[i], tape);
				lexeme[i] = '\0';
				return HEX;
			}
			ungetc(lexeme[i], tape);
			--i;
			ungetc(lexeme[i], tape);
			--i;
			ungetc(lexeme[i], tape);
			return 0;
		}
		else{
			ungetc(lexeme[i], tape);
			--i;
			ungetc(lexeme[i], tape);
			return 0;
		}
	}
	ungetc(lexeme[i], tape);
	return 0;
}

int is_octal(FILE *tape) {
        int i = 0;

	lexeme[i] = getc(tape);
        if (lexeme[i] == '0') {
		++i;
		lexeme[i] = getc(tape);
                if ( lexeme[i] >= '0' && lexeme[i] <= '7') {
			for(++i; (lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7'; ++i) {
				if(i >= MAXID_SIZE) i = MAXID_SIZE-1;
			}
                        ungetc (lexeme[i], tape);
			lexeme[i] = '\0';
                        return OCTAL;
                } else {
                        ungetc (lexeme[i], tape);
			--i;
                        ungetc (lexeme[i], tape);
                        return 0;
                }
        }
        ungetc (lexeme[i], tape);
        return 0;
}

int is_decimal(FILE *tape) {
	int i = 0;
	if (isdigit (lexeme[i] = getc(tape) ) ) {
		if (lexeme[i] == '0') {
			++i;
			lexeme[i] = getc(tape);
			return i;
		}
		// [0-9]*
		for(++i; isdigit( lexeme[i] = getc(tape) ); ++i) {
			if(i >= MAXID_SIZE) i = MAXID_SIZE-1;
		}
		return i;
	}
	return 0;
}

int is_exp(FILE *tape, int *count) {

        if( toupper( lexeme[(*count)] ) == 'E') {
                ++(*count);
                if( ( lexeme[(*count)] = getc(tape) )== '+' || lexeme[(*count)]== '-') {
                        ++(*count);
                        if( isdigit( lexeme[(*count)]  = getc(tape) )) {
                                for ((*count)++; isdigit (lexeme[(*count)] = getc(tape)); (*count)++) {
					if((*count) >= MAXID_SIZE) (*count) = MAXID_SIZE-1;
				}
                                return 1;
                        }
                }
                else if(isdigit(lexeme[(*count)])) {
					for ((*count)++; isdigit (lexeme[(*count)] = getc(tape)); (*count)++) {
						if((*count) >= MAXID_SIZE) (*count) = MAXID_SIZE-1;
					}
                        return 1;
                }
                ungetc(lexeme[(*count)], tape);
				--(*count);
                return 0;
        }
        return 0;
}

int is_float(FILE *tape) {
        int i = 0;

        if( (i = is_decimal(tape)) ) { //dec
                if( lexeme[i] == '.') { //dec.
                        for (i++; isdigit (lexeme[i] = getc(tape)); i++) {
				if(i >= MAXID_SIZE) i = MAXID_SIZE-1;
			}
                        is_exp(tape, &i);
                        ungetc(lexeme[i], tape);
			lexeme[i] = '\0';
                        return FLT;
                }
                else if(is_exp(tape, &i)) { //dec exp
			ungetc(lexeme[i], tape);
                        lexeme[i] = '\0';
                        return FLT;
                }
		ungetc(lexeme[i], tape);
                lexeme[i] = '\0';
                return DEC;
        }
        else if(lexeme[i] == '.') {
                ++i;
		lexeme[i] = getc(tape);
                if( isdigit(lexeme[i]) ) { //.digit
                        for (i++; isdigit (lexeme[i] = getc(tape)); i++) {
				if(i >= MAXID_SIZE) i = MAXID_SIZE-1;
			}
                        is_exp(tape, &i);
			ungetc(lexeme[i], tape);
                        lexeme[i] = '\0';
                        return FLT;
                }
        }
        ungetc(lexeme[i], tape);
        lexeme[i] = '\0';
        return 0;
}

int gettoken(FILE *tokenstream) {
	int token;

	skipspaces (tokenstream);

	if ( token = is_identifier(tokenstream) ) {
		return ID;
	}

	if (token = is_octal(tokenstream) ) {
		return OCTAL;
	}

	if (token = is_hexadecimal(tokenstream) ) {
		return HEX;
	}

	if ( token = is_float (tokenstream) ) {
		return token;
	}

	token = getc (tokenstream);

	return token;
}
