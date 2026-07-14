// TODO: Find an alternative
#include <cs50.h>
#include <sys/stat.h>
#include <unistd.h>

#include "helpers/cgp_args.h"
#include "helpers/cgp_errors.h"
#include "helpers/cgp_templates.h"
#include "helpers/cgp_utils.h"

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
	char* authorName = NULL;
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
		projectName = malloc((tmpLen + 1) * sizeof(char));
		strcpy(projectName, tmp);

	//? --author
		tmp = cgp_getArgValue(argc, argv, "-a", "--author");
		tmpLen = strlen(tmp);
		if (tmpLen < 1) {
			tmp = get_string_withDefault("Author Name: ", "John Doe");
			tmpLen = strlen(tmp);
		}
		authorName = malloc((tmpLen + 1) * sizeof(char));
		strcpy(authorName, tmp);

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
		projectId = malloc((tmpLen + 1) * sizeof(char));
		strcpy(projectId, tmp);

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
	char* project_name_lower_tmp = String_toLowercase(projectName);
	char* PROJECT_FILENAME = String_replaceAllMulti(project_name_lower_tmp, (char*[]){" ", "/"}, (char*[]){"",""}, 2);
	free(project_name_lower_tmp); project_name_lower_tmp = NULL;

	//! GTK Project structure learned from https://gitlab.gnome.org/GNOME/gnome-builder/-/blob/main/src/plugins/meson-templates/resources
	//!	As well as Ptyxis' project structure (I chose Ptyxis for no specific reason)
	if (!isExtension) {
		if (!strcmp(projectLang, "c")) {
			printf("\nCreating GTK4 app...\n\n");

			//? meson.build
			printf("Downloading meson.build...\n\n");
			downloadFileAndReplace(
				outputDir,
				"meson.build",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/meson.build",
				projectName,
				projectId,
				authorName,
				false, false);

			//? README.md
			printf("Downloading README.md...\n\n");
			downloadFileAndReplace(
				outputDir,
				"README.md",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/README.md",
				projectName,
				projectId,
				authorName,
				false, false);

			//? Flatpak manifest
			printf("Downloading %s.json...\n\n", projectId);
			downloadFileAndReplace(
				outputDir,
				"json",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/.json",
				projectName,
				projectId,
				authorName,
				false, true);

			//? src/
			printf("Downloading %s-application.c...\n\n", PROJECT_FILENAME);
			char* src_dir_tmp = path_join((char*[]){ outputDir, "src"}, 2);
			struct stat srcStatBuffer;

			// If it exists
			if (stat(src_dir_tmp, &srcStatBuffer) == 0) {
				// If it's not a dir
				if (!S_ISDIR(srcStatBuffer.st_mode)) {
					cgp_throw(INVALID_ARG, "--output-dir/src exists, it must be a directory.");
				}
			} else {
				//! Create it, this WON'T WORK ON WINDOWS
				if (mkdir(src_dir_tmp, 0777) != 0) {
					cgp_throw(IO, "Couldn't create source directory.");
				}
			}

			downloadFileAndReplace(
				src_dir_tmp,
				"application.c",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/application.c",
				projectName,
				projectId,
				authorName,
				true, false);

			printf("Downloading %s-application.h...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"application.h",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/application.h",
				projectName,
				projectId,
				authorName,
				true, false);

			printf("Downloading %s-window.c...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"window.c",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/window.c",
				projectName,
				projectId,
				authorName,
				true, false);

			printf("Downloading %s-window.h...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"window.h",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/window.h",
				projectName,
				projectId,
				authorName,
				true, false);

			printf("Downloading %s-window.ui...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"window.ui",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/window.ui",
				projectName,
				projectId,
				authorName,
				true, false);

			printf("Downloading %s-shortcuts.ui...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"shortcuts.ui",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/shortcuts.ui",
				projectName,
				projectId,
				authorName,
				true, false);

			printf("Downloading %s.gresource.xml...\n\n", PROJECT_FILENAME);
			char* gresource_name_temp = malloc(( strlen(PROJECT_FILENAME) + strlen(".gresource.xml") + 1) * sizeof(char));
			if (gresource_name_temp == NULL) cgp_throw(MEM_ERR, "");
			sprintf(gresource_name_temp, "%s.gresource.xml", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				gresource_name_temp,
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/gresource.xml",
				projectName,
				projectId,
				authorName,
				false, false);
			free(gresource_name_temp); gresource_name_temp = NULL;

			printf("Downloading main.c...\n\n");
			downloadFileAndReplace(
				src_dir_tmp,
				"main.c",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/main.c",
				projectName,
				projectId,
				authorName,
				false, false);
			free(src_dir_tmp); src_dir_tmp = NULL;

			//? src/gtk
			char* gtk_dir_tmp = path_join((char*[]){ outputDir, "src", "gtk"}, 3);
			struct stat gtkStatBuffer;

			// If it exists
			if (stat(gtk_dir_tmp, &gtkStatBuffer) == 0) {
				// If it's not a dir
				if (!S_ISDIR(gtkStatBuffer.st_mode)) {
					cgp_throw(INVALID_ARG, "--output-dir/src/gtk exists, it must be a directory.");
				}
			} else {
				//! Create it, this WON'T WORK ON WINDOWS
				if (mkdir(gtk_dir_tmp, 0777) != 0) {
					cgp_throw(IO, "Couldn't create src/gtk directory.");
				}
			}
			free(gtk_dir_tmp); gtk_dir_tmp = NULL;

			//? data/
			char* data_dir_tmp = path_join((char*[]){ outputDir, "data"}, 2);
			struct stat dataStatBuffer;

			// If it exists
			if (stat(data_dir_tmp, &dataStatBuffer) == 0) {
				// If it's not a dir
				if (!S_ISDIR(dataStatBuffer.st_mode)) {
					cgp_throw(INVALID_ARG, "--output-dir/data exists, it must be a directory.");
				}
			} else {
				//! Create it, this WON'T WORK ON WINDOWS
				if (mkdir(data_dir_tmp, 0777) != 0) {
					cgp_throw(IO, "Couldn't create data directory.");
				}
			}

			char* icon_dir_tmp = path_join((char*[]){ outputDir, "data", "icons"}, 3);
			struct stat iconStatBuffer;

			// If it exists
			if (stat(icon_dir_tmp, &iconStatBuffer) == 0) {
				// If it's not a dir
				if (!S_ISDIR(iconStatBuffer.st_mode)) {
					cgp_throw(INVALID_ARG, "--output-dir/data/icons exists, it must be a directory.");
				}
			} else {
				//! Create it, this WON'T WORK ON WINDOWS
				if (mkdir(icon_dir_tmp, 0777) != 0) {
					cgp_throw(IO, "Couldn't create icons directory.");
				}
			}
			free(icon_dir_tmp); icon_dir_tmp = NULL;

			
			
			free(data_dir_tmp); data_dir_tmp = NULL;
		} else {
			cgp_throw(PROGRAM_ERR, "The programming language you chose is not yet implemented.");
		}
	}

	// Free memory
	if (PROJECT_FILENAME != NULL) free(PROJECT_FILENAME);
	if (projectType != NULL) free(projectType);
	if (projectName != NULL) free(projectName);
	if (authorName != NULL) free(authorName);
	if (projectId != NULL) free(projectId);
	if (outputDir != NULL) free(outputDir);
	if (projectLang != NULL) free(projectLang);
	if (editor != NULL) free(editor);
	if (license != NULL) free(license);

	return 0;
}