			; ==============================
			;Vector Initialization
			; ==============================
			org		$0
Vector_000	dc.l	$10000
vector_001  dc.l    Main
			; ==============================
			; Main Program
			; ==============================
			org		$500

Main        movea.l	#String1,a0
			jsr     StrLen
			movea.l	#String2,a0
			jsr     StrLen
			illegal
			
			; ==============================
			; Subroutines
			; ==============================
StrLen      move.l	a0,-(a7)       ; Save A0 on the stack.
			clr.l	d0      
\loop 		tst.b	(a0)+
			beq		\quit
			
			addq.l	#1,d0
			bra		\loop

\quit       movea.l	(a7)+,a0       ; Restore AO from the stack.
			rts

; ==============================
; Data
; ==============================
String1     dc.b	"This string is made up of 40 characters.",0
String2     dc.b	"This one is made up of 26.",0
