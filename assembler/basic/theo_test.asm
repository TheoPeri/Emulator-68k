			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		move.l	#-4,d1
			neg.l	d1

			illegal
			
