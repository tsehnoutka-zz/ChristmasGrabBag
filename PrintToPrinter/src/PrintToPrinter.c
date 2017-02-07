/*
 ============================================================================
 Name        : PrintToPrinter.c
 Author      : Tim Edwards
 Version     :
 Copyright   : BE NICE!
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
const int PAGEFEED=12;

int main(void) {
	puts("Print To Printer"); /* prints Print To Printer */

	FILE *printer = popen("lpr", "w");
	fprintf(printer, "This is my message to the Printer from CentOS 7\n%c", PAGEFEED);
	pclose(printer);



	return EXIT_SUCCESS;
}
