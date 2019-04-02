			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		move.l	#$10040,d0

\loop		addq.l	#1,d1
			dbra	d0,\loop

			illegal
