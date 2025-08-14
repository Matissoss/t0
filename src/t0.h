#include<stdio.h>
#include<stdint.h>

static uint8_t ram[256];

static uint16_t pc = 0;
static uint8_t sp = 255;

static uint8_t t0 = 0;
static uint8_t t1 = 0;

typedef enum {
	E_OK, // everything fine
	E_UD, // undefined
	E_HLT, // halt execution
} t0_err;

int t0_interpret(uint8_t instruction);
int t0_exec(FILE* file);
