#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>

#include "t0.h"

static char* VERSION = "t0-v2";

void print_help(){
	printf("USAGE: t0 [FLAG] [PATH]\n");
	printf("[FLAG]:\n");
	printf("\t-v/--version\n");
	printf("\t-h/--help\n");
	printf("-----\n");
	printf("made by matissoss\nlicensed under 3-clause BSD");
	fflush(stdout);
}

int main(int argc, char** argv) {
	char* file_path = NULL;
	for (size_t idx = 1; idx < argc; idx++) {
		char* arg = argv[idx];
		if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help")) {
			print_help();
			return 0;
		} else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
			printf("%s\n", VERSION);
			return 0;
		} else {
			file_path = arg;
		}
	}
	if (file_path == NULL) {
		printf("ERROR: no file was provided\n");
		return 1;
	}
	FILE* file = fopen(file_path, "rb");
	if (file == NULL) {
		printf("ERROR: file you provided does not exist\n");
		return 1;
	}
	switch (t0_exec(file)) {
		case E_HLT:
		case E_OK: return 0;
		default: return 1;
	}
	return 0;
}
