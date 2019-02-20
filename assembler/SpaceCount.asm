			org 	$4

Vector_001	dc.l	Main

			org		$500
			
Main		movea.l #STRING,a0
			moveq.l #0,d0
			bra MinCount
			
MinCount	cmpi.b	#0,(a0)
			beq		EXIT
			cmpi.b	#' ',(a0)+
			bne		MinCount
			addq	#1,d0
			bra		MinCount

EXIT		illegal
STRING      dc.b    "Cette chaine comporte 4 espaces.",0
