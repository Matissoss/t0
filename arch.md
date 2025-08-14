<div align=center>
    <h1>arch.md</h1>
</div>

## about

`t0` is 8-bit RISC CPU architecture made for fun.

## registers

`t0` contains 2 8-bit GPR (`t0` and `t1`) and 2 reserved registers (16-bit `pc` and 8-bit `sp`).

`sp` is maxed on start (`sp = 255`).

## instruction format

`t0` orders are fixed at 1 byte (8-bit).

`t0` order layout:

```
+---+---+---+---+---+---+---+---+
|   opcode      |   immediate   | (1B)
+---+---+---+---+---+---+---+---+
```

## instructions

### Arithmetical instructions

- `AND`:
    - opcode: 0x0
    - operation: `t0 &= t1`
    - immediate: 0
- `OR`:
    - opcode: 0x0
    - operation: `t0 |= t1`
    - immediate: 1
- `XOR`:
    - opcode: 0x0
    - operation: `t0 |= t1`
    - immediate: 2
- `NOT`:
    - opcode: 0x0
    - operation: `t0 = !t1`
    - immediate: 3
- `LSH`:
    - opcode: 0x0
    - operation: `t0 <<= t1`
    - immediate: 4
- `RSH`:
    - opcode: 0x0
    - operation: `t0 >>= t1`
    - immediate: 5
- `INCS` (Increment `sp`):
    - opcode: 0x0
    - operation: `sp += 1`
    - immediate: 6
- `DECS` (Decrement `sp`):
    - opcode: 0x0
    - operation: `sp += 1`
    - immediate: 7
- `INC`:
    - opcode: 0x0
    - operation: `t0++`
    - immediate: 8
- `DEC`:
    - opcode: 0x0
    - operation: `t0--`
    - immediate: 9

### Memory instructions

- `LD`:
    - opcode: 0x1
    - operation: `t0 = ram[t1]`
    - immediate: 0
- `ST`:
    - opcode: 0x1
    - operation: `ram[t1] = t0`
    - immediate: 1
- `PPC`:
    - opcode: 0x1
    - operation: `sp -= 2; push (pc & (0xFF << 8)) >> 8; push pc & 0xFF`
    - immediate: 2
- `LSP`:
    - opcode: 0x1
    - operation: `t0 = sp`
    - immediate: 3
- `LLS` (Load Last on Stack):
    - opcode: 0x1
    - operation: `t0 = ram[sp]` (does not pop stack)
    - immediate: 4
- `SLS` (Store Last on Stack):
    - opcode: 0x1
    - operation: `ram[sp] = t0`
    - immediate: 5

### t0 immediate instructions

- `S0LO` (Set LO bits):
    - opcode: 0x2
    - operation: `t0 = t0 & 0xF0; t0 |= imm`
    - immediate: varies
- `S0HI` (Set HI bits):
    - opcode: 0x3
    - operation: `t0 = t0 & 0x0F; t0 |= imm << 4`
    - immediate: varies

### t1 immediate instructions

- `S1LO` (Set LO bits):
    - opcode: 0x4
    - operation: `t1 |= imm`
    - immediate: varies
- `S1HI` (Set HI bits):
    - opcode: 0x5
    - operation: `t1 |= (imm) << 4`
    - immediate: varies

### condition instructions

- `CMP`:
    - opcode: 0x6
    - operation: `t0 = t0 cmp t1`
    - immediate: condition code 
- `JMP`:
    - opcode: 0x6
    - operation: `pc = ram[sp] | ram[sp + 1] << 4; sp += 2`
    - immediate: jump condition code << 4

### misc instructions

- `NOP`:
    - opcode: 0x7
    - operation: `NOP`
    - immediate: 0
- `SWAP`:
    - opcode: 0x7
    - operation: `t0, t1 = t1, t0`
    - immediate: 1
- `HLT`:
    - opcode: 0x7
    - operation: `halt CPU`
    - immediate: 2

### emulator instructions

Emulator instructions have opcode 0xA to 0xF and vary on implementation.

- `IN`:
    - opcode: 0xA
    - operation: `t0 = 0; scanf(" %c", t0);`
    - immediate: 0
- `OUT`:
    - opcode: 0xA
    - operation: `printf("%c", t0)`
    - immediate: 1

## appendix A: condition codes

- 0b0000: EQ
- 0b0001: GT
- 0b0010: GTE
- 0b0011: LT
- 0b0100: LTE

## appendix B: jump codes

- 0b0001: T (t0 == 1)
- 0b0010: F (t0 == 0)
- 0b0011: N (No Condition)
