<div align=center>
    <h1>arch.md</h1>
</div>

## about

`t0` is 8-bit RISC CPU architecture made for fun.

## registers

`t0` contains 1 8-bit GPR (`t0`) and 2 reserved registers (16-bit `pc` and 8-bit `sp`).

## instruction format

`t0` orders are fixed at 2 bytes (16-bit).

`t0` order layout:

```
+---+---+---+---+---+---+---+---+
|            opcode             | (1B)
+---+---+---+---+---+---+---+---+
|           immediate           | (1B)
+---+---+---+---+---+---+---+---+
```

## instructions

> [!NOTE]
> `lz` abbreviation stands for last element on stack.

- ADD:
    - opcode: 0x00
    - operation: `t0 += lz`
    - uses immediate: no
- SUB:
    - opcode: 0x01
    - operation: `t0 -= lz`
    - uses immediate: no
- AND:
    - opcode: 0x02
    - operation: `t0 &= lz`
    - uses immediate: no
- OR:
    - opcode: 0x03
    - operation: `t0 |= lz`
    - uses immediate: no
- XOR:
    - opcode: 0x04
    - operation: `t0 ^= lz`
    - uses immediate: no
- NOT:
    - opcode: 0x05
    - operation: `t0 != t0`
    - uses immediate: no
- PUSH:
    - opcode: 0x06
    - operation: pushes `t0` to stack
    - uses immediate: no
- LDI:
    - opcode 0x07
    - operation: `t0 = imm`
    - uses immediate: yes
- POP:
    - opcode: 0x08
    - operation: pops stack to `t0`
    - uses immediate: no
- LD:
    - opcode: 0x09
    - operation: `t0 = ram[t0]`
    - uses immediate: no
- CMP:
    - opcode: 0x0A
    - operation: `t0 = t0 cmp lz` ()
    - uses immediate: yes (for condition)
- PPC:
    - opcode: 0x0B
    - operation: pushes current value of `pc` to stack
    - uses immediate: no
- RET:
    - opcode: 0x0C
    - operation: jumps to `pc`; pops stack twice
    - uses immediate: no
- JMP:
    - opcode: 0x0D
    - operation: `pc = stack[sp] + stack[sp - 1] << 8`; pops stack twice
    - uses immediate: yes (for jump condition)
- ST:
    - opcode: 0x0E
    - operation: `ram[t0] = lz`; pops stack
    - uses immediate: no
- PSP:
    - opcode: 0x0F
    - operation: pushes current value of `sp` to stack
    - uses immediate: no
- OUT:
    - opcode: 0x10
    - operation: prints value of `t0` to stdout (as ascii character)
    - uses immediate: no
- IN:
    - opcode: 0x11
    - operation: takes value from `stdin` and puts into `t0`

## appendix A: condition codes

- 0b000: EQ
- 0b001: GT
- 0b010: GTE
- 0b011: LT
- 0b100: LTE

## appendix B: jump codes

- 0b000: T (t0 == 1)
- 0b001: F (t0 == 0)
- 0b010: N (No Condition)
