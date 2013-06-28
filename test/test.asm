; Assembler test for DCPU
; by Markus Persson

	set a, 0xbeef 					; 7c01 beef
	set [0x1000], a					; 03c1 1000
	ifn a, [0x1000] 				; 7813 1000
	set PC, end 					; 7f81 0020
	
	set i, 0 						; 84c1
:nextchar ife [data+i], 0 			; 86d2 0013
	set PC, end 					; 7f81 0020
	set [0x8000+i], [data+i] 		; 5ac1 0013 8000
	add i, 1 						; 88c2
	set PC, nextchar 				; 7f81 0009

:data dat "Hello world!", 0 		; 0048 0065 006c 006c 006f 0020 0077 006f 0072 006c 0064 0021 0000 

:end sub PC, 1 						; 8b83