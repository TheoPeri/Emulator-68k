			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		move.l	#4589,d0
			move.l	#$ffffffff,$1112
			or.l	d0,$1112
