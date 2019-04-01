			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		move.l	#5,d1
			sub.l	#6,d1

			illegal
