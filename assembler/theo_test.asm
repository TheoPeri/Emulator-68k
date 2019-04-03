			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		
			
			;move.l		#$8,d0
			
			move.l		#$-8,d1
			move.l		#$-6,d2			
			muls		d2,d1     
;~ \loop
			;~ dbra		d0,\loop
			;~ illegal

;~ toto
