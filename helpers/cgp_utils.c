#include "cgp_utils.h"
#include "cgp_errors.h"

char* get_string_withDefault(char* prompt, char* defaultValue) {
	char* response = get_string("%s\x1b[90m(%s)\x1b[0m ", prompt, defaultValue);

	if (strlen(response) < 1)
		return defaultValue;
	else
		return response;
}

CURLcode downloadFile(FILE* file, char* URI) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		cgp_throw(PROGRAM_ERR, "curl not available.");
	}

	CURLcode res;
	curl_easy_setopt(
		curl,
		CURLOPT_URL,
		URI);
	curl_easy_setopt(
		curl,
		CURLOPT_WRITEFUNCTION,
		fwrite);
	curl_easy_setopt(
		curl,
		CURLOPT_WRITEDATA,
		file
	);

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return res;
}

char* path_join(char* paths[], size_t length) {
	if (length > UINT_MAX)
		cgp_throw(TOO_LARGE, "");

	size_t pathSize = 1;
	char* outputPath = malloc(pathSize * sizeof(char));
	if (outputPath == NULL) {
		cgp_throw(MEM_ERR, "");
	}
	// Empty string
	outputPath[0] = '\0';

	for (unsigned int i = 0; i < length; i++) {
		char* curStr = paths[i];
		bool isSuffixed = String_endsWith(curStr, "/");
		if (isSuffixed) {
			pathSize += strlen(curStr);
		} else {
			pathSize += ( strlen(curStr) + strlen("/") );
		}

		char* tmp = realloc(outputPath, pathSize * sizeof(char));
		if (tmp == NULL) {
			free(outputPath);
			cgp_throw(MEM_ERR, "");
		}
		outputPath = tmp;
		
		strcat(outputPath, paths[i]);
		// Do not assume that the last segment of the path is a directory
		if (!isSuffixed && i != (length - 1)) strcat(outputPath, "/");
	}

	outputPath[pathSize - 1] = '\0';
	return outputPath;
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

// Requires freeing
char* String_toUppercase(char* string) {
	char* newString = malloc((strlen(string) + 1) * sizeof(char));
	if (newString == NULL) {
		cgp_throw(MEM_ERR, "");
	}
	for (int i = 0; string[i] != '\0'; i++) {
		char curChar = string[i];
		if (islower(curChar)) {
			newString[i] = toupper(curChar);
		} else {
			newString[i] = curChar;
		}
	}
	newString[strlen(string)] = '\0';
	return newString;
}

// Requires freeing
char* String_toPascalcase(char* string) {
	char* newString = malloc((strlen(string) + 1) * sizeof(char));
	if (newString == NULL) {
		cgp_throw(MEM_ERR, "");
	}
	// Empty string
	newString[0] = '\0';

	for (int i = 0; string[i] != '\0'; i++) {
		if (islower(string[i])) {
			if (i == 0)
				charcat(newString, toupper(string[i]));
			else
				charcat(newString, string[i]);
		} else if (isblank(string[i])) {
			if(isalpha(string[i + 1])) {
				charcat(newString, toupper(string[i + 1]));
				i++;
			}
		} else {
			charcat(newString, string[i]);
		}
	}

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

char* String_replaceAll(char* string, char* pattern, char* replacement) {
	// Grab the lengths
	size_t patternLen = strlen(pattern);
	size_t stringLen = strlen(string);
	size_t replacementLen = strlen(replacement);

	size_t newStringSize = strlen(string) + 1;
	char* newString = malloc(newStringSize * sizeof(char));
	// Start with empty string
	newString[0] = '\0';

	for (unsigned int i = 0; i < stringLen; i++) {
		// Start a search buffer
		char* patternCandidate = malloc(( patternLen + 1 ) * sizeof(char));
		for (unsigned int j = i; j < (i + patternLen); j++) {
			// Out of bounds of original string, break.
			if (j > (stringLen - 1)) {
				patternCandidate[j - i] = '\0';
				break;
			}
			patternCandidate[j - i] = string[j];
		}
		patternCandidate[patternLen] = '\0';

		// If the pattern buffer matches the pattern
		if (!strcmp(patternCandidate, pattern)) {
			/*
				Calculate the difference in length between the replacement and the pattern, since the
				source string already contains the pattern at this point, we only need to allocate more
				memory if the replacement is larger.
			*/
			const size_t increaseAmount = replacementLen - patternLen;
			if (increaseAmount > 0) {
				newStringSize += increaseAmount;
				char* tmp = realloc(newString, newStringSize * sizeof(char));
				if (tmp == NULL) {
					free(newString);
					cgp_throw(MEM_ERR, "");
				}

				newString = tmp;
			}

			/*
				Copy the replacement into the new string and skip adding the characters from the source
				string.
			*/
			if (i == 0)
				strcpy(newString, replacement);
			else
				strcat(newString, replacement);
			// Skip by patternLen - 1 because the loop itself contains i++
			i += (patternLen - 1);
			// Free the buffer
			free(patternCandidate); patternCandidate = NULL;
			continue;
		}
		free(patternCandidate); patternCandidate = NULL;
		/*
			If the pattern buffer didn't match, then just copy the current character into the string and
			continue the loop character-by-character
		*/
		if (i == 0)
			charcpy(newString, string[i]);
		else
			charcat(newString, string[i]);
	}

	return newString;
}

char* String_replaceAllMulti(char* string, char* patterns[], char* replacements[], size_t filterCount) {
	char* result = NULL;

	for (size_t i = 0; i < filterCount; i++) {
		if (result == NULL) {
			result = String_replaceAll(string, patterns[i], replacements[i]);
		} else {
			char* tmp = result;
			result = String_replaceAll(tmp, patterns[i], replacements[i]);
			free(tmp); tmp = NULL;
		}
	}

	return result;
}

void charcpy(char* string, char character) {
	strcpy(string, (char[]){character, '\0'});
}

void charcat(char* string, char character) {
	strcat(string, (char[]){character, '\0'});
}

char* Int_toString(int32_t number) {
	// 10 for the digits in 2^32, 1 for the potential negative sign and 1 for NUL
	char* newString = malloc(12 * sizeof(char));
	snprintf(newString, 12, "%i", number);
	return newString;
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
	if (dynamicArray->array == NULL) {
		DynIntArray newArray;
		unsigned int newLength = 1;

		newArray.array = malloc(1 * sizeof(int));
		if (newArray.array == NULL) {
			cgp_throw(MEM_ERR, "");
		}
		newArray.length = newLength;
		newArray.array[0] = item;

		*dynamicArray = newArray;
		return newLength;
	}

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
	free(dynamicArray->array);
	*dynamicArray = newArray;

	return newLength;
}

//* Pop the last element of a dynamic integer array
unsigned int DynIntArray_pop(DynIntArray* dynamicArray) {
	if (dynamicArray->array == NULL) {
		if (dynamicArray->length != 0)
			dynamicArray->length = 0;
		return 0;
	}

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
	if (dynamicArray->array == NULL)
		return false;

	for (int i = 0; i < dynamicArray->length; i++) {
		if (dynamicArray->array[i] == item)
			return true;
	}
	return false;
}

int64_t DynIntArray_indexOf(DynIntArray* dynamicArray, int item) {
	if (dynamicArray->array == NULL)
		return -1;

	for (int i = 0; i < dynamicArray->length; i++) {
		if (dynamicArray->array[i] == item)
			return i;
	}
	return -1;
}

//* Dynamic string arrays
// TODO: String dynamic arrays