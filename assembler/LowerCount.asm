			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500

Main		movea.l #STRING,a0
			bsr LowerCount
			illegal

LowerCount  movem.l d1/a0,-(a7)
            moveq.l #0,d0
            
\loop       move.b  (a0)+,d1
            beq     \quit
            
            cmp.b   #'a',d1
            blo     \loop

            cmp.b   #'z',d1
            bhi     \loop
            
            addq.l  #1,d0
            bra     \loop
            
\quit       movem.l	(a7)+,d1/a0
            rts
            
STRING      dc.b    "Cette chAINE A 10 CARAcTErE En MINiSCULE.",0
