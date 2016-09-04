/**@<interpreter.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <string.h>
#include <interpreter.h>

int	symtab_nextentry = 0;
char 	symtab[MAXSYMTAB_SIZE][MAXID_SIZE+1];
double	memtab[MAXSYMTAB_SIZE];

int sp = -1;
double 	stack[MAXSTACK_SIZE];
double 	accumulator = 0;

void store(char* storeSymbol){
  int symbolMemAddress = lookup(storeSymbol);

  if ( symbolMemAddress  >= 0 )
     memtab[symbolMemAddress] = accumulator;
}

int lookup(char* symbol){
  int i = 0;
  for(i = 0; i < MAXSYMTAB_SIZE; i++){
    if(strcmp(symbol, symtab[i]) == 0)
		  return i;
	}

	return -1;
}

void allocateSymbol(){
	strcpy(symtab[symtab_nextentry], lexeme);
	memtab[symtab_nextentry] = 0;

	symtab_nextentry++;
}

void push(){
  if(sp < MAXSTACK_SIZE-1){
	   sp++;
	   stack[sp] = accumulator;
  }
  else {
     fprintf(stderr, "Stack overflow.\n");
  }
}

double pop(){
  if(sp >= 0){
    double tmpAccumulator = stack[sp];
    sp--;
    return tmpAccumulator;
  }
  else {
    fprintf(stderr, "Stack underflow.\n");
  }

  return -1;
}

double operation(int op, double oprnd1, double oprnd2){
  switch (op) {
    case '+': return oprnd1 + oprnd2;
    case '-': return oprnd1 - oprnd2;
    case '*': return oprnd1 * oprnd2;
    case '/': return oprnd1 / oprnd2;
  }

  return 0;
}
