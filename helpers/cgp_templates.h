#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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

char* projectFilenameToFile(char* filename, char* PROJECT_FILENAME);

char* projectIdToFile(char* filename, char* PROJECT_ID);

void finishAndGreet(bool isExtension, bool isLibadwaita, bool doGit, char* outputDir);