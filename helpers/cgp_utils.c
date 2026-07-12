#include "cgp_errors.h"
#include "cgp_utils.h"

// Short option, long option and description

const char* CGP_OPTIONS[MAX_OPTIONS][3] = {
	{"-h", "--help", "Show this help message and exit."},
	{"-n", "--name", "Project Name. Default: my-program"},
	{"-i", "--id", "Application ID for GTK apps or extension ID for extensions. Default: com.example.MyProgram"},
	{"-o", "--output-dir", "Output directory. Default: ./"},
	{"-l", "--lang", "Programming Language. Available options: JavaScript, C. Default: JavaScript"},
	{"-li", "--license", "Code License. Available options: GPLv3, MIT. Default: GPLv3"},
	// Terminal styling courtesy of my own javascript library, https://github.com/Proman4713/javascript-console-styling
	{"-g", "--git", "Initiate a git repository in \x1b[1m--output-dir\x1b[0m"},
	{"-p", "--project", "\x1b[31m\x1b[1m[REQUIRED]\x1b[0m Type of project. Available options: GTK4, Libadwaita, Extension"},
};

void cgp_printHelp()
{
	printf(
		"USAGE: create-gnome-project [-h]\n"\
		"\n"\
		"A simple C utility that creates a template GNOME shell extension or GTK4 application\n"\
		"\n"\
		"Options:\n"
	);
	const int TOTAL_SPACES = 32;

	for (int i = 0; i < MAX_OPTIONS; i++)
	{
		const char** optionData = CGP_OPTIONS[i];
		if (optionData[0] == NULL)
			break; // We've reached the end, otherwise we would have data at that index.
		
		const char* shortOpt = optionData[0];
		const char* longOpt = optionData[1];
		const char* desc = optionData[2];

		// Initial indentation
		printf("	");

		int entryLength = 0;

		// If there is a shortened option, print it
		if (shortOpt[0] != '\0')
		{
			entryLength += strlen(shortOpt);
			printf("%s", shortOpt);
		}

		// If there's a long option...
		if (longOpt[0] != '\0')
		{
			entryLength += strlen(longOpt);
			if (shortOpt[0] != '\0')
			{
				char* prefix = ", ";
				entryLength += strlen(prefix);
				printf("%s%s", prefix, longOpt); // ...add that as well
			}
			else
				printf("%s", longOpt); // ...or print it alone if there was no short one
		}
		
		// Description
		if (desc[0] != '\0')
		{
			printf(":");
			// account for the colon
			entryLength++;
			for (int i = 0; i < TOTAL_SPACES - entryLength; i++)
			{
				printf(" ");
			}
			printf("%s", desc);
		}
		
		// Final newline
		printf("\n");
	}
}

// JavaScript `Array.prototype.includes()` equivalent
bool IntArray_includes(int array[], unsigned int length, int item)
{
	for (int i = 0; i < length; i++)
	{
		if (array[i] == item)
			return true;
	}
	return false;
}

// JavaScript `Array.prototype.includes()` equivalent
bool StringArray_includes(char* array[], unsigned int length, char* item)
{
	for (int i = 0; i < length; i++)
	{
		if (strcmp(array[i], item) == 0)
			return true;
	}
	return false;
}

//* 64-bit integer to fit all `unsigned int` values plus negative values for the -1 and avoid overflows
// JavaScript `Array.prototype.indexOf()` equivalent
int64_t IntArray_indexOf(int array[], unsigned int length, int item)
{
	for (int i = 0; i < length; i++)
	{
		if (array[i] == item)
			return i;
	}
	return -1;
}

// JavaScript `Array.prototype.indexOf()` equivalent
int64_t StringArray_indexOf(char* array[], unsigned int length, char* item)
{
	for (int i = 0; i < length; i++)
	{
		if (strcmp(array[i], item) == 0)
			return i;
	}
	return -1;
}

//^ My own OOP coding style as a JS and C# dev sneaking in

//* Dynamic integer arrays
DynIntArray new_DynIntArray(int* array, unsigned int initialLength)
{
	if (array == NULL || initialLength == 0)
	{
		DynIntArray newArray;
		newArray.array = NULL;
		newArray.length = 0;
		return newArray;
	}

	DynIntArray newArray;
	newArray.array = malloc(initialLength * sizeof(int));
	if (newArray.array == NULL)
	{
		cgp_throw(MEM_ERR);
		exit(1);
	}
	newArray.length = initialLength;

	for (int i = 0; i < initialLength; i++)
	{
		newArray.array[i] = array[i];
	}

	return newArray;
}

//* Push an element to a dynamic integer array
unsigned int DynIntArray_push(DynIntArray* dynamicArray, int item)
{
	if (dynamicArray->length + 1 > UINT_MAX)
	{
		cgp_throw(TOO_LARGE);
		exit(1);
	}

	DynIntArray newArray;
	unsigned int newLength = dynamicArray->length + 1;

	int *newPtr = realloc(dynamicArray->array, newLength * sizeof(int));
	if (newPtr == NULL)
	{
		cgp_throw(MEM_ERR);
		exit(1);
	}

	// Set last element to new item
	newPtr[dynamicArray->length] = item;

	newArray.array = newPtr;
	newArray.length = newLength;
	*dynamicArray = newArray;

	return newLength;
}

//* Pop the last element of a dynamic integer array
unsigned int DynIntArray_pop(DynIntArray* dynamicArray)
{
	DynIntArray newArray;
	unsigned int newLength = dynamicArray->length - 1;

	// This frees the last element, basically deleting it
	int *newPtr = realloc(dynamicArray->array, newLength * sizeof(int));
	if (newPtr == NULL)
	{
		cgp_throw(MEM_ERR);
		exit(1);
	}

	newArray.array = newPtr;
	newArray.length = newLength;
	*dynamicArray = newArray;

	return newLength;
}

bool DynIntArray_includes(DynIntArray* dynamicArray, int item)
{
	for (int i = 0; i < dynamicArray->length; i++)
	{
		if (dynamicArray->array[i] == item)
			return true;
	}
	return false;
}

int64_t DynIntArray_indexOf(DynIntArray* dynamicArray, int item)
{
	for (int i = 0; i < dynamicArray->length; i++)
	{
		if (dynamicArray->array[i] == item)
			return i;
	}
	return -1;
}

//* Dynamic string arrays
//TODO: String dynamic arrays