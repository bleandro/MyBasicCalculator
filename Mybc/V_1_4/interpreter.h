/**@<interpreter.h>::**/

#include <lexer.h>

#define	MAXSYMTAB_SIZE	0x10000
extern int	symtab_nextentry;
extern char 	symtab[MAXSYMTAB_SIZE][MAXID_SIZE+1];
extern double	memtab[MAXSYMTAB_SIZE];

#define MAXSTACK_SIZE	64
extern int sp;
extern double 	stack[MAXSTACK_SIZE];
extern double 	accumulator;

extern void store(char*); 
extern int lookup(char*);
extern void allocateSymbol();
extern void push();
extern double pop();
double operation(int op, double oprnd1, double oprnd2);
