/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <string.h>
#include <interpreter.h>

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

void cmd (void){
  accumulator = 0;
  expr();

  printf("\nResult: < %0.2f >\n\n", accumulator);

  if (lookahead == ';') {
    match(';');
    cmd();
  }
}

/***************************** LL(1) grammar emulation *****************************
 *
 *      source language        ||          object language
 * -----------------------------------------------------------------------
 * expr -> term { addop term } || expr.pf := term.pf { term.pf addop.pf }
 * -----------------------------------------------------------------------
 * expr -> term { addop term [[ printf(addop.pf); ]] }
 */
void expr (void)
{
   /**/int op, neg=0/**/;
   if(lookahead == '-'){
      match('-');
      /**/neg = '-'/**/;
   }

   term();/**/ if(neg){ accumulator *= -1; printf("<+/-> "); }/**/
   while( op = addop() ) {
      /**/printf("<enter> ")/**/;
      push();
      term();
      /**/printf("<%c> ",op)/**/;
      accumulator = operation(op, pop(), accumulator);
   }
}
/*
 * term -> fact { mulop fact } || term.pf := fact.pf { fact.pf mulop.pf }
 */
void term (void)
{
   /**/int op/**/;
   fact();
   while( op = mulop() ) {
      /**/printf("<enter> ")/**/;
      push();
      fact();
      /**/printf("<%c> ", op)/**/;
      accumulator = operation(op, pop(), accumulator);
	}
}
/*
 * fact -> vrbl | cons | ( expr ) || fact.pf := expr.pf */
void fact (void)
{
   char bkplexeme[MAXID_SIZE+1];
   int symbolMemAddress;

   switch (lookahead) {
   case ID:
      symbolMemAddress = lookup(lexeme);
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
         store(bkplexeme); /**/printf("<store> ")/**/;
      }
      break;

   case FLT:
   case HEX:
   case DEC:
   case OCTAL:
      accumulator = atof(lexeme);

      /**/printf("%s ", lexeme)/**/; match (lookahead);
      break;

    default:
            match ('('); expr(); match (')');
    }
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
         if (expected == lookahead) {
                 lookahead = gettoken (source);
         } else {
                 fprintf(stderr,"parser: token mismatch error. found # %d <%s> ",
                        lookahead, lexeme);
                 fprintf(stderr,"whereas expected # %d <%c>\n",
                        expected, expected);
                 exit (SYNTAX_ERR);
         }
 }
