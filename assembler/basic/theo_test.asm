			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		andi.l	#$FF,d0
			ori.b	#$FF,d0

			illegal
			
			lsl.l	d0,d1
			
