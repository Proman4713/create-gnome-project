#include "cgp_args.h"
#include "cgp_errors.h"
#include "cgp_utils.h"

// Short option, long option and description

const char* CGP_OPTIONS[MAX_OPTIONS][3] = {
	{"-h", "--help", "Show this help message and exit."},
	{"-p", "--project", "\x1b[31m\x1b[1m[REQUIRED]\x1b[0m Type of project. Available options: GTK, Libadwaita, Extension"},
	{"-n", "--name", "Project Name. Default: my-program"},
	{"-a", "--author", "Project Author. Default: John Doe"},
	{"-i", "--id", "Application ID for GTK apps or extension ID for extensions. Default: com.example.MyProgram"},
	{"-o", "--output-dir", "Output directory. Default: ./"},
	{"-l", "--lang", "Programming Language. Available options: JavaScript, C. Default: JavaScript"},
	{"-e", "--editor", "Configure editor or IDE to work immediately. Available options: VSCode, Rider, None. Default: None"},
	{"-li", "--license", "Code License. Available options: GPLv3, MIT. Default: GPLv3"},
	// Terminal styling courtesy of my own javascript library, https://github.com/Proman4713/javascript-console-styling
	{"-g", "--git", "Initiate a git repository in \x1b[1m--output-dir\x1b[0m"},
};

void cgp_printHelp() {
	printf(
		"USAGE: create-gnome-project [-h]\n"
		"\n"
		"A simple C utility that creates a template GNOME shell extension or GTK4 application\n"
		"\n"
		"Options:\n");
	const int TOTAL_SPACES = 32;

	for (int i = 0; i < MAX_OPTIONS; i++) {
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
		if (shortOpt[0] != '\0') {
			entryLength += strlen(shortOpt);
			printf("%s", shortOpt);
		}

		// If there's a long option...
		if (longOpt[0] != '\0') {
			entryLength += strlen(longOpt);
			if (shortOpt[0] != '\0') {
				char* prefix = ", ";
				entryLength += strlen(prefix);
				printf("%s%s", prefix, longOpt); // ...add that as well
			} else
				printf("%s", longOpt); // ...or print it alone if there was no short one
		}

		// Description
		if (desc[0] != '\0') {
			printf(":");
			// account for the colon
			entryLength++;
			for (int i = 0; i < TOTAL_SPACES - entryLength; i++) {
				printf(" ");
			}
			printf("%s", desc);
		}

		// Final newline
		printf("\n");
	}
}

bool cgp_searchArgs(int argc, char* argv[], char* shortArg, char* longArg) {
	// If we have arguments, then search the arguments only without argv[0]
	if (StringArray_includes(argv + 1, argc - 1, shortArg) || StringArray_includes(argv + 1, argc - 1, longArg))
		return true;
	return false;
}

char* cgp_getArgValue(int argc, char* argv[], char* shortArg, char* longArg) {
	bool isShort = true;
	if (!StringArray_includes(argv, argc, shortArg)) {
		if (StringArray_includes(argv, argc, longArg))
			isShort = false;
		else
			return "\0";
	}

	int keyIdx = StringArray_indexOf(argv, argc, isShort ? shortArg : longArg);
	// Check if the key is the last option or if the next argument is itself an option.
	if (argv[keyIdx + 1] == NULL || argv[keyIdx + 1][0] == '-')
		return "\0";

	return argv[keyIdx + 1];
}

void cgp_validateArgs(int argc, char* argv[]) {
	// Validate all arguments after argv[0], of course
	for (int i = 1; i < argc; i++) {
		bool found = false;
		// It's a value, not an argument
		if (argv[i][0] != '-') continue;
		for (int j = 0; j < MAX_OPTIONS; j++) {
			const char** optionData = CGP_OPTIONS[j];
			if (optionData[0] == NULL)
				break;

			// If it's either a shortened or a long version of an option
			if (strcmp(optionData[0], argv[i]) == 0 || strcmp(optionData[1], argv[i]) == 0)
				found = true;
		}
		if (!found)
			cgp_throw(INVALID_ARG, argv[i]);
	}
}

// Requires freeing
char* cgp_getOrPromptArg(
	int argc,
	char* argv[],
	char* shortArg,
	char* longArg,
	char* argName,
	bool lower,
	bool required,
	char* prompt,
	char* defaultValue,
	char* allowedOptions[],
	unsigned int allowedOptionCount
) {
	char* value_tmp = NULL;
	unsigned int tmpLen = 0;

	value_tmp = cgp_getArgValue(argc, argv, shortArg, longArg);
	tmpLen = strlen(value_tmp);
	if (tmpLen < 1) {
		if (required) {
			char* err_tmp = malloc(( strlen(argName) + strlen(" required.") + 1 ) * sizeof(char));
			if (err_tmp == NULL) cgp_throw(MEM_ERR, "");
			strcpy(err_tmp, argName); strcat(err_tmp, " required.");
			cgp_throw(INVALID_ARG, err_tmp);
			// No need to free err_tmp, already thrown and exited
		} else {
			value_tmp = get_string_withDefault(
				prompt, defaultValue
			);
			tmpLen = strlen(value_tmp);
		}
	}

	char* lower_tmp = String_toLowercase(value_tmp);
	if (lower_tmp == NULL) cgp_throw(MEM_ERR, "");
	if (allowedOptions != NULL && allowedOptionCount > 0 && !StringArray_includes(allowedOptions, allowedOptionCount, lower_tmp)) {
		char* err_tmp = malloc(( strlen("Invalid ") + strlen(argName) + strlen(".") + 1 ) * sizeof(char));
		if (err_tmp == NULL) cgp_throw(MEM_ERR, "");
		strcpy(err_tmp, "Invalid "); strcat(err_tmp, argName); strcat(err_tmp, ".");
		cgp_throw(INVALID_ARG, err_tmp);
	}

	char* argValue = malloc((tmpLen + 1) * sizeof(char));
	if (argValue == NULL) cgp_throw(MEM_ERR, "");

	if (lower) {
		strcpy(argValue, lower_tmp);
	} else {
		strcpy(argValue, value_tmp);
	}
	free(lower_tmp); lower_tmp = NULL;

	return argValue;
}