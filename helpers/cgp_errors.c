#include "cgp_errors.h"

void cgp_throw(CGP_ERR error) {
	printf("Error: ");
	switch (error) {
		case MEM_ERR:
			printf("Memory error.");
			break;

		case TOO_LARGE:
			printf("Tried to use too much memory.");
			break;

		default:
			printf("Uknown.");
			break;
	}
	printf("\n");
}