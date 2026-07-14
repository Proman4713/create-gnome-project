#include <stdbool.h>
#include <stdlib.h>

// 30 maximum options
#define MAX_OPTIONS 30

extern const char* CGP_OPTIONS[MAX_OPTIONS][3];

bool cgp_searchArgs(int argc, char* argv[], char* shortArg, char* longArg);
char* cgp_getArgValue(int argc, char* argv[], char* shortArg, char* longArg);
void cgp_validateArgs(int argc, char* argv[]);