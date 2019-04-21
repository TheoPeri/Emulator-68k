q			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		move.l	#$12345678,d1
			andi.b	#33,d1
			move.l	#$12345678,d1
			andi.w	#$6699,d1
			move.l	#$12345678,d1
			andi.l	#$f0ff0f,d1
			
