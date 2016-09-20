/**@<parser.h>::**/

extern int lookahead; // @ local

extern int gettoken (FILE *); // @ lexer.c

extern FILE *source; // @ main.c

extern void match (int); // @ local

/* syntax names */
void mypas(void);
void body(void);
void namelist(void);
void vartype(void);
void parmdef(void);
void declarative(void);
void imperative(void);
void stmtlist(void);
void stmt(void);
void ifstmt(void);
void whilestmt(void);
void repstmt(void);

void expr (void);
int addop (void);
int mulop (void);

