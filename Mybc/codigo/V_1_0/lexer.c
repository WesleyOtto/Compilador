/**@<lexer.c>::**/

/*

1: Tue Aug 16 20:49:40 BRT 2016

 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>

void skipspaces (FILE *dish)
{
        int cake;

        while ( isspace ( cake = getc (dish) ) );

        ungetc ( cake, dish );
}

int is_identifier(FILE *dish)
{
        int cake = getc(dish);
        if (isalpha (cake) ) {
                while (isalnum (cake = getc(dish)));
                ungetc (cake, dish);
                return ID;
        }
        ungetc (cake, dish);
        return 0;
}

int is_decimal(FILE *dish)
{
        int cake = getc(dish);
        if (isdigit (cake) ) {
                if (cake == '0') {
                        return DEC;
                }
                // [0-9]*
                while ( isdigit (cake = getc(dish)) );
                ungetc (cake, dish);
                return DEC;
        }
        ungetc (cake, dish);
        return 0;
}

int is_octal(FILE *dish)
{
        int octpref = getc(dish);
        if (octpref == '0') {
                int cake = getc(dish);
                if ( cake >= '0' && cake <= '7') {
                        while ( (cake = getc(dish)) >= '0' && cake <= '7');
                        ungetc (cake, dish);
                        return OCTAL;
                } else {
                        ungetc (cake, dish);
                        ungetc (octpref, dish);
                        return 0;
                }
        }
        ungetc (octpref, dish);
        return 0;
}

int gettoken (FILE *tokenstream)
{
        int token;

        skipspaces (tokenstream);

        if ( token = is_identifier(tokenstream) ) {
                return ID;
        }

        if ( token = is_decimal (tokenstream) ) {
                return DEC;
        }

        token = getc (tokenstream);

        return token;
}
