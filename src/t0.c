#include<stdlib.h>
#include<stdint.h>
#include "t0.h"

int t0_interpret(uint8_t instruction) {
	uint8_t opcode = (instruction & 0xF0) >> 4;
	uint8_t imm = instruction & 0x0F;

	switch (opcode) {
		case 0x0:
			switch (imm) {
				case 0x0:
					t0 &= t1;
					break;
				case 0x1:
					t0 |= t1;
					break;
				case 0x2:
					t0 ^= t1;
					break;
				case 0x3:
					t0 = !t1;
					break;
				case 0x4:
					t0 <<= t1;
					break;
				case 0x5:
					t0 >>= t1;
					break;
				case 0x6:
					sp++;
					break;
				case 0x7:
					sp--;
					break;
				case 0x8:
					t0++;
					break;
				case 0x9:
					t0--;
					break;
				default: return E_UD;
			}
			return E_OK;
		case 0x1:
			switch (imm) {
				case 0x0:
					t0 = ram[t1];
					break;
				case 0x1:
					ram[t1] = t0;
					break;
				case 0x2:
					ram[sp] = (pc & 0xFF00) >> 8;
					ram[sp - 1] = pc & 0xFF;
					break;
				case 0x3:
					t0 = sp;
					break;
				case 0x4:
					t0 = ram[sp];
					break;
				case 0x5:
					ram[sp] = t0;
					break;
				default: return E_UD;
			}
			return E_OK;
		case 0x2:
			t0 = t0 & 0xF0;
			t0 |= imm;
			return E_OK;
		case 0x3:
			t0 = t0 & 0x0F;
			t0 |= imm << 4;
			return E_OK;
		case 0x4:
			t1 = t1 & 0xF0;
			t1 |= imm;
			return E_OK;
		case 0x5:
			t1 = t1 & 0x0F;
			t1 |= imm << 4;
			return E_OK;
		case 0x6:
			switch (imm) {
				case 0:
					t0 = t0 == t1;
					break;
				case 1:
					t0 = t0 > t1;
					break;
				case 2:
					t0 = t0 >= t1;
					break;
				case 3:
					t0 = t0 < t1;
					break;
				case 4:
					t0 = t0 <= t1;
					break;
				case 0b00010000:
					if (t0 == 1) {
						pc = ram[sp] << 8 | ram[sp + 1];
						sp += 2;
					}
					break;
				case 0b00100000:
					if (t0 == 0) {
						pc = ram[sp] << 8 | ram[sp + 1];
						sp += 2;
					}
					break;
				case 0b00110000:
					pc = ram[sp] << 8 | ram[sp + 1];
					sp += 2;
					break;
				default: return E_UD;
			}
			return E_OK;
		case 0x7:
			switch (imm) {
				case 0: break;
				case 1:
					uint8_t tmp = t0;
					t0 = t1;
					t1 = tmp;
					break;
				case 2: return E_HLT;
				default: return E_UD;
			}
			return E_OK;
		case 0x8:
			switch (imm) {
				case 0:
					t0 = 0;
					scanf(" %c", &t0);
					break;
				case 1:
					printf("%c", t0);
					break;
			}
			return E_OK;
		default: return E_UD;
	}
}

int t0_exec(FILE* file) {
	uint8_t buf;
	uint8_t tmp_pc;
	while (fread(&buf, 1, 1, file) != 0) {
		tmp_pc = pc;
		int ecode = t0_interpret(buf);
		if (ecode != E_OK)
			return ecode;
		if (tmp_pc == pc)
			pc++;
		fseek(file, (long)pc, SEEK_SET);
	}
	return E_OK;
}
