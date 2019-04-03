			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		
			ori.b	#%01010,ccr
			andi.b	#%10101,ccr
