/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <string.h>

#define	MAXSYMTAB_SIZE	0x10000 
int	symtab_nextentry = 0;
char 	symtab[MAXSYMTAB_SIZE][MAXID_SIZE+1];
double	memtab[MAXSYMTAB_SIZE];
#define MAXSTACK_SIZE	64
int 	sp = -1;
double 	stack[MAXSTACK_SIZE];
double 	accumulator;

/*************************** LL(1) grammar definition ******************************
 *
 * mybc -> cmd { ; cmd} \n | <eof>
 *
 * cmd -> expr
 *
 * expr -> term {addop term}
 *
 * term -> fact {mulop fact}
 *
 * fact -> vrbl | cons | ( expr )
 *
 * vrbl -> ID
 *
 * cons -> DEC
 *
 * addop -> '+' | '-'
 *
 * mulop -> '*' | '/'
 *
 * ID = [A-Za-z][A-Za-z0-9]*
 *
 * DEC = [1-9][0-9]* | 0
 *
 **********************************************************************************/

/***************************** LL(1) grammar emulation *****************************
 *
 *      source language        ||          object language
 * -----------------------------------------------------------------------
 * expr -> term { addop term } || expr.pf := term.pf { term.pf addop.pf }
 * -----------------------------------------------------------------------
 * expr -> term { addop term [[ printf(addop.pf); ]] }
 */
void expr (void)
{       /**/int op, neg=0/**/;
	if(lookahead == '-'){
	 	match('-');
	/**/	neg = '-' /**/;
	} 
        term();/**/ if(neg){ printf("<+/-> ");}/**/
	while( op = addop() ) {
	/**/	printf("<enter> ")/**/;
		term();
	/**/	printf("<%c> ",op)/**/;
	}
}
/*
 * term -> fact { mulop fact } || term.pf := fact.pf { fact.pf mulop.pf }
 */
void term (void)
{       /**/int op/**/;
        fact(); 
	while( op = mulop() ) { 
	/**/	printf("<enter> ")/**/; 
		fact();
	/**/	printf("<%c> ",op)/**/;
	}
}
/*
 * fact -> vrbl | cons | ( expr ) || fact.pf := expr.pf */
void fact (void)
{	char bkplexeme[MAXID_SIZE+1];

        switch (lookahead) {
        case ID:
		stackUpdate();

		int symbolMemAddress = getSymbolMemAddress(lexeme);
		if( symbolMemAddress >= 0 ){
			accumulator = memtab[symbolMemAddress];
		} else {
			allocateSymbol();
                        accumulator = memtab[symtab_nextentry-1];
		}

		strcpy(bkplexeme, lexeme);
		printf("%s ", bkplexeme);
		match(ID);
		if(lookahead == '='){
			match('=');
			expr();
                /**/	printf("<store> ")/**/; match (ID); 
		} 
		break;
        case DEC:
		stackUpdate();
		accumulator = atoi(lexeme);
                /**/printf("%s ", lexeme )/**/; match (DEC);
	 break;
        default:
                match ('('); expr(); match (')');
        }
}


/*
	TODO - MUDAR PRA OUTRO ARQUIVO
*/
int getSymbolMemAddress(char* symbol)
{
	int i = 0;
	for(i = 0; i < MAXSYMTAB_SIZE; i++){
		if(strcmp(symbol, symtab[i]) == 0)
			return i;
	}

	return -1;
}

void allocateSymbol()
{
	strcpy(symtab[symtab_nextentry], lexeme);
	memtab[symtab_nextentry] = 0;

	symtab_nextentry++;
}

void stackUpdate()
{
	sp++;
	stack[sp] = accumulator;
}


/*
 * vrbl -> ID
 *
 * cons -> DEC
*
 * ID = [A-Za-z][A-Za-z0-9]*
 *V_1_4/
 * DEC = [1-9][0-9]* | 0
 *
 * addop -> '+' | '-' */
int addop (void)
{
        switch(lookahead){
        case '+':
                        match('+'); return '+';
        case '-':
                        match('-'); return '-';
        }
        return 0;
}

/*
 * mulop -> '*' | '/' */
int mulop (void)
{
        switch(lookahead){
        case '*':
                        match('*'); return '*';
        case '/':
                        match('/'); return '/';
        }
        return 0;
}
/***************************** lexer-to-parser interface **************************/

int lookahead; // @ local

void match (int expected)
 {
         if ( expected == lookahead) {
                 lookahead = gettoken (source);
         } else {
                 fprintf(stderr,"parser: token mismatch error. found # %d ",
                        lookahead);
                 fprintf(stderr,"whereas expected # %d\n",
                        expected);
                 exit (SYNTAX_ERR);
         }
 }

