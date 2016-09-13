/**@<mybc.c>::**/

/*
1: Tue Aug 16 20:50:47 BRT 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <mybc.h>

FILE *source;

extern int lookahead; // @ parser.c

main (int argc, char *argv[], char *envp[])
{
        if (argc == 1) {
                source = stdin;
        } else {
                source = fopen (argv[1], "r");
                if (source == NULL) {
                        fprintf (stderr, "%s: cannot open %s... exiting\n",
                                argv[0], argv[1]);
                        exit (-1);
                }
        }

        lookahead = gettoken (source);

        expr ();

        printf("\n");

        exit (0);
}
