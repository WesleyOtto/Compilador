#define	MAXSYMTAB_SIZE	0x10000
#define	MAXSTACK_SIZE	64

extern	int		symtab_nextentry;
extern	char	symtab[MAXSYMTAB_SIZE][MAXID_SIZE+1];
extern	double	memtab[MAXSYMTAB_SIZE];

extern	int	sp;
extern	double	stack[MAXSTACK_SIZE];
extern	double	accumulator;
extern	double	auxregister;


void push(double value);
double pop(void);
void enter(double  value);
int search_symbol(char *identifier);
void insert_symbol(char *identifier);
void operation(int op);
