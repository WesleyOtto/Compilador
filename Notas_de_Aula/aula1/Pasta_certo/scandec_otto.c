#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEC 1
#define OCTAL 2
#define ID 3
#define HEX 4


int scanid (FILE *tape)
{
	int head = getc(tape);
	if (toupper(head) >='A' && toupper(head) <= 'Z'){
		head = getc(tape);
		while((toupper(head) >='A' && toupper(head) <= 'Z')|| (head >= '0' && head <= '9'))
		{head = getc(tape);
		}
		ungetc(head, tape);
		return ID;
	}
	ungetc(head, tape);
	return 0;

}

int scandec(FILE *tape)
{
	int head = getc(tape);
	if (head >= '0' && head <= '9') {
		if (head == '0') {
			return DEC;
		}
		// [0-9]*
		while ( (head = getc(tape)) >= '0' && head <= '9');
		ungetc(head,tape);		
		return DEC;
		
	}
	ungetc(head,tape);
	return 0;
}

int scanoct (FILE *tape){
	
	int octpref = getc (tape);
	if (octpref == '0') {
		 octpref = getc(tape);
		if (octpref >= '0' && octpref <=  '7'){
			while ( (octpref = getc(tape)) >= '0' && octpref <= '7') ;
			ungetc (octpref, tape);
			return OCTAL;
		}else{ 
		       ungetc(octpref, tape);   
		       ungetc (octpref, tape);
		       return 0;
		      }
		}
			ungetc (octpref, tape);
			return 0;
}

int scanhex(FILE *tape){
	
	int head = getc (tape);
	//if (head == '0') {
		head = getc(tape);
		if (toupper(head) == 'X'){
			head = getc(tape);
			while((toupper(head) >='A' && toupper(head) <= 'F')|| (head >= '0' && head <= '9')) {
				head = getc(tape);
			}
			ungetc(head, tape);
			return HEX;
		}else{	
			ungetc(head,tape);
			ungetc(head,tape);
			return 0;
		}
	//}
	
	ungetc(head,tape);
	return 0;

}


main (int argc, char *argv[], char *envp[])
{
	FILE *buffer;

	if(argc == 1) {
		buffer = stdin;
	} else {
		buffer = fopen (argv[1], "r");
		if(buffer == NULL) {
			fprintf (stderr, "%s: cannot open %s... exiting\n",
				argv[0], argv[1]);
			exit (-1);
		}
	}
	

	if (scanid(buffer)){
		printf ("parsed string is an identifier\n");
	}else if(scanoct(buffer)) {
		printf ("parsed string is an octal\n");
	}else if(scanhex(buffer)) {
		printf ("parsed string is an hexadecimal\n");
	}else if(scandec(buffer)){
	        printf("Parsed string is a decimal\n");
	}else{
		printf ("parsed string is not a decimal,octal or hexadecimal\n");
	}


	exit (0);

}
