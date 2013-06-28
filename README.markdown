DASM16
========

An assembler for DCPU-16 development, written in C++.

1. [Introduction](https://github.com/majestic53/dasm16#introduction)
2. [Usage](https://github.com/majestic53/dasm16#usage)
3. [How It Works](https://github.com/majestic53/dasm16#how_it_works)
4. [Example](https://github.com/majestic53/dasm16#example)
5. [Syntax](https://github.com/majestic53/dasm16#syntax)
6. [License](https://github.com/majestic53/dasm16#license)

Introduction
========

DASM16 is an 16-bit assembler built for development on the fictional DCPU-16 
processor from the game 0x10c. For more information on the DCPU-16 processor, see: 
http://0x10cwiki.com/wiki/DCPU-16.

Usage
========

__dasm16 [-h | -v] [-o OUTPUT] input...__

* __-h__: Display help menu
* __-v__: Enable verbose mode
* __-o__ OUTPUT: Specify output file path
* input: Specify input file path

###Usage Example

For the sake of this example, we will assume that we have an input file called 
__code.asm__. If we wish to compile this file into a binary called __out.bin__
with verbose output, we would use the command below:

```
dasm16 -v -o out.bin code.asm
```

######Output:

```
Initializing... Done.
Building source file(s)... pass 1... pass 2... Done.
Writing to file... Done.
Build successful.
```

How It Works
========

Internally, DASM16 consists of three major pieces (that can be thought of as a stack), that 
perform the three basic steps required for assembly.

![Software stack](http://dl.dropboxusercontent.com/u/6410544/dasm16/stack.png)

1. The first step, called 'tokenizing', breaks the the assembly (source) file into descrete pieces
through a process known as lexical analysis. These descrete pieces of code produced through this
procerss are called tokens, and are used to check for syntactic/symantic correctness in the 
following steps.

![Token generation](http://dl.dropboxusercontent.com/u/6410544/dasm16/lexer.png)

2. Once the code has been broken into tokens, the next step is to determine if they are placed in the
right order. This is what is known as syntactic analysis, or checking that the code is correctly 
formatted. Using this process, the tokens are used to create trees of tokens, called syntax trees.

![Syntax tree generation](http://dl.dropboxusercontent.com/u/6410544/dasm16/parser.png)

3. The finaly step is to generate the code based off the syntax trees generated earlier. To do this, we
must "walk over" the trees. During this process, we perform symantic analysis, or checking that the 
syntax trees make sense.

If all goes well, the binary code (output) will be generated successfully, and written to a file.

Example
========

For this example, we will look at the classic Hello World example, showing the 
entire process of compilation. First step, create a new file; call it __HelloWorld.asm__.
This file will hold our assembly code, and will be used as an input file during 
compilation. 

Once you've created this file, copy and paste the code below into your newly created 
Hello World assembly file.

```asm
; Assembler test for DCPU
; by Markus Persson

	set a, 0xbeef
	set [0x1000], a
	ifn a, [0x1000]
	set PC, end
	
	set i, 0
:nextchar ife [data+i], 0
	set PC, end
	set [0x8000+i], [data+i]
	add i, 1
	set PC, nextchar

:data dat "Hello world!", 0

:end sub PC, 1
```

Now that we have the code, we are ready to compile. Save the file and open a console 
window in the same directly. Next, use the command below to compile your code.

```
dasm16 -v -o HelloWorld.bin HelloWorld.asm
```

You should see output in the console window similar to this (notice that the output shows in
the console windows when using verbose mode):

```
DASM16 1.0.1326 (rev. 7, Jun 27 2013, 20:24:38)
Initializing... Done.
Building source file(s)... pass 1... pass 2... Done.
Writing to file... Done.
0000 | 7c01 beef 03c1 1000 7813 1000 7f81 0020
0008 | 84c1 86d2 0013 7f81 0020 5ac1 0013 8000
0010 | 88c2 7f81 0009 0048 0065 006c 006c 006f
0018 | 0020 0077 006f 0072 006c 0064 0021 0000
0020 | 8b83
Build successful. (0.015 sec.)
```

Thats it! You should now see a file called _HelloWorld.bin_ in the same directory
as your assembly file.

Syntax
========

DASM16 is fully compliant with the DCPU-16 1.7 specification as supplied by Notch.

###Addressing Modes

The DCPU-16 processor supports 12 unique addressing modes:

```
1. Register: <register>
2. Indirect Register: [ <register> ]
3. Indirect Register + Next Word: [ <register> + <static_value> ] | [ <static_value> + <register> ]
4. Push/Pop: PUSH | POP
5. Peek: PEEK
6. Pick(n): PICK ( <static_value> ) | PICK <static_value>
7. SP: SP
8. PC: PC
9. EX: EX
10. Indirect Next-Word Literal: [ <static_value> ]
11. Next-Word literal: <static_value>
12. Literal: # <static_value> | <static_value> (NOTE: only applies to values from -1 - 30)
```

###Commands

The DCPU-16 processor supports two different types of opcodes: 

######Basic opcodes refer to opcodes with the format: 

```
<opcode> <operator_b>, <operator_a>

SET | ADD | SUB | MUL | MLI | DIV | DVI | MOD | MDI
| AND | BOR | XOR | SHR | ASR | SHL | IFB | IFC 
| IFE | IFN | IFG | IFA | IFL | IFU | ADX | SBX
| STI | STD
```

######Special opcodes refer to opcodes with the format:

```
<opcode> <operator_a>

JSR | INT | IAG | IAS | RFI | IAQ | HWN | HWQ
```

###Directives

The DASM16 supports a series of assembler directives with the format: <directive> <value_list>

```
DAT
```

###Registers

The DCPU-16 contains 8 general purpose registers, and 4 system registers.

```
A | B | C | X | Y | Z | I | J | EX | IA | PC | SP
```

###Assembly BNF

Listed below is the Backus–Naur Form for the DCPU-16 assembly:

```
command ::= <basic_opcode> <operand_b> <operator_a>
	| <special_opcode> <operator_a>
	
directive ::= <directive_type> <value_list>

label_directive ::= : <label>

literal ::= <integer> | <hex_integer>

operand ::= <generic_register>						; register
	| [ <generic_register> ]						; indirect register
	| [ <generic_register> + <static_value> ]		; indirect register + next-word
	| PEEK											; peek operator
	| PICK ( <static_value> )						; pick(n) operator
	| PICK <static_value>
	| SP											; sp system register
	| PC											; pc system register
	| EX											; ex system register
	| [ <static_value> ]							; indirect next-word literal
	| <static_value>								; next-word literal

operand_a ::= <operand>
	| # <static_value>								; literal value (-1 - 31)
	| POP											; pop operator

operand_b ::= <operand>
	| PUSH											; push operator
	
statement ::= <command> | <directive> | <label_directive>

static_value ::= <label_directive> | <literal>

value ::= <static_value> | <string>

value_list ::= <value> , <value_list> | <value>
```

License
======

Copyright(C) 2013 David Jolly <majestic53@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
