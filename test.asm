				org 		$4
				
Vector_001		dc.l		Main
				
				org			$500
Main			move.l		#$8000,a0
				move.l		#$1,d1
				cmp.b		#-1,d1
