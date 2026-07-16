#include "cgp_errors.h"
#include "cgp_templates.h"
#include "cgp_utils.h"

void downloadFileAndIterate(
	char* directory,
	char* filename,
	char* URL,
	char* patterns[],
	char* replacements[],
	size_t filterCount
) {
	size_t name_cat_tmp_len = strlen(filename) + strlen(".tmp") + 1;
	char* name_cat_tmp = malloc(name_cat_tmp_len * sizeof(char));
	if (name_cat_tmp == NULL) cgp_throw(MEM_ERR, "");
	snprintf(name_cat_tmp, name_cat_tmp_len, "%s.tmp", filename);

	char* tempFilepath = path_join((char*[]){directory, name_cat_tmp}, 2);
	char* filepath = path_join((char*[]){directory, filename}, 2);
	free(name_cat_tmp); name_cat_tmp = NULL;

	FILE* tempBinPtr = fopen(tempFilepath, "wb");
	if (tempBinPtr == NULL) cgp_throw(MEM_ERR, "");

	CURLcode downloadRes = downloadFile(
		tempBinPtr,
		URL);

	fclose(tempBinPtr); tempBinPtr = NULL;

	FILE* tempReadPtr = fopen(tempFilepath, "r");
	if (tempReadPtr == NULL) cgp_throw(MEM_ERR, "");

	FILE* writePtr = fopen(filepath, "w");
	if (writePtr == NULL) cgp_throw(MEM_ERR, "");
	free(filepath); filepath = NULL;

	if (downloadRes != CURLE_OK) {
		size_t err_tmp_len = strlen("Couldn't download ") + strlen(filename) + strlen(".") + 1;
		char* err_tmp = malloc(err_tmp_len * sizeof(char));
		snprintf(err_tmp, err_tmp_len, "Couldn't download %s.", filename);
		cgp_throw(PROGRAM_ERR, err_tmp);
		free(err_tmp); err_tmp = NULL;
	}

	char lineBuffer[LINE_BUFFER_LENGTH];
	while (fgets(lineBuffer, sizeof(lineBuffer), tempReadPtr) != NULL) {
		char* filteredLine = String_replaceAllMulti(
			lineBuffer,
			patterns,
			replacements,
			filterCount);
		fputs(filteredLine, writePtr);
		free(filteredLine);
	}

	fclose(tempReadPtr); tempReadPtr= NULL;
	remove(tempFilepath);
	fclose(writePtr); writePtr = NULL;
	free(tempFilepath); tempFilepath = NULL;
}

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

	char* localFilename = appendProjectFilename ? projectDataToFile(filename, FILENAME, "-")
												: (appendProjectId ? projectDataToFile(filename, PROJECT_ID, ".")
																  : filename);

	char* name_caps_tmp = String_toUppercase(PROJECT_NAME);
	char* NAME_CAPS = String_replaceAll(name_caps_tmp, " ", "");
	free(name_caps_tmp); name_caps_tmp = NULL;

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
	
	//* License headers
	char* fallbackLicenseHeader = " * [create-gnome-project]: Unsupported license";
	size_t license_header_len_tmp = strlen(fallbackLicenseHeader) + 1;
	char* LICENSE_HEADER = malloc(license_header_len_tmp * sizeof(char));
	if (LICENSE_HEADER == NULL) cgp_throw(MEM_ERR, "");
	snprintf(LICENSE_HEADER, license_header_len_tmp, "%s", fallbackLicenseHeader);

	if (!strcmp(PROJECT_LICENSE, "gplv3")) {
		char* licenseHeaderTemp = " * Copyright (C) <year>  <name of author>\n"
								  " *\n"
								  " * This program is free software: you can redistribute it and/or modify\n"
								  " * it under the terms of the GNU General Public License as published by\n"
								  " * the Free Software Foundation, version 3.\n"
								  " *\n"
								  " * This program is distributed in the hope that it will be useful,\n"
								  " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
								  " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
								  " * GNU General Public License for more details.\n"
								  " *\n"
								  " * You should have received a copy of the GNU General Public License\n"
								  " * along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
								  " *\n"
								  " * SPDX-License-Identifier: GPL-3.0-only";

		free(LICENSE_HEADER); LICENSE_HEADER = NULL;
		LICENSE_HEADER = String_replaceAllMulti(licenseHeaderTemp, (char*[]){ "<year>", "<name of author>" }, (char*[]){ YEAR, AUTHOR }, 2);
	} else if (!strcmp(PROJECT_LICENSE, "mit")) {
		char* licenseHeaderTemp = " * Licensed under the MIT license. See the LICENSE file in the project root for details.\n"
								  " *\n"
								  " * SPDX-License-Identifier: MIT";

		free(LICENSE_HEADER); LICENSE_HEADER = NULL;
		// Must be malloc'd because it's freed later
		LICENSE_HEADER = malloc((strlen(licenseHeaderTemp) + 1) * sizeof(char));
		if (LICENSE_HEADER == NULL) cgp_throw(MEM_ERR, "");
		strcpy(LICENSE_HEADER, licenseHeaderTemp);
	}

	downloadFileAndIterate(
		directory, localFilename, URL,
		(char*[]){	"{{FILENAME}}",	"{{PROJECT_NAME}}", "{{NAME_CAPS}}",	"{{PROJECT_ID}}", "{{PascalName}}", "{{AUTHOR}}", "{{YEAR}}",	"{{PROJECT_ID_PATH}}",	"{{PROJECT_LICENSE}}", "{{PROJECT_PATH}}",	" * Place your license here" },
		(char*[]){	FILENAME,		PROJECT_NAME,		NAME_CAPS,			PROJECT_ID,		  PascalName,		AUTHOR,		  YEAR,			PROJECT_ID_PATH,		PROJECT_LICENSE,		PROJECT_PATH,		LICENSE_HEADER },
		11
	);

	if (appendProjectFilename || appendProjectId) { free(localFilename); localFilename == NULL; }
	free(LICENSE_HEADER); LICENSE_HEADER = NULL;
	free(PROJECT_PATH); PROJECT_PATH = NULL;
	free(PROJECT_ID_PATH); PROJECT_ID_PATH = NULL;
	free(NAME_CAPS); NAME_CAPS = NULL;
	free(YEAR); YEAR = NULL;
	free(PascalName); PascalName = NULL;
	free(FILENAME); FILENAME = NULL;
}

