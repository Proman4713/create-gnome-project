# create-gnome-project
### Video Demo:  https://youtu.be/RDS79fLvndw
### Run `make` and then `./dist/create-gnome-project -p gtk` to use this program properly.
### Description:
A simple C utility that creates a template GNOME shell extension or GTK4 application.

### Details:
This program is a CLI utility, written in C, which takes in user input to create a template GNOME-ecosystem project such as a GTK4 app, GTK4/Libadwaita, or a GNOME shell extension in either C or JavaScript. I have currently only implemented the GTK4 C template, although I've structured my code to easily allow expansion.

You usually can't create such a GNOME project without installing a GUI app like GNOME builder for apps and the GNOME shell extension manager for extensions. This can increase the friction before someone can get started and, for someone like me, deny them that ability as they're on WSL and only infrequently access a GNOME system. So I made this&hellip;

### Structure:
This project contains a main.c, a makefile, a helpers/ directory, a debian/ directory and a templates/ directory.

`main.c` contains the runtime logic for the app&hellip; It starts by setting up the different arguments used by my program and then downloads the required files based on the template matched by the user's choices. It mostly uses functions from other source files. It took a lot of thinking to figure out how or where to abstract away code while allowing flexibility and avoiding memory leaks. I decided to have it download files from my GitHub repository rather than reference them locally, so the user only downloads what they need, and the Debian package stays slim even as templates grow larger.

The helpers/ directory contains all the other source code.

### Source code:
`cgp_args.c` declares all the valid arguments for my program in an array of arrays, and then implements the following functions:
- `cgp_printHelp` uses the `CGP_OPTIONS` array to print a help message dynamically based on whether or not short and long versions of an option are available, as well as a description. To avoid issues with fixed tab stops, this function subtracts a total of 32 spaces from the length of the help entry, and appends that at the end. This function shows a bit of the benefits of C not trying to be helpful (`printf` without newlines).
- `cgp_searchArgs` returns `true` if an argument was passed to the program.
- `cgp_getArgValue` gets the argument *after* the one required to determine its value.
- `cgp_validateArgs` makes sure all the arguments are supported.
- `cgp_getOrPromptArg` is perhaps the most interesting function in this file. It takes an argument from `argv` or, if it wasn't passed and the option isn't required, prompts the user for it with a default value and validates the it needed.

`cgp_errors.c` and its corresponding header file are pretty simple; they provide an enum of common errors in this program and a function called `cgp_throw` to, well, throw the error and exit the program.

`cgp_templates.c` contains the templating logic and implements these functions:
- `downloadFileAndIterate` abstracts away the process of downloading a file to a `.tmp` destination first, then doing find-and-replace over each line. It uses different file I/O operations as well as functions from other files.
- `downloadFileAndReplace` used to include the logic of `downloadFileAndIterate`. It sets up all the values used in the templating process (such as the spacey code dedicated to license headers) and then calls `downloadFileAndIterate`.
- `projectDataToFile` joins a piece of project data to a filename with a separator; this is used for template filenames.
- `implantLicenseFile` adds a `LICENSE` file in the generated project's output directory based on the user's choice.
- `finishAndGreet` runs `git init` if the user requested it and then `meson setup` for GTK4 apps to get the user started with editor/IDE integrations right away. It then wishes the user good luck 🍀

`cgp_utils.c` is the *most* interesting and functionally the densest source file in this project. It contains utilities used by everything else, and is mostly dedicated to replicating basic higher-level functionality.
- `get_string_withDefault` wraps *CS50's* own `get_string` function with a default prompt and fallback value.
- `downloadFile` is a `libcurl` wrapper which uses it to set options for CURL and perform a request.
- `mkdir_p` could've been recursive (or so I initially thought). Since `mkdir` requires that all *parent* directories be existent, rather than child ones, we don't need to recurse our way down the file path and back. It uses dynamic arrays and string operations to repeatedly check if a directory is valid and create it. It is a re-implementation of the `-p` flag in the coreutils `mkdir` command.
- `path_join` properly joins multiple path segments together.
- `String_toLowercase` does exactly as its name suggests.
- `String_toUppercase` yep.
- `String_toPascalcase` is more interesting; it's my design of an algorithm to convert words to PascalCase.
- `String_startsWith` compares a string and a prefix pattern.
- `String_endsWith` is similar, however, it requires looping from the *end* of the string and matching that with the pattern.
- `String_replaceAll` is an algorithm that took a lot of trial and error to get right. I am still unsure of whether it handles all possible edge cases. It takes a pattern and its replacement and goes through a string to replace it while avoiding memory issues. It *looks* to me like it's in O(n) considering it loops `n * k` times where `k` is the length of the pattern, however, the larger `k` is, the less times we loop so `n` decreases. So I'm not sure, and I haven't measured it, and functions like `strcmp`, `strcpy` and `strcat` have their own loops inside, so I'll leave that up for discussion.
- `String_split` is similar to `String_replaceAll`; it finds a pattern and then *splits* the string there rather than replace it.
- `String_replaceAllMulti` is a for loop of `String_replaceAll`.
- `charcpy` and `charcat` are `strcpy` and `strcat` wrappers for adding single characters to a string.
- `Int_toString` converts 32-bit integers to strings.
- `IntArray_includes` and `StringArray_includes` are linear search algorithms since we have no guarantee of sorted input.
- `IntArray_indexOf` and `StringArray_indexOf` return the indices rather than booleans.
- `new_DynIntArray` uses the correspondingly named struct declared in `cgp_utils.h`. This is my unguided implementation of dynamic arrays; it uses the input provided to it, checks for NULL, then `malloc`s a pointer to an integer (i.e., an integer array) and fills it, while setting a `length` value inside the struct to constantly keep track of it across all later operations.
- `new_DynStringArray` is adapted because strings are different (and `strncpy`d).
- `DynIntArray_push` pushes an element to the array while checking for `UINT_MAX` and other memory issues.
- `DynStringArray_push`, like `new_`, adapts to strings' higher potential for issues compared to assigning an integer value.
- `DynIntArray_pop` was a discovery that `realloc` can be used to shrink memory and *free* values at the end of an array.
- `DynStringArray_pop` also needed some adaptation; we can't just shrink the array to free the last element, we need to free the element itself since it's a pointer.
- `DynIntArray_includes`, `DynIntArray_indexOf`, `DynStringArray_includes` and `DynStringArray_indexOf` achieve that functionality without needing to know the length of an array beforehand and with NULL checks.
- `DynIntArray_free` frees the integer array with a simple `free()`, however, `DynStringArray_free` loops to free each string independently first.

#### The `debian/` directory contains simple metadata for the Debian package for this program, and the `templates/` directory contains code that, for the largest part, *I did not write&hellip;* It is standard GTK4 boilerplate.