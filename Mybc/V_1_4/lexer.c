/**@<lexer.c>::**/

/*

1: Tue Aug 16 20:49:40 BRT 2016

 */

// Dish = tape / Cake = lexeme //

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
                for( i++; isdigit (lexeme[i] = getc(tape)) && i<= MAXID_SIZE; i++ );
                        /*if(i >= MAXID_SIZE){
                                int head = getc(tape);
                                while ( isdigit (head = getc(tape)) );
                                ungetc (head, tape);
                                return DEC;
                        } */
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
                        for ( i++; (lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7'&& i<= MAXID_SIZE; i++);
                           /*     if(i >= MAXID_SIZE){
                                        int head = getc(tape);
                                        while ( (head = getc(tape)) >= '0' && head <= '7');
                                        ungetc (head, tape);
                                        return OCTAL;
                                } */
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
                int i =0;
                lexeme[i] = hexzero;
                i++;
                int hexX = getc(tape);

                if( (toupper(hexX)) == 'X') {
                        lexeme[i] = hexX;
                        i++;
                        lexeme[i] = getc(tape);

                        if(isdigit(lexeme[i] = getc(tape)) || (toupper(lexeme[i]) >= 'A' && toupper(lexeme[i]) <= 'F') ) {
                                for(i++; isdigit(lexeme[i] = getc(tape)) || (toupper(lexeme[i]) >= 'A' && toupper(lexeme[i]) <= 'F' && i<= MAXID_SIZE ); i++);
                                        /*if(i >= MAXID_SIZE){
                                                int head = getc(tape);
                                                while(isdigit(head = getc(tape)) || (toupper(head) >= 'A' && toupper(head) <= 'F'));
                                                ungetc(head, tape);
                                                return HEX;
                                        } */
                                ungetc(lexeme[i], tape);
                                lexeme[i] = 0;
                                return HEX;
                                
                        }else{
                                ungetc(hexzero, tape);
                                ungetc(hexX, tape);
                                ungetc(lexeme[i], tape);
                        }
                }
                else{
                        ungetc(hexzero, tape);
                        ungetc(hexX, tape);
                        return 0;
                }
        }
        ungetc(hexzero, tape);
        return 0;
}

int is_float(FILE *tape){
        int i = 0;

        lexeme[i] = getc(tape);

        if( is_decimal(tape) ){ //inicia com dec
                if(lexeme[i] == '.'){
                        for(i++; isdigit(lexeme[i] = getc(tape)); i++){
                                if(i >= MAXID_SIZE){
                                        int head = getc(tape);
                                        while ( isdigit (head = getc(tape)) );
                                        ungetc (head, tape);
                                        return FLT;
                                }
                                //aqui verifica se é exp
                                ungetc(lexeme[i], tape);
                                lexeme[i] = 0;
                                return FLT;
                        }
                } /*else if ()  //verificar se é exp */ 
        } else if ( lexeme[i] == '.'){
                if( isdigit(lexeme[i])){ //condição aceitavel ( .digit )
                        for(i++; isdigit(lexeme[i] = getc(tape)) && i<= MAXID_SIZE; i++);
                                /* if(i >= MAXID_SIZE){
                                        int head = getc(tape);
                                        while ( isdigit (head = getc(tape)) );
                                        ungetc (head, tape);
                                        return FLT;
                                }*/

                        ungetc(lexeme[i], tape);
                        lexeme[i] = 0;
                        return FLT;
                        
                }
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

        if ( token = is_decimal (tokenstream) ) {
                return DEC;
        }

        /*
        if ( token = is_octal (tokenstream) ) {
                return OCTAL;
        }

        if ( token = is_hexadecimal (tokenstream) ) {
                return HEX;
        }

        if ( token = is_float (tokenstream) ) {
                return FLT;
        }
        */
        token = getc (tokenstream);

        return token;
}
