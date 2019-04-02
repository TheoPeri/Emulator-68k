			org 	$0
Vector_000	dc.l	$10000
Vector_001	dc.l	Main

			org		$500
Main		move.w		#$ffff,$666
			clr.w		$666
			illegal
