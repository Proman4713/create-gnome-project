#include <stdbool.h>
#include <stdlib.h>

// 30 maximum options
#define MAX_OPTIONS 30

extern const char* CGP_OPTIONS[MAX_OPTIONS][3];

bool cgp_searchArgs(const int argc, const char* argv[], const char* shortArg, const char* longArg);
char* cgp_getArgValue(const int argc, const char* argv[], const char* shortArg, const char* longArg);
void cgp_validateArgs(const int argc, const char* argv[]);