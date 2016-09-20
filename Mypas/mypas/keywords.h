/**@<keywords.h>::**/

enum {
	BEGIN =  = 0x4097, IF, THEN, ELSE, ENDIF, WHILE, DO, REPEAT, UNTIL, PROCEDURE, FUNCTION, DONE
};

extern char *keywords[];

extern int iskeyword( char const *identifier);
