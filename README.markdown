DASM16
========

An assembler for DCPU-16 development, written in C++.

1. [Introduction](https://github.com/majestic53/dasm16#introduction)
3. [Changelog](https://github.com/majestic53/dasm16#changelog)
	* [Version 1.1](https://github.com/majestic53/dasm16#version-1.1)
2. [Usage](https://github.com/majestic53/dasm16#usage)
	* [Usage Example](https://github.com/majestic53/dasm16#usage-example)
3. [Architecture](https://github.com/majestic53/dasm16#architecture)
	* [Lexer](https://github.com/majestic53/dasm16#lexer)
	* [Parser](https://github.com/majestic53/dasm16#parser)
	* [Code Generator](https://github.com/majestic53/dasm16#code-generator)
4. [Syntax](https://github.com/majestic53/dasm16#syntax)
	* [Addressing Modes](https://github.com/majestic53/dasm16#addressing-modes)
	* [Commands](https://github.com/majestic53/dasm16#commands)
	* [Directives](https://github.com/majestic53/dasm16#directives)
	* [Registers](https://github.com/majestic53/dasm16#registers)
	* [Assembly BNF](https://github.com/majestic53/dasm16#assembly-bnf)
5. [Example](https://github.com/majestic53/dasm16#example)
6. [License](https://github.com/majestic53/dasm16#license)

Introduction
========

DASM16 is an 16-bit assembler built for development on the fictional DCPU-16 
processor from the game 0x10c. For more information on the DCPU-16 processor, see: 
http://0x10cwiki.com/wiki/DCPU-16.

Changelog
========

###Version 1.1
Updated: 7/5/2013

######Added extended directives:

* __INCBIN__: Embed a binary file inside the assembled binary

```asm
incbin "example.bin" ; file must be word aligned (length % 2 == 0)
```

* __INCLUDE__: Include another assembly file

```asm
include "example.asm"
```

* __RESERVE__: Reserve a contiguous block of words inside the assembled 
binary

```asm
reserve 0x10 ; reserves 16 words in the assembled binary, initialized to zero
```

######Bug fixes:
* Fixed a bug where the build path was not being set to the current location of 
the input file. As a result, all include directives required absolute paths, 
or ralative paths to the assembler.

Usage
========

__dasm16 [-h | -v] [-o OUTPUT] input...__

* __-h__: Display help menu
* __-v__: Enable verbose mode
* __-o__ OUTPUT: Specify output file path
* input: Specify input file path

###Usage Example

For the sake of this example, assume that we have an assembly file called 
__code.asm__. If we wish to compile this code into a binary file called __out.bin__
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

Architecture
========

DASM16 consists of three major pieces: Lexer, Parser, and Code Generator, which 
perform the basic steps required for assembly.

![Software stack](http://dl.dropboxusercontent.com/u/6410544/dasm16/stack.png)

###Lexer

The first step, breaks the the assembly file into descrete pieces, through a process known 
as _lexical analysis_. These descrete pieces are called tokens, and are used to check 
for code correctness in the next step.

![Token generation](http://dl.dropboxusercontent.com/u/6410544/dasm16/lexer.png)

###Parser

Once the code has been broken into tokens, the next step is to determine if the tokens are 
in the correct order. This is known as _syntactic analysis_, or checking that the code's
syntax is correct. During this step, the tokens are placed into tree structures, called _syntax trees_.
These trees give the tokens structure, as well as imply precedence.

![Syntax tree generation](http://dl.dropboxusercontent.com/u/6410544/dasm16/parser.png)

###Code Generator

The finaly step is to generate the binary code using the syntax trees generated earlier. To do this, we
must "walk over" the trees. During this process, we perform _symantic analysis_, or checking that the 
syntax trees make sense.

If the code is syntactically/semantically sound, the assembler will produce a file containing binary
representation of the assembly code.

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
-----------------------------------

SET | ADD | SUB | MUL | MLI | DIV | DVI | MOD | MDI
| AND | BOR | XOR | SHR | ASR | SHL | IFB | IFC 
| IFE | IFN | IFG | IFA | IFL | IFU | ADX | SBX
| STI | STD
```

######Special opcodes refer to opcodes with the format:

```
<opcode> <operator_a>
---------------------

JSR | INT | IAG | IAS | RFI | IAQ | HWN | HWQ
```

###Directives

The DASM16 supports a series of assembler directives with the format:

```
<directive> <value_list>
------------------------

DAT | INCBIN | INCLUDE | RESERVE
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

Example
========

Now, time for an example. We will take a look at the classic Hello World example, showing the 
entire process of compilation. The first step is to create a new file, calling it __HelloWorld.asm__.
This file will hold our assembly code, and will be used as an input file during 
compilation. 

Once you've created this file, copy and paste the code below.

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
window in the same directly. Next, use the command below to compile your code (making
sure that _dasm16.exe_ is also in the same directory).

```
dasm16 -v -o HelloWorld.bin HelloWorld.asm
```

You should see output in the console window similar to this:

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
as your assembly file. To run this binary file, fire up your favorite DCPU-16 emulator 
and have fun!

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
