				org 		$4
				
Vector_001		dc.l		Main
				
				org			$500
Main			move.l		#$7000,a0
				move.l		#$8000,a1
				move.b		#-128,(a0)
				move.b		#-1,(a1)
				move.b		d2,d1
