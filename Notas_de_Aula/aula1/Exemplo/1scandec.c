#include <stdio.h>
#include <stdlib.h>

#define DEC 1

int scandec(FILE *tape)
{
	int head = getc(tape);
	if (head >= '0' && head <= '9') {
		if (head == '0') {
			return DEC;
		}
		// [0-9]*
		while ( (head = getc(tape)) >= '0' && head <= '9');
		return DEC;
	}
	return 0;
}

main (int argc, char *argv[], char *envp[])
{
	FILE *buffer;

	if (argc == 1) {
		buffer = stdin;
	} else {
		buffer = fopen (argv[1], "r");
		if (buffer == NULL) {
			fprintf (stderr, "%s: cannot open %s... exiting\n",
				argv[0], argv[1]);
			exit (-1);
		}
	}

	if (scandec (buffer)) {
		printf ("parsed string is a decimal\n");
	} else {
		printf ("parsed string is not a decimal\n");
	}

	exit (0);

}
