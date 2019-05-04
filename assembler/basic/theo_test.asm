q			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		move.l  #$ff,d0
			move.l	#$2,d1
			
			lsl.l	d0,d1
			
