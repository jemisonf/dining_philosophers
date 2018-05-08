default:
	gcc -pthread -Wall -std=c99 conc_2.c -o conc_2
verbose:
	gcc -pthread -std=c99 -Wall conc_2.c -o conc_2 -D VERBOSE 
