#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	MEM_ERR,
	TOO_LARGE,
	INVALID_ARG,
	IO,
	PROGRAM_ERR
} CGP_ERR;

void cgp_throw(const CGP_ERR error, const char* message);