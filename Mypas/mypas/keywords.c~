/**@<keywords.c>::**/
#include <string.h>
#include <keywords.h>

char *keywords[] = {"if","then","elif","else","endif","while","do","done"};

int iskeyword ( char const *identifier)
{
	int token;
	for ( token = IF; token <= DONE; token++) {
		if ( strcmp ( keywords [token-IF], identifier) == 0 ) return token;
	}
	return 0 ;
}
