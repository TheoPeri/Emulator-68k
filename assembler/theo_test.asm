q			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		lea.l	$678,a0
			move.l	#$1234,4(a0)

			lea.l	$4(a0),a1
			
