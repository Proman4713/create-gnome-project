#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define LINE_BUFFER_LENGTH 256

void downloadFileAndIterate(
	char* directory,
	char* filename,
	char* URL,
	char* patterns[],
	char* replacements[],
	size_t filterCount
);

void downloadFileAndReplace(
	char* directory,
	char* filename,
	char* URL,
	char* PROJECT_NAME,
	char* PROJECT_ID,
	char* AUTHOR,
	char* PROJECT_LICENSE,
	bool appendProjectFilename,
	bool appendProjectId
);

char* projectDataToFile(char* filename, char* PROJECT_DATA, char* SEP);

void implantLicenseFile(char* PROJECT_LICENSE, char* AUTHOR, char* outputDir);

void finishAndGreet(bool isExtension, bool isLibadwaita, bool doGit, char* outputDir);