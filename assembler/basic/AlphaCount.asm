			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		movea.l	#String1,a0
			jsr     AlphaCount
			illegal

LowerCount  movem.l	d1/a0,-(a7)
			clr.l	d0

\loop       move.b	(a0)+,d1
			beq		\quit
			
			cmp.b	#'a',d1
			blo		\loop
			
			cmp.b	#'z',d1
			bhi		\loop
			
			addq.l	#1,d0
			bra		\loop
			
\quit 		movem.l	(a7)+,d1/a0
			rts

UpperCount  movem.l	d1/a0,-(a7)
			clr.l	d0
			
\loop       move.b	(a0)+,d1
			beq		\quit
			
			cmp.b	#'A',d1
			blo		\loop
			
			cmp.b	#'Z',d1
			bhi		\loop
			
			addq.l	#1,d0
			bra		\loop
			
\quit 		movem.l	(a7)+,d1/a0
			rts

DigitCount  movem.l	d1/a0,-(a7)
			clr.l	d0
			
\loop       move.b	(a0)+,d1
			beq		\quit
			
			cmp.b	#'0',d1
			blo		\loop
			
			cmp.b	#'9',d1
			bhi		\loop
			
			addq.l	#1,d0
			bra		\loop
			
\quit 		movem.l	(a7)+,d1/a0
			rts

AlphaCount	; Count the number of small letters
			; and push it onto the stack.
			jsr		LowerCount
			move.l	d0,-(a7)
			
			; Count the number of capital letters and add it
			; to the top of stack (without popping off).
			; Top of stack = Small letters + Capital letters
			jsr     UpperCount
			add.l   d0,(a7)
			
			; Count the number of digits.
			; The top of stack (Small letters + Capital letters)
			; is added to the number of digits (D0).
			; The sum is loaded into D0.
			; D0 = Small letters + Capital letters + Digits
			; The top of stack is poppep off (postincrement mode).
			jsr     DigitCount
			add.l   (a7)+,d0
			
			; Return from subroutine.
			rts

String1     dc.b	"This string contains 42 alphanumeric characters.",0
