// TODO: Find alternative
#include <cs50.h>
#include <ctype.h>
#include <curl/curl.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
	int* array;
	unsigned int length;
} DynIntArray;

typedef struct {
	char** array;
	unsigned int length;
} DynStringArray;

char* get_string_withDefault(char* prompt, char* defaultValue);
CURLcode downloadFile(FILE* file, char* URI);
void mkdir_p(const char* path);
char* path_join(char* paths[], size_t length);
char* String_toLowercase(char* string);
char* String_toUppercase(char* string);
char* String_toPascalcase(char* string);
bool String_startsWith(char* string, char* prefix);
bool String_endsWith(char* string, char* suffix);
char* String_replaceAll(char* string, char* pattern, char* replacement);
DynStringArray String_split(const char* string, char* pattern);
char* String_replaceAllMulti(char* string, char* patterns[], char* replacements[], size_t filterCount);
void charcpy(char* string, char character);
void charcat(char* string, char character);
char* Int_toString(int32_t number);
bool IntArray_includes(int array[], unsigned int length, int item);
bool StringArray_includes(char* array[], unsigned int length, char* item);
int64_t IntArray_indexOf(int array[], unsigned int length, int item);
int64_t StringArray_indexOf(char* array[], unsigned int length, char* item);

DynIntArray new_DynIntArray(int array[], unsigned int initialLength);
unsigned int DynIntArray_push(DynIntArray* dynamicArray, int item);
unsigned int DynIntArray_pop(DynIntArray* dynamicArray);
bool DynIntArray_includes(DynIntArray* dynamicArray, int item);
int64_t DynIntArray_indexOf(DynIntArray* dynamicArray, int item);
void DynIntArray_free(DynIntArray* dynamicArray);

DynStringArray new_DynStringArray(char* array[], unsigned int initialLength);
unsigned int DynStringArray_push(DynStringArray* dynamicArray, char* item);
unsigned int DynStringArray_pop(DynStringArray* dynamicArray);
bool DynStringArray_includes(DynStringArray* dynamicArray, char* item);
int64_t DynStringArray_indexOf(DynStringArray* dynamicArray, char* item);
void DynStringArray_free(DynStringArray* dynamicArray);