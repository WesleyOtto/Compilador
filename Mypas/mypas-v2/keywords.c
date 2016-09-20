/**@<keywords.c>::**/

#include <string.h>
#include <keywords.h>

char *keywords[] = {"begin", "if", "then", "else", "while", "repeat", "until", "end", "procedure", "function", "var", "int", "float", "hexa", "octal"};

int iskeyword ( char const *identifier) {
	int token;
	for ( token = BEGIN; token <= OCTA; token++) {
		if ( strcmp ( keywords [token-BEGIN], identifier) == 0 ) return token;
	}
	return 0 ;
}
