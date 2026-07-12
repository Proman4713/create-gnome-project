#include <unistd.h>

#include "helpers/cgp_errors.h"
#include "helpers/cgp_utils.h"

bool cgp_searchArgs(int argc, char* argv[], char* item) {
	// If we have arguments, then search the arguments only without argv[0]
	if (StringArray_includes(argv + 1, argc - 1, item))
		return true;
	return false;
}

char* cgp_getArgValue(int argc, char* argv[], char* key) {
	if (!StringArray_includes(argv, argc, key))
		return "\0";

	int keyIdx = StringArray_indexOf(argv, argc, key);
	// Check if the key is the last option or if the next argument is itself an option.
	if (argv[keyIdx + 1] == NULL || argv[keyIdx + 1][0] == '-')
		return "\0";

	return argv[keyIdx + 1];
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cgp_printHelp();
		exit(0);
	}

	if (cgp_searchArgs(argc, argv, "--help") || cgp_searchArgs(argc, argv, "-h")) {
		cgp_printHelp();
		exit(0);
	}

	// Validate all arguments after that, and after argv[0], of course
	for (int i = 1; i < argc; i++) {
		bool found = false;
		for (int j = 0; j < MAX_OPTIONS; j++) {
			const char** optionData = CGP_OPTIONS[j];
			if (optionData[0] == NULL)
				break;

			// If it's either a shortened or a long version of an option
			if (strcmp(optionData[0], argv[i]) == 0 || strcmp(optionData[1], argv[i]) == 0)
				found = true;
		}
		if (!found) {
			printf("Error: Invalid argument %s\n", argv[i]);
			exit(1);
		}
	}

	if (!cgp_searchArgs(argc, argv, "-p") && !cgp_searchArgs(argc, argv, "--project"))
	{
		cgp_printHelp();
		exit(1);
	}

	//^ Program

	return 0;
}