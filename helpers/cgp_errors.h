#include <stdio.h>

typedef enum
{
	MEM_ERR,
	TOO_LARGE
} CGP_ERR;

void cgp_throw(CGP_ERR error);