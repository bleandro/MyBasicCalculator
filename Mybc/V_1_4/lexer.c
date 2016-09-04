/**@<lexer.c>::**/

/*

1: Tue Aug 16 20:49:40 BRT 2016

 */

// Dish = tape / Cake = lexeme //

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>

void skipspaces (FILE *tape){
        int head;

        while ( isspace ( head = getc (tape) ) );

        ungetc ( head, tape );
}

char lexeme[MAXID_SIZE+1];//@ lexer.c

int is_identifier(FILE *tape){
        int i = 0;

        lexeme[i] = getc(tape);
        if (isalpha (lexeme[i]) ) {
                for (i++; isalnum (lexeme[i] = getc(tape)) && i <= MAXID_SIZE; i++);
                ungetc (lexeme[i], tape);
                lexeme[i] = 0;
                return ID;
        }
        ungetc (lexeme[i], tape);
        return 0;
}

int is_decimal(FILE *tape){
        int i =0;

        lexeme[i] = getc(tape);

        if (isdigit (lexeme[i]) ) {
                if (lexeme[i] == '0') {
                        return DEC;
                }
                // [0-9]*
                for( i++; isdigit (lexeme[i] = getc(tape)); i++ );
                ungetc (lexeme[i], tape);
                lexeme[i] = 0;
                return DEC;

        }
        ungetc (lexeme[i], tape);
        return 0;
}

int is_octal(FILE *tape){
        int octpref = getc(tape);

        if (octpref == '0') {
                int i =0;
                lexeme[i] = octpref;  //primeiro elemento recebe 0, prefixo de octal
                i++;
                lexeme[i] = getc(tape); //segundo elemento recebe o conteudo da fita

                if ( lexeme[i] >= '0' && lexeme[i] <= '7') {
                        for ( i++; (lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7'; i++);
                        ungetc (lexeme[i], tape);
                        lexeme[i] = 0;
                        return OCTAL;

                } else {
                        ungetc (lexeme[i], tape);
                        ungetc (octpref, tape);
                        return 0;
                }
        }
        ungetc (octpref, tape);
        return 0;
}

int is_hexadecimal(FILE *tape){
        int hexzero = getc(tape);
        if(hexzero == '0') {
                int i = 0;
                lexeme[i] = hexzero;
                i++;
                int hexX = getc(tape);

                if( (toupper(hexX)) == 'X') {
                        lexeme[i] = hexX;
                        i++;
                        lexeme[i] = getc(tape);

                        if(isdigit(lexeme[i]) || (toupper(lexeme[i]) >= 'A' && toupper(lexeme[i]) <= 'F') ) {
                                for(i++; isdigit(lexeme[i] = getc(tape)) || (toupper(lexeme[i]) >= 'A' && toupper(lexeme[i]) <= 'F'); i++);
                                ungetc(lexeme[i], tape);
                                lexeme[i] = 0;
                                return HEX;
                        }else{
                                ungetc(lexeme[i], tape);
                                ungetc(hexX, tape);
                                ungetc(hexzero, tape);
                        }
                }
                else{
                        ungetc(hexX, tape);
                        ungetc(hexzero, tape);
                        return 0;
                }
        }
        ungetc(hexzero, tape);
        return 0;
}

int is_decimal_float(FILE *tape){

      int i;
      if( is_decimal(tape) ){ //inicia com dec

                i = strlen(lexeme);
                lexeme[i] = getc(tape);

                if(lexeme[i] == '.'){
                        for(i++; isdigit(lexeme[i] = getc(tape)); i++);
                        ungetc(lexeme[i], tape);
                        lexeme[i] = 0;
                        return FLT;
                } //else if ()  //verificar se é exp

                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
                return DEC;
      }

      lexeme[0] = getc(tape);
      if (lexeme[0] == '.'){

                i = 1;
                if( isdigit(lexeme[i] = getc(tape)) ){ //condição aceitavel ( .digit )
                        for(i++; isdigit(lexeme[i] = getc(tape)); i++);
                        ungetc(lexeme[i], tape);
                        lexeme[i] = 0;
                        return FLT;
                }

                ungetc(lexeme[1], tape);
      }
      ungetc(lexeme[0], tape);

      return 0;
}

int hasExponencial(FILE *tape){
  int i = strlen(lexeme);
  lexeme[i] = getc(tape);

  if (toupper(lexeme[i]) == 'E'){
    i++;
    lexeme[i] = getc(tape);

    if (isdigit(lexeme[i])) {
      for(i++; isdigit(lexeme[i] = getc(tape)); i++);
      ungetc(lexeme[i], tape);
      lexeme[i] = 0;
      return FLT;
    }

    ungetc(lexeme[i], tape);
    i--;
  }

  ungetc(lexeme[i], tape);
  lexeme[i] = 0;

  return 0;
}

int gettoken (FILE *tokenstream)
{
        int token;

        skipspaces (tokenstream);

        if ( token = is_identifier(tokenstream) ) {
                return ID;
        }

        if ( token = is_hexadecimal (tokenstream) ) {
                return HEX;
        }

        if ( token = is_octal (tokenstream) ) {
                return OCTAL;
        }

        if ( token = is_decimal_float (tokenstream) ) {
          if ( hasExponencial (tokenstream) )
             return FLT;


          return token;
        }

        token = getc (tokenstream);

        return token;
}
