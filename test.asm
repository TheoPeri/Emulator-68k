				org 		$4
				
Vector_001		dc.l		Main
				
				org			$500
Main			move.l		#2,d0
				move.l		#1,d1
				cmp.l		d0,d1
				bsr.b		$540
