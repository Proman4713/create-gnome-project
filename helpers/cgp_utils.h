// TODO: Find alternative
#include <cs50.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cgp_printHelp(void);
char* get_string_withDefault(char* prompt, char* defaultValue);
char* String_toLowercase(char* string);
bool String_startsWith(char* string, char* prefix);
bool String_endsWith(char* string, char* suffix);
bool IntArray_includes(int array[], unsigned int length, int item);
bool StringArray_includes(char* array[], unsigned int length, char* item);
int64_t IntArray_indexOf(int array[], unsigned int length, int item);
int64_t StringArray_indexOf(char* array[], unsigned int length, char* item);

typedef struct {
	int* array;
	unsigned int length;
} DynIntArray;

DynIntArray new_DynIntArray(int* array, unsigned int initialLength);
unsigned int DynIntArray_push(DynIntArray* dynamicArray, int item);
unsigned int DynIntArray_pop(DynIntArray* dynamicArray);
bool DynIntArray_includes(DynIntArray* dynamicArray, int item);
int64_t DynIntArray_indexOf(DynIntArray* dynamicArray, int item);

typedef struct {
	char** array;
	unsigned int length;
} DynStringArray;