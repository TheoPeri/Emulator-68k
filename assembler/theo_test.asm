			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		
			lea			$666,a0
			move.l		#$12345678,(a0)
			move.l		#$2,d1
			;lsl.l		#3,d0
			lsl.l		#$8,d1     
\loop
			dbra		d0,\loop
			illegal

toto
