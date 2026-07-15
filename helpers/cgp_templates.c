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
	char* PROJECT_LICENSE,
	bool appendProjectFilename,
	bool appendProjectId
) {
	char* project_name_lower_tmp = String_toLowercase(PROJECT_NAME);
	char* FILENAME = String_replaceAllMulti(project_name_lower_tmp, (char*[]){" ", "/"}, (char*[]){"",""}, 2);
	free(project_name_lower_tmp); project_name_lower_tmp = NULL;

	size_t name_cat_tmp_len = strlen(filename) + strlen(".tmp") + 1;
	char* name_cat_tmp = malloc(name_cat_tmp_len * sizeof(char));
	if (name_cat_tmp == NULL)
		cgp_throw(MEM_ERR, "");
	snprintf(name_cat_tmp, name_cat_tmp_len, "%s.tmp", filename);

	char* tempFilepath = path_join((char*[]){directory, name_cat_tmp}, 2);
	char* localFilename = appendProjectFilename ? projectFilenameToFile(filename, FILENAME)
												: (appendProjectId ? projectIdToFile(filename, PROJECT_ID)
																  : filename);
	char* filepath = path_join((char*[]){directory, localFilename}, 2);
	free(name_cat_tmp); name_cat_tmp = NULL;

	FILE* tempBinPtr = fopen(tempFilepath, "wb");
	if (tempBinPtr == NULL) cgp_throw(MEM_ERR, "");

	CURLcode downloadRes = downloadFile(
		tempBinPtr,
		URL);

	if (downloadRes != CURLE_OK) {
		size_t err_tmp_len = strlen("Couldn't download ") + strlen(filename) + strlen(".") + 1;
		char* err_tmp = malloc(err_tmp_len * sizeof(char));
		snprintf(err_tmp, err_tmp_len, "Couldn't download %s.", filename);
		cgp_throw(PROGRAM_ERR, err_tmp);
		free(err_tmp); err_tmp = NULL;
	}

	fclose(tempBinPtr); tempBinPtr = NULL;

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

	char* proj_id_path_tmp = String_replaceAll(PROJECT_ID, ".", "/");
	size_t proj_id_path_len = strlen("/") + strlen(proj_id_path_tmp) + 1;
	char* PROJECT_ID_PATH = malloc(proj_id_path_len * sizeof(char));
	snprintf(PROJECT_ID_PATH, proj_id_path_len, "/%s", proj_id_path_tmp);
	free(proj_id_path_tmp); proj_id_path_tmp = NULL;

	/*
		For flatpak manifest, this gets the path of whatever directory is currently being downloaded to,
		! luckily the flatpak manifest is in outputDir, but I should keep that in mind
	*/
	char* curDir = getcwd(NULL, 0);
	chdir(directory);
	char* PROJECT_PATH = getcwd(NULL, 0);
	chdir(curDir);
	free(curDir); curDir = NULL;
	
	while (fgets(lineBuffer, sizeof(lineBuffer), tempReadPtr) != NULL) {
		char* filteredLine = String_replaceAllMulti(
			lineBuffer,
			(char*[]){	"{{FILENAME}}",	"{{PROJECT_NAME}}", "{{NAME_CAPS}}",	"{{PROJECT_ID}}", "{{PascalName}}", "{{AUTHOR}}", "{{YEAR}}",	"{{PROJECT_ID_PATH}}",	"{{PROJECT_LICENSE}}", "{{PROJECT_PATH}}" },
			(char*[]){	FILENAME,		PROJECT_NAME,		NAME_CAPS,			PROJECT_ID,		  PascalName,		AUTHOR,		  YEAR,			PROJECT_ID_PATH,		PROJECT_LICENSE,		PROJECT_PATH },
			10);
		fputs(filteredLine, writePtr);
		free(filteredLine);
	}

	fclose(tempReadPtr); tempReadPtr= NULL;
	remove(tempFilepath);
	fclose(writePtr); writePtr = NULL;

	if (appendProjectFilename || appendProjectId) { free(localFilename); localFilename == NULL; }
	free(PROJECT_PATH); PROJECT_PATH = NULL;
	free(PROJECT_ID_PATH); PROJECT_ID_PATH = NULL;
	free(tempFilepath); tempFilepath = NULL;
	free(NAME_CAPS); NAME_CAPS = NULL;
	free(YEAR); YEAR = NULL;
	free(PascalName); PascalName = NULL;
	free(FILENAME); FILENAME = NULL;
}

char* projectFilenameToFile(char* filename, char* PROJECT_FILENAME) {
	size_t name_tmp_len = strlen(PROJECT_FILENAME) + strlen("-") + strlen(filename) + 1;
	char* name_tmp = malloc(name_tmp_len * sizeof(char));
	if (name_tmp == NULL) cgp_throw(MEM_ERR, "");
	snprintf(name_tmp, name_tmp_len, "%s-%s", PROJECT_FILENAME, filename);

	return name_tmp;
}

char* projectIdToFile(char* filename, char* PROJECT_ID) {
	size_t name_tmp_len = strlen(PROJECT_ID) + strlen(".") + strlen(filename) + 1;
	char* name_tmp = malloc(name_tmp_len * sizeof(char));
	if (name_tmp == NULL) cgp_throw(MEM_ERR, "");
	snprintf(name_tmp, name_tmp_len, "%s.%s", PROJECT_ID, filename);

	return name_tmp;
}

void finishAndGreet(bool isExtension, bool isLibadwaita, bool doGit, char* outputDir) {
	chdir(outputDir);
	if (doGit) {
		system("git init");
		FILE* gitignorePtr = fopen(".gitignore", "w");
		if (gitignorePtr == NULL) cgp_throw(IO, "Couldn't open .gitignore.");
		fputs("builddir/", gitignorePtr);
		fclose(gitignorePtr);
	} else
		printf("Skipping git, -g/--git flags not provided.\n\n");

	if (!isExtension) {
		printf("Running meson...\n\n");
		system("meson setup builddir");
		if (isLibadwaita) {
			printf("\x1b[32mGood luck on your GTK4/Libadwaita app :D\x1b[0m\n");
			return;
		}

		printf("\x1b[32mGood luck on your GTK4 app :D\x1b[0m\n");
		return;
	}

	printf("\x1b[32mGood luck on your GNOME extension :D\x1b[0m\n");
}