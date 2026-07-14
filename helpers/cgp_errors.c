#include "cgp_errors.h"

void cgp_throw(const CGP_ERR error, const char* message) {
	printf("\x1b[31m\x1b[1m[Error]\x1b[31m\x1b[0m ");
	switch (error) {
		case MEM_ERR:
			printf("Memory error");
			break;

		case TOO_LARGE:
			printf("Tried to use too much memory");
			break;

		case INVALID_ARG:
			printf("Invalid Argument");
			break;

		case IO:
			printf("Input/Output");
			break;

		case PROGRAM_ERR:
			printf("Program Error");
			break;

		default:
			printf("Uknown");
			break;
	}
	if (strlen(message)) {
		printf(": %s", message);
	} else {
		printf(".");
	}
	printf("\n");
	exit(1);
}