char* projectDataToFile(char* filename, char* PROJECT_DATA, char* SEP) {
	size_t name_tmp_len = strlen(PROJECT_DATA) + strlen(SEP) + strlen(filename) + 1;
	char* name_tmp = malloc(name_tmp_len * sizeof(char));
	if (name_tmp == NULL) cgp_throw(MEM_ERR, "");
	snprintf(name_tmp, name_tmp_len, "%s%s%s", PROJECT_DATA, SEP, filename);

	return name_tmp;
}

void implantLicenseFile(char* PROJECT_LICENSE, char* AUTHOR, char* outputDir) {
	char* licenseFilename = path_join((char*[]){ outputDir, "LICENSE" }, 2);
	FILE* licenseFilePtr = fopen(licenseFilename, "wb");
	if (licenseFilePtr == NULL) cgp_throw(MEM_ERR, "");

	printf("Downloading LICENSE...\n\n");
	if (!strcmp(PROJECT_LICENSE, "gplv3")) {
		CURLcode res = downloadFile(
			licenseFilePtr,
			"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/LICENSE-GPL"
		);
		if (res != CURLE_OK) {
			cgp_throw(PROGRAM_ERR, "Couldn't download GPL3 License file.");
		}
	} else if (!strcmp(PROJECT_LICENSE, "mit")) {
		//! This could produce an inconsistency of December 31st :)
		time_t curTime = time(NULL);
		char* YEAR = Int_toString(localtime(&curTime)->tm_year + 1900);

		// MIT contains the author and year, so we need so substitute them
		downloadFileAndIterate(
			outputDir, "LICENSE",
			"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/LICENSE-MIT",
			(char*[]){	"{{AUTHOR}}", "{{YEAR}}" },
			(char*[]){	AUTHOR,		  YEAR },
			2
		);

		free(YEAR); YEAR = NULL;
	} else {
		fputs("[create-gnome-project]: Unsupported license", licenseFilePtr);
	}

	free(licenseFilename); licenseFilename = NULL;
	fclose(licenseFilePtr); licenseFilePtr = NULL;
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