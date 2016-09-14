/**@<parser.h>::**/

extern int lookahead; // @ local

extern int gettoken (FILE *); // @ lexer.c

extern FILE *source; // @ main.c

extern void match (int); // @ local

/* syntax names */
void progcalc();
void cmd();
void cmdblock();
int cmdsep();
void ifstmt();
void whilestmt();
void dostmt();
void expr (void);
int addop (void);
int mulop (void);

