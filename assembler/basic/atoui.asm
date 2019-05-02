			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
			
Main		lea String,a0
			jsr		Atoui
			illegal

Atoui       ; Save registers on the stack.
			movem.l	d1/a0,-(a7)
			
			; Initialize the output variable to 0.
			clr.l	d0
			
			; Initialize the conversion variable to 0.
			clr.l	d1
			
\loop       ; Copy the current character into D1.
			; Then A0 points to the next character (postincrement mode).
			move.b	(a0)+,d1
			
			; If the copied character is null,; branch to \quit (end of string).
			beq  	\quit
			
			; Otherwise, the character is converted into an integer.
			subi.b	#'0',d1
			
			; Shift the output variable to the left (x10),; and add the integer value.
			muls.w	#10,d0
			add.l	d1,d0
			
			; Next character.
			bra  	\loop
			
\quit       ; Restore registers from the stack and return from subroutine.
			movem.l	(a7)+,d1/a0
			rts

String		dc.b		"1280",0
