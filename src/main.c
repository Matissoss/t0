#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>

static uint8_t CONDITION_EQ  = 0;
static uint8_t CONDITION_GT  = 1;
static uint8_t CONDITION_GTE = 2;
static uint8_t CONDITION_LT  = 3;
static uint8_t CONDITION_LTE = 4;

static char* VERSION = "t0-v1";

typedef struct {
	uint8_t opcode;
	uint8_t imm;
} t0_instruction;

typedef enum {
	E_NONE,
	E_UNDEFINED,
	E_STACK,
} t0_error;

typedef enum {
	M_ADD,
	M_SUB,
	M_AND,
	M_OR,
	M_XOR,
	M_NOT,
	M_PUSH,
	M_LDI,
	M_POP,
	M_LD,
	M_CMP,
	M_PPC,
	M_RET,
	M_JMP,
	M_ST,
	M_PSP,
	M_OUT,
	M_IN,
} t0_mnemonic;

uint8_t ram[256];

uint8_t t0 = 0;
uint8_t sp = 0;
uint16_t pc = 0;

int push_stack(uint8_t b) {
	ram[sp] = b;
	sp += 1;
	return 0;
}
uint8_t pop_stack() {
	sp -= 1;
	return ram[sp - 1];
}

int interpret(t0_instruction* ins) {
	uint8_t rhs;
	switch (ins->opcode) {
		case M_CMP:
			switch (ins->imm) {
				case 0:
					t0 = t0 == pop_stack();
					break;
				case 1:
					t0 = t0 > pop_stack();
					break;
				case 2:
					t0 = t0 >= pop_stack();
					break;
				case 3:
					t0 = t0 < pop_stack();
					break;
				case 4:
					t0 = t0 <= pop_stack();
					break;
				default:
					return E_UNDEFINED;
			}
			break;
		case M_JMP:
			switch (ins->imm) {
				case 0:
					if (t0 == 1) {
						pc |= pop_stack() << 8;
						pc |= pop_stack();
					} else {
						return E_NONE;
					}
					break;
				case 1:
					if (t0 == 0) {
						pc |= pop_stack() << 8;
						pc |= pop_stack();
					} else {
						return E_NONE;
					}
					break;
				case 2:
					pc |= pop_stack() << 8;
					pc |= pop_stack();
					break;
				default:
					return E_UNDEFINED;
			}
			break;
		case M_RET:
			pc = 0;
			pc |= pop_stack() << 8;
			pc |= pop_stack();
			break;
		case M_OUT:
			printf("%c", t0);
			break;
		case M_IN:
			rhs = '\0';
			scanf(" %c", &rhs);
			t0 = rhs;
			break;
		case M_ST:
			ram[t0] = pop_stack();
			break;
		case M_LD:
			t0 = ram[t0];
			break;
		case M_LDI:
			t0 = ins->imm;
			break;
		case M_PPC:
			push_stack((uint8_t)(pc & 0xFF));
			push_stack((uint8_t)((pc & 0xFF00) >> 8));
			break;
		case M_PSP:
			push_stack((uint8_t)(sp & 0xFF));
			push_stack((uint8_t)((sp & 0xFF00) >> 8));
			break;
		// arithmetical instructions
		case M_NOT:
			t0 != t0;
			break;
		case M_OR:
			t0 |= ram[sp];
			break;
		case M_AND:
			t0 &= ram[sp];
			break;
		case M_XOR:
			t0 ^= ram[sp];
			break;
		case M_SUB:
			t0 -= ram[sp];
			break;
		case M_ADD:
			t0 += ram[sp];
			break;
		default:
			return E_UNDEFINED;
	}
	return E_NONE;
}

t0_instruction decode(uint8_t cd[2]) {
	t0_instruction i;
	i.opcode = cd[0];
	i.imm = cd[1];
	return i;
}

int execution_loop(FILE* file) {
	uint8_t cd[2];
	while (fread(&cd, 2, 2, file) != 0) {
		uint8_t pc_b = pc;
		t0_instruction i = decode(cd);
		pc += 2;
		int res = interpret(&i);
		if (res != E_NONE) {
			return res;
		}
		// jump occured
		if (pc_b != pc) {
			fseek(file, pc, 0);
		}
	}
	return E_NONE;
}

int main(int argv, char** argc) {
	int idx = 1;
	char* file_name = NULL;
	while (idx < argv) {
		if (strcmp(argc[idx], "-h") == 0 || strcmp(argc[idx], "--help") == 0) {
			printf("Help Needed\n");
			return 0;
		} else if (strcmp(argc[idx], "-v") == 0 || strcmp(argc[idx], "--version") == 0) {
			printf("%s\n", VERSION);
			return 0;
		} else {
			if (file_name == NULL) {
				file_name = argc[idx];
			} else {
				printf("error: unknown argument/flag: %s!\n", argc[idx]);
				return 1;
			}
		}
		if (file_name == NULL) {
			file_name = argc[idx];
		}
		idx += 1;
	}
	FILE* file = fopen(file_name, "rb");

	if (file == NULL) {
		printf("error: file that you tried to access, could not be opened!\n");
		return 1;
	}
	int x = execution_loop(file);
	fclose(file);
	switch (x) {
		case E_UNDEFINED:
			return 1;
		case E_STACK:
			return 2;
		case E_NONE:
			return 0;
	}
}
