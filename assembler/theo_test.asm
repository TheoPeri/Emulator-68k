			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		
			
			;move.l		#$8,d0
			
			move.l		#$2,d1	
			muls.w		#-5,d1     
;~ \loop
			;~ dbra		d0,\loop
			;~ illegal

;~ toto
