/**@<keywords.h>::**/

enum {
	IF = 0x4097, THEN, ELIF, ELSE, ENDIF, WHILE, DO, DONE
};

extern char *keywords[];

extern int iskeyword( char const *identifier);
