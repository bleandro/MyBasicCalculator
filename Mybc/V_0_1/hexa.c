#include <ctype.h>
#include <stdio.h>

int is_hexadecimal(FILE *tape) {
	int hexzero = getc(tape);

	if(hexzero == '0') {
		int hexX = getc(tape);

		if( (toupper(hexX)) == 'X') {
			int head = getc(tape);

			if(isdigit(head = getc(tape)) || (toupper(head) >= 'A' && toupper(head) <= 'F') ) {
				while(isdigit(head = getc(tape)) || (toupper(head) >= 'A' && toupper(head) <= 'F'));
				ungetc(head, tape);
				return HEX;
			}else{
				ungetc(hexzero, tape);
				ungetc(hexX, tape);
				ungetc(head, tape);
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
