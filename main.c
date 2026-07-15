// TODO: Find an alternative
#include <cs50.h>
#include <dirent.h>
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

	projectType = cgp_getOrPromptArg(
		argc, argv,
		"-p", "--project", "Project Type",
		true, true,
		NULL, NULL,
		(char*[]){ "gtk", "libadwaita", "extension" }, 3
	);

	isExtension = !strcmp(projectType, "extension");
	isLibadwaita = !strcmp(projectType, "libadwaita");


	projectName = cgp_getOrPromptArg(
		argc, argv,
		"-n", "--name", "Project Name",
		false, false,
		isExtension ? "Extension Name: " : "App Name: ", isExtension ? "My Extension" : "My Program",
		NULL, 0
	);

	authorName = cgp_getOrPromptArg(
		argc, argv,
		"-a", "--author", "Author Name",
		false, false,
		"Author Name: ", "John Doe",
		NULL, 0
	);

	projectId = cgp_getOrPromptArg(
		argc, argv,
		"-i", "--id", "Project ID",
		false, false,
		isExtension ? "Extension ID: " : "App ID: ", isExtension ? "com.example.my_extension" : "com.example.MyProgram",
		NULL, 0
	);

	outputDir = cgp_getOrPromptArg(
		argc, argv,
		"-o", "--output-dir", "Output Directory",
		false, false,
		"Output Directory: ", "./",
		NULL, 0
	);

	projectLang = cgp_getOrPromptArg(
		argc, argv,
		"-l", "--lang", "Project Language",
		true, false,
		"Language: ", "C",
		(char*[]){ "c", "javascript" }, 2
	);

	editor = cgp_getOrPromptArg(
		argc, argv,
		"-e", "--editor", "Editor",
		true, false,
		"Editor/IDE: ", "None",
		(char*[]){ "vscode", "rider", "none" }, 3
	);

	license = cgp_getOrPromptArg(
		argc, argv,
		"-li", "--license", "Project License",
		false, false,
		"Code License: ", "GPLv3",
		(char*[]){ "gplv3", "mit" }, 2
	);
	
	doGit = cgp_searchArgs(argc, argv, "-g", "--git");

	printf("Working...\n");

	mkdir_p(outputDir);
	DIR* output_dir_tmp = opendir(outputDir);
	if (output_dir_tmp == NULL) cgp_throw(IO, "Couldn't check output directory.");
	unsigned int dir_content_count = 0;
	while (readdir(output_dir_tmp) != NULL) {
		dir_content_count++;
	}
	// if it has more than . and ..
	if (dir_content_count > 2) cgp_throw(IO, "Output directory not empty.");
	closedir(output_dir_tmp); output_dir_tmp = NULL;

	char* project_name_lower_tmp = String_toLowercase(projectName);
	char* PROJECT_FILENAME = String_replaceAllMulti(project_name_lower_tmp, (char*[]){" ", "/"}, (char*[]){"",""}, 2);
	free(project_name_lower_tmp); project_name_lower_tmp = NULL;

	const char* NOT_IMPLEMENTED = "The template you chose is not yet implemented :(";

	//* GTK Project structure learned from https://gitlab.gnome.org/GNOME/gnome-builder/-/blob/main/src/plugins/meson-templates/resources
	if (!isExtension) {
		if (!strcmp(projectLang, "c")) {
			printf("\nCreating GTK4 app...\n\n");

			//^ meson.build
			printf("Downloading meson.build...\n\n");
			downloadFileAndReplace(
				outputDir,
				"meson.build",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/meson.build",
				projectName,
				projectId,
				authorName,
				license,
				false, false);

			//^ README.md
			printf("Downloading README.md...\n\n");
			downloadFileAndReplace(
				outputDir,
				"README.md",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/README.md",
				projectName,
				projectId,
				authorName,
				license,
				false, false);

			//^ Flatpak manifest
			printf("Downloading %s.json...\n\n", projectId);
			downloadFileAndReplace(
				outputDir,
				"json",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/.json",
				projectName,
				projectId,
				authorName,
				license,
				false, true);

			//^ src/
			printf("Downloading src/%s-application.c...\n\n", PROJECT_FILENAME);
			char* src_dir_tmp = path_join((char*[]){ outputDir, "src"}, 2);
			mkdir_p(src_dir_tmp);

			downloadFileAndReplace(
				src_dir_tmp,
				"application.c",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/application.c",
				projectName,
				projectId,
				authorName,
				license,
				true, false);

			printf("Downloading src/%s-application.h...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"application.h",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/application.h",
				projectName,
				projectId,
				authorName,
				license,
				true, false);

			printf("Downloading src/%s-window.c...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"window.c",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/window.c",
				projectName,
				projectId,
				authorName,
				license,
				true, false);

			printf("Downloading src/%s-window.h...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"window.h",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/window.h",
				projectName,
				projectId,
				authorName,
				license,
				true, false);

			printf("Downloading src/%s-window.ui...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"window.ui",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/window.ui",
				projectName,
				projectId,
				authorName,
				license,
				true, false);

			printf("Downloading src/%s-shortcuts.ui...\n\n", PROJECT_FILENAME);
			downloadFileAndReplace(
				src_dir_tmp,
				"shortcuts.ui",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/shortcuts.ui",
				projectName,
				projectId,
				authorName,
				license,
				true, false);

			printf("Downloading src/%s.gresource.xml...\n\n", PROJECT_FILENAME);
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
				license,
				false, false);
			free(gresource_name_temp); gresource_name_temp = NULL;

			printf("Downloading src/main.c...\n\n");
			downloadFileAndReplace(
				src_dir_tmp,
				"main.c",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/main.c",
				projectName,
				projectId,
				authorName,
				license,
				false, false);

			printf("Downloading src/meson.build...\n\n");
			downloadFileAndReplace(
				src_dir_tmp,
				"meson.build",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/src/meson.build",
				projectName,
				projectId,
				authorName,
				license,
				false, false);
			free(src_dir_tmp); src_dir_tmp = NULL;

			//^ src/gtk
			char* gtk_dir_tmp = path_join((char*[]){ outputDir, "src", "gtk"}, 3);
			mkdir_p(gtk_dir_tmp); free(gtk_dir_tmp); gtk_dir_tmp = NULL;

			//^ po/
			char* po_dir_tmp = path_join((char*[]){ outputDir, "po"}, 2);
			mkdir_p(po_dir_tmp);

			printf("Downloading po/meson.build...\n\n");
			downloadFileAndReplace(
				po_dir_tmp,
				"meson.build",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/po/meson.build",
				projectName,
				projectId,
				authorName,
				license,
				false, false);

			printf("Downloading po/POTFILES.in...\n\n");
			downloadFileAndReplace(
				po_dir_tmp,
				"POTFILES.in",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/po/POTFILES.in",
				projectName,
				projectId,
				authorName,
				license,
				false, false);

			printf("Downloading po/LINGUAS...\n\n");
			downloadFileAndReplace(
				po_dir_tmp,
				"LINGUAS",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/po/LINGUAS",
				projectName,
				projectId,
				authorName,
				license,
				false, false);
			free(po_dir_tmp); po_dir_tmp = NULL;

			//^ data/
			char* data_dir_tmp = path_join((char*[]){ outputDir, "data"}, 2);
			mkdir_p(data_dir_tmp);

			char* icon_dir_tmp = path_join((char*[]){ outputDir, "data", "icons"}, 3);
			mkdir_p(icon_dir_tmp);

			printf("Downloading data/%s.metainfo.xml.in...\n\n", projectId);
			downloadFileAndReplace(
				data_dir_tmp,
				"metainfo.xml.in",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/data/metainfo.xml.in",
				projectName,
				projectId,
				authorName,
				license,
				false, true);

			printf("Downloading data/%s.gschema.xml...\n\n", projectId);
			downloadFileAndReplace(
				data_dir_tmp,
				"gschema.xml",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/data/gschema.xml",
				projectName,
				projectId,
				authorName,
				license,
				false, true);

			printf("Downloading data/%s.desktop.in...\n\n", projectId);
			downloadFileAndReplace(
				data_dir_tmp,
				"desktop.in",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/data/desktop.in",
				projectName,
				projectId,
				authorName,
				license,
				false, true);

			printf("Downloading data/meson.build...\n\n");
			downloadFileAndReplace(
				data_dir_tmp,
				"meson.build",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/data/meson.build",
				projectName,
				projectId,
				authorName,
				license,
				false, false);

			//^ data/icons/
			printf("Downloading data/icons/meson.build...\n\n");
			downloadFileAndReplace(
				icon_dir_tmp,
				"meson.build",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/data/icons/meson.build",
				projectName,
				projectId,
				authorName,
				license,
				false, false);

			char* scalable_apps_dir_tmp = path_join((char*[]){outputDir, "data", "icons", "hicolor", "scalable", "apps"}, 6);
			char* symbolic_apps_dir_tmp = path_join((char*[]){outputDir, "data", "icons", "hicolor", "symbolic", "apps"}, 6);
			mkdir_p(scalable_apps_dir_tmp); mkdir_p(symbolic_apps_dir_tmp);

			printf("Downloading data/icons/hicolor/scalable/apps/%s.svg...\n\n", projectId);
			downloadFileAndReplace(
				scalable_apps_dir_tmp,
				"svg",
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/data/icons/hicolor/scalable/apps/svg",
				projectName,
				projectId,
				authorName,
				license,
				false, true);

			char* symbolic_name_tmp = malloc(( strlen(projectId) + strlen("-symbolic.svg") + 1 ) * sizeof(char));
			if (symbolic_name_tmp == NULL) cgp_throw(MEM_ERR, "");
			sprintf(symbolic_name_tmp, "%s-symbolic.svg", projectId);

			printf("Downloading data/icons/hicolor/symbolic/apps/%s-symbolic.svg...\n\n", projectId);
			downloadFileAndReplace(
				symbolic_apps_dir_tmp,
				symbolic_name_tmp,
				"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/data/icons/hicolor/symbolic/apps/svg",
				projectName,
				projectId,
				authorName,
				license,
				false, false);
			free(symbolic_name_tmp); symbolic_name_tmp = NULL;

			free(scalable_apps_dir_tmp); scalable_apps_dir_tmp = NULL;
			free(symbolic_apps_dir_tmp); symbolic_apps_dir_tmp = NULL;
			free(icon_dir_tmp); icon_dir_tmp = NULL;
			free(data_dir_tmp); data_dir_tmp = NULL;

			if (!strcmp(editor, "vscode")) {
				char* vscode_dir_tmp = path_join((char*[]){outputDir, ".vscode"}, 2);
				mkdir_p(vscode_dir_tmp);

				printf("Downloading .vscode/launch.json...\n\n");
				downloadFileAndReplace(
					vscode_dir_tmp,
					"launch.json",
					"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/.vscode/launch.json",
					projectName,
					projectId,
					authorName,
					license,
					false, false);

				printf("Downloading .vscode/tasks.json...\n\n");
				downloadFileAndReplace(
					vscode_dir_tmp,
					"tasks.json",
					"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/.vscode/tasks.json",
					projectName,
					projectId,
					authorName,
					license,
					false, false);

				free(vscode_dir_tmp); vscode_dir_tmp = NULL;
			} else if (!strcmp(editor, "rider")) {
				char* rider_dir_tmp = path_join((char*[]){outputDir, ".idea", "runConfigurations"}, 3);
				mkdir_p(rider_dir_tmp);

				printf("Downloading .idea/runConfigurations/Build_and_debug_GTK4_app.xml...\n\n");
				downloadFileAndReplace(
					rider_dir_tmp,
					"Build_and_debug_GTK4_app.xml",
					"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/.idea/runConfigurations/Build_and_debug_GTK4_app.xml",
					projectName,
					projectId,
					authorName,
					license,
					false, false);

				printf("Downloading .idea/runConfigurations/build_debug.sh...\n\n");
				downloadFileAndReplace(
					rider_dir_tmp,
					"build_debug.sh",
					"https://raw.githubusercontent.com/Proman4713/create-gnome-project/refs/heads/main/templates/c/gtk/.idea/runConfigurations/build_debug.sh",
					projectName,
					projectId,
					authorName,
					license,
					false, false);

				free(rider_dir_tmp); rider_dir_tmp = NULL;
			}
		} else {
			cgp_throw(PROGRAM_ERR, NOT_IMPLEMENTED);
		}
	} else {
		cgp_throw(PROGRAM_ERR, NOT_IMPLEMENTED);
	}

	finishAndGreet(isExtension, isLibadwaita, doGit, outputDir);

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