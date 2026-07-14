// TODO: Find an alternative
#include <cs50.h>
#include <sys/stat.h>
#include <unistd.h>

#include "helpers/cgp_errors.h"
#include "helpers/cgp_utils.h"
#include "helpers/cgp_args.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cgp_printHelp();
		exit(0);
	}

	if (cgp_searchArgs(argc, argv, "--help", "-h")) {
		cgp_printHelp();
		exit(0);
	}

	cgp_validateArgs(argc, argv);

	if (!cgp_searchArgs(argc, argv, "-p", "--project")) {
		cgp_printHelp();
		exit(1);
	}

	//^ Program
	char* projectType = NULL;
	char* projectName = NULL;
	char* projectId = NULL;
	char* outputDir = NULL;
	char* projectLang = NULL;
	char* editor = NULL;
	char* license = NULL;
	bool doGit = false;
	bool isExtension = false;
	bool isLibadwaita = false;

	//* Reusable variables
	char* tmp = NULL;
	unsigned int tmpLen = 0;
	char* tmpPtr = NULL;

	//? --project
		tmp = cgp_getArgValue(argc, argv, "-p", "--project");
		tmpLen = strlen(tmp);
		if (tmpLen < 1) cgp_throw(INVALID_ARG, "Project Type required.");
		// 								NUL
		tmpPtr = malloc((tmpLen + 1) * sizeof(char));
		if (tmpPtr == NULL)
			cgp_throw(MEM_ERR, "");
		strcpy(tmpPtr, tmp);
		projectType = String_toLowercase(tmpPtr);

		// If it's different from all possible values
		if (!StringArray_includes((char*[]){ "gtk", "libadwaita", "extension" }, 3, projectType))
			cgp_throw(INVALID_ARG, "Invalid Project Type.");

		isExtension = !strcmp(projectType, "extension");
		isLibadwaita = !strcmp(projectType, "libadwaita");

		free(tmpPtr); tmpPtr = NULL;

	//? --name
		tmp = cgp_getArgValue(argc, argv, "-n", "--name");
		tmpLen = strlen(tmp);
		if (tmpLen < 1) {
			tmp = get_string_withDefault(
				isExtension ? "Extension Name: " : "App Name: ",
				isExtension ? "My Extension" : "My Program"
			);
			tmpLen = strlen(tmp);
		}
		// 								NUL
		tmpPtr = malloc((tmpLen + 1) * sizeof(char));
		if (tmpPtr == NULL)
			cgp_throw(MEM_ERR, "");
		strcpy(tmpPtr, tmp);
		projectName = String_toLowercase(tmpPtr);
		
		free(tmpPtr); tmpPtr = NULL;

	//? --id
		tmp = cgp_getArgValue(argc, argv, "-i", "--id");
		tmpLen = strlen(tmp);
		if (tmpLen < 1) {
			tmp = get_string_withDefault(
				isExtension ? "Extension ID: " : "App ID: ",
				isExtension ? "com.example.my_extension" : "com.example.MyProgram"
			);
			tmpLen = strlen(tmp);
		}
		// 								NUL
		tmpPtr = malloc((tmpLen + 1) * sizeof(char));
		if (tmpPtr == NULL)
			cgp_throw(MEM_ERR, "");
		strcpy(tmpPtr, tmp);
		projectId = String_toLowercase(tmpPtr);
		
		free(tmpPtr); tmpPtr = NULL;

	//? --output-dir
		tmp = cgp_getArgValue(argc, argv, "-o", "--output-dir");
		tmpLen = strlen(tmp);
		if (tmpLen < 1) tmp = get_string_withDefault("Output Directory: ", "./"), tmpLen = strlen(tmp);
		// 								NUL
		tmpPtr = malloc((tmpLen + 1) * sizeof(char));
		if (tmpPtr == NULL)
			cgp_throw(MEM_ERR, "");
		strcpy(tmpPtr, tmp);
		outputDir = String_toLowercase(tmpPtr);

		struct stat statBuffer;
		// If it exists
		if (stat(outputDir, &statBuffer) == 0) {
			// If it's not a dir
			if (!S_ISDIR(statBuffer.st_mode)) {
				cgp_throw(INVALID_ARG, "--output-dir must be a directory.");
			}
		} else {
			//! Create it, this WON'T WORK ON WINDOWS
			if (mkdir(outputDir, 0777) != 0) {
				cgp_throw(IO, "Couldn't create output directory.");
			}
		}
		free(tmpPtr); tmpPtr = NULL;

	//? --lang
		tmp = cgp_getArgValue(argc, argv, "-l", "--lang");
		tmpLen = strlen(tmp);
		if (tmpLen < 1) {
			tmp = get_string_withDefault(
				"Language: ",
				"JavaScript"
			);
			tmpLen = strlen(tmp);
		}
		// 								NUL
		tmpPtr = malloc((tmpLen + 1) * sizeof(char));
		if (tmpPtr == NULL)
			cgp_throw(MEM_ERR, "");
		strcpy(tmpPtr, tmp);
		projectLang = String_toLowercase(tmpPtr);

		// If it's different from all possible values
		if (!StringArray_includes((char*[]){ "c", "javascript" }, 2, projectLang))
			cgp_throw(INVALID_ARG, "Unsupported language.");

		free(tmpPtr); tmpPtr = NULL;

	//? --editor
		tmp = cgp_getArgValue(argc, argv, "-e", "--editor");
		tmpLen = strlen(tmp);
		if (tmpLen < 1) {
			tmp = get_string_withDefault(
				"Editor/IDE: ",
				"None"
			);
			tmpLen = strlen(tmp);
		}
		// 								NUL
		tmpPtr = malloc((tmpLen + 1) * sizeof(char));
		if (tmpPtr == NULL)
			cgp_throw(MEM_ERR, "");
		strcpy(tmpPtr, tmp);
		editor = String_toLowercase(tmpPtr);

		// If it's different from all possible values
		if (!StringArray_includes((char*[]){ "vscode", "rider", "none" }, 3, editor))
			cgp_throw(INVALID_ARG, "Unsupported editor.");

		free(tmpPtr); tmpPtr = NULL;

	//? --license
		tmp = cgp_getArgValue(argc, argv, "-li", "--license");
		tmpLen = strlen(tmp);
		if (tmpLen < 1) {
			tmp = get_string_withDefault(
				"Code License: ",
				"GPLv3"
			);
			tmpLen = strlen(tmp);
		}
		// 								NUL
		tmpPtr = malloc((tmpLen + 1) * sizeof(char));
		if (tmpPtr == NULL)
			cgp_throw(MEM_ERR, "");
		strcpy(tmpPtr, tmp);
		license = String_toLowercase(tmpPtr);

		// If it's different from all possible values
		if (!StringArray_includes((char*[]){ "gplv3", "mit" }, 2, license))
			cgp_throw(INVALID_ARG, "Unsupported license.");

		free(tmpPtr); tmpPtr = NULL;
	
	//? --git
		doGit = cgp_searchArgs(argc, argv, "-g", "--git");

	printf("Working...\n");

	//! GTK Project structure learned from https://developer.gnome.org/documentation/tutorials/beginners/getting_started.html
	if (!isExtension) {
		printf("Creating GTK4 app...\n");

		if (projectLang == "c") {
			char* mesonBuildFilepath = malloc((strlen(outputDir) + strlen("meson.build") + strlen("/") + 1) * sizeof(char));
			strcpy(mesonBuildFilepath, outputDir);
			if (!String_endsWith(outputDir, "/")) strcat(mesonBuildFilepath, "/");
			strcat(mesonBuildFilepath, "meson.build");

			FILE* mesonBuildFilePtr = fopen(mesonBuildFilepath, "w");
			free(mesonBuildFilepath);
			mesonBuildFilepath = NULL;
			printf("Downloading meson.build...\n");

			fclose(mesonBuildFilePtr);
		}
	}

	// Free memory
	if (projectType != NULL) free(projectType);
	if (projectName != NULL) free(projectName);
	if (projectId != NULL) free(projectId);
	if (outputDir != NULL) free(outputDir);
	if (projectLang != NULL) free(projectLang);
	if (editor != NULL) free(editor);
	if (license != NULL) free(license);

	return 0;
}