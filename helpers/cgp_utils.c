#include "cgp_utils.h"
#include "cgp_errors.h"

char* get_string_withDefault(char* prompt, char* defaultValue) {
	char* response = get_string("%s\x1b[90m(%s)\x1b[0m ", prompt, defaultValue);

	if (strlen(response) < 1)
		return defaultValue;
	else
		return response;
}

// Requires freeing
char* String_toLowercase(char* string) {
	char* newString = malloc((strlen(string) + 1) * sizeof(char));
	if (newString == NULL) {
		cgp_throw(MEM_ERR, "");
	}
	for (int i = 0; string[i] != '\0'; i++) {
		char curChar = string[i];
		if (isupper(curChar)) {
			newString[i] = tolower(curChar);
		} else {
			newString[i] = curChar;
		}
	}
	newString[strlen(string)] = '\0';
	return newString;
}

bool String_startsWith(char* string, char* prefix) {
	bool matches = true;
	size_t prefixLen = strlen(prefix);
	for (int i = 0; i < prefixLen; i++) {
		if (string[i] != prefix[i]) {
			matches = false;
		}
	}
	return matches;
}

bool String_endsWith(char* string, char* suffix) {
	bool matches = true;
	size_t suffixLen = strlen(suffix);
	size_t stringLen = strlen(string);
	if (suffixLen > stringLen)
		return false;

	for (int i = strlen(string) - 1; i > (stringLen - suffixLen - 1); i--) {
		if (string[i] != suffix[i - (stringLen - suffixLen)]) {
			matches = false;
		}
	}
	return matches;
}

// JavaScript `Array.prototype.includes()` equivalent
bool IntArray_includes(int array[], unsigned int length, int item) {
	for (int i = 0; i < length; i++) {
		if (array[i] == item)
			return true;
	}
	return false;
}

// JavaScript `Array.prototype.includes()` equivalent
bool StringArray_includes(char* array[], unsigned int length, char* item) {
	for (int i = 0; i < length; i++) {
		if (strcmp(array[i], item) == 0)
			return true;
	}
	return false;
}

//* 64-bit integer to fit all `unsigned int` values plus negative values for the -1 and avoid overflows
// JavaScript `Array.prototype.indexOf()` equivalent
int64_t IntArray_indexOf(int array[], unsigned int length, int item) {
	for (int i = 0; i < length; i++) {
		if (array[i] == item)
			return i;
	}
	return -1;
}

// JavaScript `Array.prototype.indexOf()` equivalent
int64_t StringArray_indexOf(char* array[], unsigned int length, char* item) {
	for (int i = 0; i < length; i++) {
		if (strcmp(array[i], item) == 0)
			return i;
	}
	return -1;
}

//^ My own OOP coding style as a JS and C# dev sneaking in

//* Dynamic integer arrays
DynIntArray new_DynIntArray(int* array, unsigned int initialLength) {
	if (array == NULL || initialLength == 0) {
		DynIntArray newArray;
		newArray.array = NULL;
		newArray.length = 0;
		return newArray;
	}

	DynIntArray newArray;
	newArray.array = malloc(initialLength * sizeof(int));
	if (newArray.array == NULL)
		cgp_throw(MEM_ERR, "");

	newArray.length = initialLength;

	for (int i = 0; i < initialLength; i++) {
		newArray.array[i] = array[i];
	}

	return newArray;
}

//* Push an element to a dynamic integer array
unsigned int DynIntArray_push(DynIntArray* dynamicArray, int item) {
	if (dynamicArray->length + 1 > UINT_MAX)
		cgp_throw(TOO_LARGE, "");

	DynIntArray newArray;
	unsigned int newLength = dynamicArray->length + 1;

	int* newPtr = realloc(dynamicArray->array, newLength * sizeof(int));
	if (newPtr == NULL)
		cgp_throw(MEM_ERR, "");

	// Set last element to new item
	newPtr[dynamicArray->length] = item;

	newArray.array = newPtr;
	newArray.length = newLength;
	*dynamicArray = newArray;

	return newLength;
}

//* Pop the last element of a dynamic integer array
unsigned int DynIntArray_pop(DynIntArray* dynamicArray) {
	DynIntArray newArray;
	unsigned int newLength = dynamicArray->length - 1;

	// This frees the last element, basically deleting it
	int* newPtr = realloc(dynamicArray->array, newLength * sizeof(int));
	if (newPtr == NULL)
		cgp_throw(MEM_ERR, "");

	newArray.array = newPtr;
	newArray.length = newLength;
	*dynamicArray = newArray;

	return newLength;
}

bool DynIntArray_includes(DynIntArray* dynamicArray, int item) {
	for (int i = 0; i < dynamicArray->length; i++) {
		if (dynamicArray->array[i] == item)
			return true;
	}
	return false;
}

int64_t DynIntArray_indexOf(DynIntArray* dynamicArray, int item) {
	for (int i = 0; i < dynamicArray->length; i++) {
		if (dynamicArray->array[i] == item)
			return i;
	}
	return -1;
}

//* Dynamic string arrays
// TODO: String dynamic arrays