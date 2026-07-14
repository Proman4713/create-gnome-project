// TODO: Find alternative
#include <cs50.h>
#include <ctype.h>
#include <curl/curl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_string_withDefault(char* prompt, char* defaultValue);
CURLcode downloadFile(FILE* file, char* URI);
char* path_join(char* paths[], size_t length);
char* String_toLowercase(char* string);
char* String_toUppercase(char* string);
char* String_toPascalcase(char* string);
bool String_startsWith(char* string, char* prefix);
bool String_endsWith(char* string, char* suffix);
char* String_replaceAll(char* string, char* pattern, char* replacement);
char* String_replaceAllMulti(char* string, char* patterns[], char* replacements[], size_t filterCount);
void charcpy(char* string, char character);
void charcat(char* string, char character);
char* Int_toString(int32_t number);
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