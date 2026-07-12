#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 30 maximum options
#define MAX_OPTIONS 30

extern const char* CGP_OPTIONS[MAX_OPTIONS][3];

void cgp_printHelp(void);
bool IntArray_includes(int array[], unsigned int length, int item);
bool StringArray_includes(char* array[], unsigned int length, char* item);
int64_t IntArray_indexOf(int array[], unsigned int length, int item);
int64_t StringArray_indexOf(char* array[], unsigned int length, char* item);

typedef struct
{
	int* array;
	unsigned int length;
} DynArrayInt;

DynArrayInt new_DynArrayInt(int* array, unsigned int initialLength);
unsigned int DynArrayInt_push(DynArrayInt* dynamicArray, int item);
unsigned int DynArrayInt_pop(DynArrayInt* dynamicArray);

typedef struct
{
	char **array;
	unsigned int length;
} DynArrayString;