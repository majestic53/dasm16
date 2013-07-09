DASM16
========

DASM16 is an 16-bit assembler built for development on the fictional DCPU-16 
processor from the game 0x10c, and is fully compliant with the DCPU-16 1.7 
specification as supplied by [Notch](http://dcpu.com/dcpu-16/).

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
* The build path was not being set to the location of the input file. 
As a result, all include directives required absolute paths, 
or ralative paths from the assembler.

Table of Contents
========

1. [Usage](https://github.com/majestic53/dasm16#usage)
	* [Usage Example](https://github.com/majestic53/dasm16#usage-example)
2. [Architecture](https://github.com/majestic53/dasm16#architecture)
	* [Lexer](https://github.com/majestic53/dasm16#lexer)
	* [Parser](https://github.com/majestic53/dasm16#parser)
	* [Code Generator](https://github.com/majestic53/dasm16#code-generator)
3. [Syntax](https://github.com/majestic53/dasm16#syntax)
	* [Addressing Modes](https://github.com/majestic53/dasm16#addressing-modes)
	* [Commands](https://github.com/majestic53/dasm16#commands)
	* [Directives](https://github.com/majestic53/dasm16#directives)
	* [Registers](https://github.com/majestic53/dasm16#registers)
	* [Assembly BNF](https://github.com/majestic53/dasm16#assembly-bnf)
4. [Example](https://github.com/majestic53/dasm16#example)
5. [License](https://github.com/majestic53/dasm16#license)

Usage
========

DASM16 is a command-line tool with a very basic interface. To use DAMS16, follow the
usage statement:

```
dasm16 [-h | -v] [-o OUTPUT] input...
```

* __-h__: Display help menu
* __-v__: Enable verbose mode
* __-o__ OUTPUT: Specify output file path
* input: Specify input file path

###Usage Example

Assume that we have an assembly file called __code.asm__. If we wish to compile 
this code into a binary file called __out.bin__ with verbose output, we would 
use the command:

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

The first step breaks the the assembly file into descrete pieces, through a process known 
as _lexical analysis_. These descrete pieces are called tokens, and are used to check 
for code correctness in the next step.

![Token generation](http://dl.dropboxusercontent.com/u/6410544/dasm16/lexer.png)

###Parser

Once the code has been broken into tokens, the token ordering is used to determine if the code is 
syntactically correct. This is known as _syntactic analysis_. During this step the tokens are 
placed into tree structures, called _syntax trees_, which form statements. These trees give the tokens 
structure, as well as imply precedence.

![Syntax tree generation](http://dl.dropboxusercontent.com/u/6410544/dasm16/parser.png)

###Code Generator

The final step involves generating the binary code using the statements (syntax trees) generated earlier. 
To do this, the statements are evaluated. During this process, _symantic analysis_ is performed, which makes 
sure the statements make sense.

If the code is syntactically and semantically correct, the assembler produces a file containing binary
representation of the assembly code.

Syntax
========

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

We will take a look at the classic Hello World example, showing the 
entire process of compilation. 

###Steps:

* Create a new file, called __HelloWorld.asm__ (or something...). This file will hold our assembly code, 
and will be used as the input file during compilation. 

* Copy-and-paste the code below into the file:

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

* Save the file and open a console window in the same directly.

* Use the command below to compile your code (making sure that _dasm16.exe_ 
is also in the same directory):

```
dasm16 -v -o HelloWorld.bin HelloWorld.asm
```

* The output that appears in the console window should be similar to this:

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

Thats it! _HelloWorld.bin_ can be found in the same directory
as the assembly file.

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
