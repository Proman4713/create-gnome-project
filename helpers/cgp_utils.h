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
char* get_string_withDefault(const char* prompt, const char* defaultValue);
char* String_toLowercase(const char* string);
bool IntArray_includes(const int array[], const unsigned int length, const int item);
bool StringArray_includes(const char* array[], const unsigned int length, const char* item);
int64_t IntArray_indexOf(const int array[], const unsigned int length, const int item);
int64_t StringArray_indexOf(const char* array[], const unsigned int length, const char* item);

typedef struct {
	int* array;
	unsigned int length;
} DynIntArray;

DynIntArray new_DynIntArray(const int* array, const unsigned int initialLength);
unsigned int DynIntArray_push(DynIntArray* dynamicArray, const int item);
unsigned int DynIntArray_pop(DynIntArray* dynamicArray);
bool DynIntArray_includes(DynIntArray* dynamicArray, const int item);
int64_t DynIntArray_indexOf(DynIntArray* dynamicArray, const int item);

typedef struct {
	char** array;
	unsigned int length;
} DynStringArray;