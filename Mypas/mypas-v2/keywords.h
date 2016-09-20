/**@<keywords.h>::**/

enum {
	BEGIN = 0x4097,
	IF,
	THEN,
	ELSE,
	WHILE,
	REPEAT,
	UNTIL,
	END,
	PROCEDURE,
	FUNCTION,
	VAR,
	INT,
	FLOAT,
	HEXA,
	OCTA
};

extern char *keywords[];

extern int iskeyword( char const *identifier);
