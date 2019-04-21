q			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		move.l	#$fff,d1
			move.l	#5,d2
			mulu	d2,d1
