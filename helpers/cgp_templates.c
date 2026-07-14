#include "cgp_errors.h"
#include "cgp_templates.h"
#include "cgp_utils.h"

void downloadFileAndReplace(
	char* directory,
	char* filename,
	char* URL,
	char* PROJECT_NAME,
	char* PROJECT_ID,
	char* AUTHOR,
	bool appendProjectFilename,
	bool appendProjectId
) {
	char* project_name_lower_tmp = String_toLowercase(PROJECT_NAME);
	char* FILENAME = String_replaceAllMulti(project_name_lower_tmp, (char*[]){" ", "/"}, (char*[]){"",""}, 2);
	free(project_name_lower_tmp); project_name_lower_tmp = NULL;

	char* temp_cat_tmp = malloc(( strlen(filename) + strlen(".tmp") + 1) * sizeof(char));
	if (temp_cat_tmp == NULL)
		cgp_throw(MEM_ERR, "");
	strcpy(temp_cat_tmp, filename); strcat(temp_cat_tmp, ".tmp");

	char* tempFilepath = path_join((char*[]){directory, temp_cat_tmp}, 2);
	char* localFilename = appendProjectFilename ? projectFilenameToFile(filename, FILENAME)
												: (appendProjectId ? projectIdToFile(filename, PROJECT_ID)
																  : filename);
	char* filepath = path_join((char*[]){directory, localFilename}, 2);
	free(temp_cat_tmp); temp_cat_tmp = NULL;

	FILE* tempBinPtr = fopen(tempFilepath, "wb");
	if (tempBinPtr == NULL) cgp_throw(MEM_ERR, "");

	CURLcode downloadRes = downloadFile(
		tempBinPtr,
		URL);

	if (downloadRes != CURLE_OK) {
		cgp_throw(PROGRAM_ERR, "Couldn't download meson.build");
	}

	fclose(tempBinPtr);

	FILE* tempReadPtr = fopen(tempFilepath, "r");
	if (tempReadPtr == NULL) cgp_throw(MEM_ERR, "");

	FILE* writePtr = fopen(filepath, "w");
	if (writePtr == NULL) cgp_throw(MEM_ERR, "");
	free(filepath); filepath = NULL;

	char* name_caps_tmp = String_toUppercase(PROJECT_NAME);
	char* NAME_CAPS = String_replaceAll(name_caps_tmp, " ", "");
	free(name_caps_tmp); name_caps_tmp = NULL;
	
	char lineBuffer[256];

	time_t curTime = time(NULL);
	char* YEAR = Int_toString(localtime(&curTime)->tm_year + 1900);

	char* PascalName = String_toPascalcase(PROJECT_NAME);
	
	while (fgets(lineBuffer, sizeof(lineBuffer), tempReadPtr) != NULL) {
		char* filteredLine = String_replaceAllMulti(
			lineBuffer,
			(char*[]){	"{{FILENAME}}",	"{{PROJECT_NAME}}", "{{NAME_CAPS}}",	"{{PROJECT_ID}}", "{{PascalName}}", "{{AUTHOR}}", "{{YEAR}}" },
			(char*[]){	FILENAME,		PROJECT_NAME,		NAME_CAPS,			PROJECT_ID,		  PascalName,		AUTHOR,		  YEAR },
			7);
		fputs(filteredLine, writePtr);
		free(filteredLine);
	}

	fclose(tempReadPtr); tempReadPtr= NULL;
	remove(tempFilepath);
	fclose(writePtr); writePtr = NULL;

	if (appendProjectFilename || appendProjectId) { free(localFilename); localFilename == NULL; }
	free(tempFilepath); tempFilepath = NULL;
	free(NAME_CAPS); NAME_CAPS = NULL;
	free(YEAR); YEAR = NULL;
	free(PascalName); PascalName = NULL;
	free(FILENAME); FILENAME = NULL;
}

char* projectFilenameToFile(char* filename, char* PROJECT_FILENAME) {
	char* name_tmp = malloc((strlen(PROJECT_FILENAME) + strlen("-") + strlen(filename) + 1) * sizeof(char));
	if (name_tmp == NULL) cgp_throw(MEM_ERR, "");
	strcpy(name_tmp, PROJECT_FILENAME); strcat(name_tmp, "-"); strcat(name_tmp, filename);

	return name_tmp;
}

char* projectIdToFile(char* filename, char* PROJECT_ID) {
	char* name_tmp = malloc((strlen(PROJECT_ID) + strlen(".") + strlen(filename) + 1) * sizeof(char));
	if (name_tmp == NULL) cgp_throw(MEM_ERR, "");
	strcpy(name_tmp, PROJECT_ID); strcat(name_tmp, "."); strcat(name_tmp, filename);

	return name_tmp;
}