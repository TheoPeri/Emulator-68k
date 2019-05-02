					; ==============================
					; Definitions of Constants
					; ==============================

					; Video Memory
					; ------------------------------
VIDEO_START     	equ		$ffb500									; Starting address
VIDEO_WIDTH     	equ		480										; Width in pixels
VIDEO_HEIGHT    	equ		320										; Height in pixels
VIDEO_SIZE      	equ		(VIDEO_WIDTH*VIDEO_HEIGHT/8)			; Size in bytes
BYTE_PER_LINE		equ		(VIDEO_WIDTH/8)
VIDEO_BUFFER    	equ		(VIDEO_START-VIDEO_SIZE)				; Number of bytes per line

					;Bitmaps
					; ------------------------------

WIDTH            	equ	0
HEIGHT           	equ	2
MATRIX           	equ	4

					; Sprites
					; ------------------------------
; Sprites
					; ------------------------------
STATE               equ		0										; Current display state
X					equ		2										; Abscissa
Y                   equ		4										; Ordinate
BITMAP1             equ		6										; Bitmap 1
BITMAP2             equ		10										; Bitmap 2
HIDE                equ		0										; Hide the sprite
SHOW                equ		1										; Show the sprite
SIZE_OF_SPRITE      equ		14										; Size of a sprite in bytes

					; Invaders
					; ------------------------------
INVADER_PER_LINE 	equ		10
INVADER_PER_COLUMN  equ		5
INVADER_COUNT       equ		INVADER_PER_LINE*INVADER_PER_COLUMN
INVADER_STEP_X      equ		4
INVADER_STEP_Y      equ		8
INVADER_X_MIN       equ		0
INVADER_X_MAX       equ		(VIDEO_WIDTH-(INVADER_PER_LINE*32))
INVADER_SHOT_MAX    equ		5

					; End Game
					; ------------------------------
SHIP_WIN 			equ		42
SHIP_HIT  			equ		1
SHIP_COLLIDING      equ		2
INVADER_LOW      	equ		3

					; Keyboard
					; ------------------------------
SPACE_KEY       	equ		$420
LEFT_KEY        	equ		$46F
UP_KEY          	equ		$470
RIGHT_KEY       	equ		$471
DOWN_KEY        	equ		$472

					; Step increment in pixels
					; ------------------------------

SHIP_STEP       	equ		4										; Ship step increment
SHIP_SHOT_STEP  	equ		4
INVADER_SHOT_STEP   equ		1										; Invader shot step increment

					; ==============================
					; Vector Initialization
					; ==============================

					org	$0

vector_000  		dc.l	VIDEO_BUFFER							; Initial value of A7
vector_001  		dc.l	Main									; Initial value of the PC
					; ==============================
					; Main Program
					; ==============================
					org		$500

Main   				jsr		InitInvaders
					jsr		InitInvaderShots

\loop				jsr		PrintShip
					jsr		PrintShipShot
					jsr		PrintInvaders
					jsr		PrintInvaderShots
					jsr		BufferToScreen

					jsr		DestroyInvaders

					jsr		MoveShip
					jsr		MoveInvaders
					jsr		MoveShipShot
					jsr     MoveInvaderShots

					jsr		NewShipShot
					jsr     NewInvaderShot

					jsr		SpeedInvaderUp

					jsr     IsGameOver
					bne		\loop
					
					jsr		GameOver
					illegal


CopyLine			movem.l	d1-d4/a1,-(a7)
					subq.w	#1,d3

\loop				move.b	(a0)+,d1
					move.b	d1,d2

					lsr.b	d0,d1

					moveq.l	#8,d4
					sub.b	d0,d4
					lsl.b	d4,d2

					or.b	d1,(a1)+
					or.b	d2,(a1)
					dbra	d3,\loop

					movem.l	(a7)+,d1-d4/a1
					rts


PixelToByte			addq.w 	#7,d3									; alignement de 8 donc (x + 7) & ~(7)
					lsr.w	#3,d3
					rts


CopyBitmap			movem.l	d3/d4/a0/a1,-(a7)

					; Convert in Byte
					move.w	WIDTH(a0),d3
					jsr		PixelToByte

					;loop
					move.w	HEIGHT(a0),d4
					subq.w 	#1,d4

					lea		MATRIX(a0),a0

\loop				jsr		CopyLine
					adda.l	#BYTE_PER_LINE,a1
					dbra	d4,\loop

					movem.l	(a7)+,d3/d4/a0/a1
					rts


PixelToAddress		movem.w	d1/d2,-(a7)
					move.w	d1,d0
					lsr.w	#3,d1
					and.w	#7,d0

					mulu.w	#BYTE_PER_LINE,d2

					lea		VIDEO_BUFFER,a1
					adda.w	d1,a1
					adda.l	d2,a1

					movem.w	(a7)+,d1/d2
					rts


PrintBitmap			movem.l	d0/a1,-(a7)

					jsr		PixelToAddress
					jsr		CopyBitmap

					movem.l	(a7)+,d0/a1
					rts


PrintSprite			movem.l	d0-d2/a0,-(a7)

					move.w	STATE(a1),d0

					beq		\quit

					move.w  X(a1),d1
					move.w  Y(a1),d2
					movea.l	BITMAP1(a1),a0

					jsr		PrintBitmap

\quit				movem.l	(a7)+,d0-d2/a0
					rts


ClearScreen     	movem.l	d7/a0,-(a7)

					lea     VIDEO_START,a0

					move.w	#VIDEO_SIZE/4-1,d7
\loop         		move.l	#$0,(a0)+
					dbra	d7,\loop

					movem.l	(a7)+,d7/a0
					rts


BufferToScreen		movem.l	a0/a1/d7,-(a7)
					lea		VIDEO_BUFFER,a0
					lea		VIDEO_START,a1

					move.w	#VIDEO_SIZE/4-1,d7

\loop				move.l	(a0),(a1)+
					clr.l	(a0)+
					dbra	d7,\loop

					movem.l	(a7)+,a0/a1/d7
					rts


IsOutOfX			move.l	d1,-(a7)

					tst.w	d1
					bmi		\true

					add.w	WIDTH(A0),d1
					cmp.w	#VIDEO_WIDTH,d1
					bhi.w	\true

\false				move.l	(a7)+,d1
					andi.b	#%11111011,ccr
					rts


\true				move.l	(a7)+,d1
					ori.b	#%00000100,ccr
					rts


IsOutOfY			move.l	d2,-(a7)

					tst.w	d2
					bmi		\true

					add.w	HEIGHT(A0),d2
					cmp.w	#VIDEO_HEIGHT,d2
					bhi.w	\true

\false				move.l	(a7)+,d2
					andi.b	#%11111011,ccr
					rts


\true				move.l	(a7)+,d2
					ori.b	#%00000100,ccr
					rts


IsOutOfScreen       jsr     IsOutOfX
					beq		\quit
					jsr     IsOutOfY
\quit               rts


MoveSprite			movem.l	d1/d2/a0,-(a7)

					add.w	X(a1),d1
					add.w	Y(a1),d2

					movea.l	BITMAP1(a1),a0

					jsr		IsOutOfScreen
					beq 	\false

					move.w	d1,X(a1)
					move.w	d2,Y(a1)

\true				ori.b	#%00000100,ccr
					bra.s 	\quit

\false				andi.b	#%11111011,ccr
\quit				movem.l	(a7)+,d1/d2/a0
					rts


MoveSpriteKeyboard	movem.l	d1/d2,-(a7)

					clr.w	d1
					clr.w	d2

					tst.b	UP_KEY
					beq		\down
					subq.w	#1,d2

\down				tst.b	DOWN_KEY
					beq		\right
					addq.w	#1,d2


\right				tst.b	RIGHT_KEY
					beq		\left
					addq.w	#1,d1

\left				tst.b	LEFT_KEY
					beq		\exit
					subq.w	#1,d1

\exit				jsr		MoveSprite

					movem.l	(a7)+,d1/d2
					rts


GetRectangle		move.l	a0,-(a7)

					move.w 	X(a0),d1
					move.w	Y(a0),d2

					movea.l	BITMAP1(a0),a0

					move.w	d1,d3
					add.w	WIDTH(a0),d3
					subq.w	#1,d3

					move.w	d2,d4
					add.w	HEIGHT(a0),d4
					subq.w	#1,d4

					move.l	(a7)+,a0
					rts


IsSpriteColliding	movem.l	d1-d4/a0,-(a7)

					cmp.w	#SHOW,STATE(a1)
					bne		\quit
					cmp.w	#SHOW,STATE(a2)
					bne		\quit

					movea.l	a1,a0
					jsr     GetRectangle
					movem.w	d1-d4,-(a7)

					movea.l	a2,a0
					jsr     GetRectangle

					cmp.w	(a7),d3
					blt		\false

					cmp.w	2(a7),d4
					blt		\false

					cmp.w	4(a7),d1
					bgt		\false

					cmp.w	6(a7),d2
					bgt		\false

\true           	ori.b	#%00000100,ccr
					bra		\cleanStack

\false          	andi.b	#%11111011,ccr

\cleanStack     	adda.l	#8,a7

\quit				movem.l	(a7)+,d1-d4/a0
					rts


PrintShip			move.l	a1,-(a7)

					lea 	Ship,a1
					jsr		PrintSprite
					move.l	(a7)+,a1
					rts


MoveShip			movem.l	d1/d2/a1,-(a7)

					clr.w	d1
					clr.w	d2

\right				tst.b	RIGHT_KEY
					beq		\left
					add.w	#SHIP_STEP,d1

\left				tst.b	LEFT_KEY
					beq		\next
					sub.w	#SHIP_STEP,d1

\next				lea		Ship,a1
					jsr		MoveSprite

					movem.l	(a7)+,d1/d2/a1
					rts


PrintShipShot		move.l	a1,-(a7)

					lea 	ShipShot,a1
					jsr		PrintSprite
					move.l	(a7)+,a1
					rts


MoveShipShot    	movem.l	a1/d1/d2,-(a7)

					lea     ShipShot,a1

					cmp.w	#HIDE,STATE(a1)
					beq		\quit

					clr.w	d1
					move.w	#-SHIP_SHOT_STEP,d2
					jsr     MoveSprite
					beq		\quit

\outOfScreen    	move.w	#HIDE,STATE(a1)

\quit           	movem.l	(a7)+,a1/d1/d2
					rts


NewShipShot     	movem.l	d1-d3/a0/a1,-(a7)

					tst.b   SPACE_KEY
					beq		\quit

					lea     ShipShot,a0
					cmp.w	#SHOW,STATE(a0)
					beq		\quit

					lea     Ship,a1
					move.w  X(a1),X(a0)
					move.w  Y(a1),Y(a0)

					movea.l BITMAP1(a1),a1
					move.w  WIDTH(a1),d1

					movea.l BITMAP1(a0),a1
					move.w  HEIGHT(a1),d2
					move.w  WIDTH(a1),d3

					sub.w	d3,d1
					lsr.w	#1,d1
					add.w	d1,X(a0)

					sub.w	d2,Y(a0)
					move.w	#SHOW,STATE(a0)

\quit				movem.l	(a7)+,d1-d3/a0/a1
					rts


InitInvaderLine		movem.l	d0-d2/a0,-(a7)

					; get padding
					move.w	#32,d0
					sub.w	WIDTH(a1),d0
					lsr.w	#1,d0
					add.w	d0,d1

					; loop
					move.w	#INVADER_PER_LINE-1,d0

\loop				move.w	#SHOW,STATE(a0) ; SHOW = 1
					move.w	d1,X(a0)
					move.w	d2,Y(a0)
					move.l	a1,BITMAP1(a0)
					move.l	a2,BITMAP2(a0)

					addi.w 	#32,d1
					adda.l	#SIZE_OF_SPRITE,a0

					dbra	d0,\loop

					; exit
					movem.l	(a7)+,d0-d2/a0
					rts


PrintInvaders		movem.l	d7/a1,-(a7)

					; init
					move.w	#INVADER_COUNT-1,d7
					lea		Invaders,a1


\loop				jsr		PrintSprite
					adda.l	#SIZE_OF_SPRITE,a1

					dbra	d7,\loop

					;quit
					movem.l	(a7)+,d7/a1
					rts


InitInvaders		movem.l	d1/d2/a0-a2,-(a7)

					move.w	InvaderX,d1
					move.w	InvaderY,d2

					lea		Invaders,a0
					lea		0,a2

					; line 0
					lea		InvaderC1_Bitmap,a1
					lea		InvaderC2_Bitmap,a2
					jsr		InitInvaderLine
					addi.w	#32,d2
					adda.l	#SIZE_OF_SPRITE*INVADER_PER_LINE,a0

					; line 1
					lea		InvaderB1_Bitmap,a1
					lea		InvaderB2_Bitmap,a2
					jsr		InitInvaderLine
					addi.w	#32,d2
					adda.l	#SIZE_OF_SPRITE*INVADER_PER_LINE,a0

					; line 2
					jsr		InitInvaderLine
					addi.w	#32,d2
					adda.l	#SIZE_OF_SPRITE*INVADER_PER_LINE,a0

					; line 3
					lea		InvaderA1_Bitmap,a1
					lea		InvaderA2_Bitmap,a2
					jsr		InitInvaderLine
					addi.w	#32,d2
					adda.l	#SIZE_OF_SPRITE*INVADER_PER_LINE,a0

					; line 4
					jsr		InitInvaderLine

					movem.l	(a7)+,a0-a2/d1/d2
					rts


GetInvadersStep		move.w	d0,-(a7)
					; add
					move.w	InvaderX,d0
					add.w	InvaderCurrentStep,d0	; step

					; test direction
					cmpi.w	#INVADER_X_MIN,d0
					blo		\change_direction

					cmpi.w	#INVADER_X_MAX,d0
					bhi		\change_direction

					; else
					move.w	InvaderCurrentStep,d1
					clr.w	d2

					move.w	d0,InvaderX
					bra.s	\quit

\change_direction	clr.w	d1
					move.w	#INVADER_STEP_Y,d2

					add.w	d2,InvaderY
					neg.w	InvaderCurrentStep	; inversion

\quit				move.w (a7)+,d0
					rts


MoveAllInvaders		movem.l	d1/d2/a1/d7,-(a7)

					jsr		GetInvadersStep

					; init
					move.w	#INVADER_COUNT-1,d7
					lea		Invaders,a1

					; loop
\loop				tst.w	STATE(a1)
					beq		\continue
					; add
					jsr MoveSprite
					jsr SwapBitmap

					; incrementation
\continue			adda.l	#SIZE_OF_SPRITE,a1
					dbra	d7,\loop

					;quit
					movem.l	(a7)+,d1/d2/a1/d7
					rts


MoveInvaders		subq.w	#1,\count
					bne		\quit

					jsr		MoveAllInvaders
					move.w	InvaderSpeed,\count
\quit				rts

\count				dc.w	1


SwapBitmap			move.l 	d0,-(a7)

					move.l	BITMAP1(a1),d0
					move.l	BITMAP2(a1),BITMAP1(a1)
					move.l	d0,BITMAP2(a1)

					move.l	(a7)+,d0
					rts


SpeedInvaderUp		movem.w	d0/a0,-(a7)

					clr.w	InvaderSpeed
					move.w	InvaderCount,d0		;counter
					lea		SpeedLevels,a0

\loop				addq.w	#1,InvaderSpeed

					cmp.w	(a0)+,d0
					bhi.w	\loop

\exit				movem.w	(a7)+,d0/a0
					rts


DestroyInvaders		movem.l	d7/a1/a2,-(a7)

					movea.l	#ShipShot,a1		;sprite shoot
					movea.l	#Invaders,a2		;sprite ship
					move.w	#INVADER_COUNT-1,d7	;i

\loop				jsr 	IsSpriteColliding
					beq		\collide

					adda.l	#SIZE_OF_SPRITE,a2	;increment
					dbra.w	d7,\loop

\exit				movem.l	(a7)+,d7/a1/a2
					rts


\collide			move.w	#HIDE,STATE(a1)
					move.w	#HIDE,STATE(a2)
					subq.w	#1,InvaderCount
					bra.s	\exit

InitInvaderShots	movem.l	d7/a0,-(a7)

					lea		InvaderShots,a0
					move.w	#INVADER_SHOT_MAX-1,d7

\loop				;set
					move.w	#HIDE,STATE(a0)
					move.l	#InvaderShot1_Bitmap,BITMAP1(a0)
					move.l	#InvaderShot2_Bitmap,BITMAP2(a0)

					adda.w	#SIZE_OF_SPRITE,a0
					dbra.w	d7,\loop

					movem.l	(a7)+,a0/d7
					rts


GetHiddenShot		move.l	d7,-(a7)

					lea		InvaderShots,a0
					move.w	#INVADER_SHOT_MAX-1,d7

\loop				cmpi.w	#HIDE,STATE(a0)
					beq		\true

					adda.w	#SIZE_OF_SPRITE,a0
					dbra.w	d7,\loop


\false				move.l	(a7)+,d7
					andi.b	#%11111011,ccr
					rts

\true				move.l	(a7)+,d7
					ori.b	#%00000100,ccr
					rts


ConnectInvaderShot	movem.l	d1-d3/a0/a1,-(a7)

					cmpi.w	#SHOW,STATE(a1)
					bne		\exit

					jsr		GetHiddenShot
					bne		\exit

					move.w	X(a1),X(a0)
					move.w	Y(a1),Y(a0)

					movea.l	BITMAP1(a1),a1
					move.w  WIDTH(a1),d1
					move.w  HEIGHT(a1),d2

					movea.l BITMAP1(a0),a1
					move.w  WIDTH(a1),d3

					sub.w	d3,d1
					lsr.w	#1,d1

					add.w	d1,X(a0)
					sub.w	d2,Y(a0)

					move.w	#SHOW,STATE(a0)

\exit				movem.l	(a7)+,d1-d3/a0/a1
					rts


Random              move.l	\old,d0 33
					muls.w	#16807,d0
					and.l	#$7fffffff,d0
					move.l	d0,\old
					lsr.l	#4,d0
					and.l	#$7ff,d0
					rts

\old                dc.l	425625


NewInvaderShot		movem.l	d0/a1,-(a7)
					jsr 	Random

					cmpi.w	#INVADER_COUNT,d0
					bhs		\quit

					mulu.w	#SIZE_OF_SPRITE,d0
					movea.l	#Invaders,a1
					adda.l	d0,a1

					jsr 	ConnectInvaderShot

\quit				movem.l	(a7)+,d0/a1
					rts


PrintInvaderShots	movem.l	d7/a1,-(a7)

					; init
					move.w	#INVADER_SHOT_MAX-1,d7
					lea		InvaderShots,a1

\loop				jsr		PrintSprite
					adda.l	#SIZE_OF_SPRITE,a1

					dbra	d7,\loop

					;quit
					movem.l	(a7)+,d7/a1
					rts


SwapInvaderShots    ; Check swap?
					subq.w	#1,\count
					bne		\quit
					
					move.w	#6,\count

					; Save registers on the stack.
					movem.l	d7/a1,-(a7)

					; Number of iterations = Number of invader shots.
					; Number of iterations - 1 -> D7.W (DBRA)
					move.w #INVADER_SHOT_MAX-1,d7

					; Address of the invader shots -> A1.L
					lea     InvaderShots,a1

\loop               ; Switch between bitmaps 1 and 2 for all the shots.
					jsr     SwapBitmap
					adda.l	#SIZE_OF_SPRITE,a1
					dbra	d7,\loop

					move.w	InvaderShootUpdate,\count

					; Restore registers from the stack and return from subroutine.
					movem.l	(a7)+,d7/a1
\quit				rts

\count				dc.w	6


MoveInvaderShots	movem.l	a1/d1/d2/d7,-(a7)

					move.w	#INVADER_SHOT_MAX-1,d7
					lea		InvaderShots,a1

\loop				cmp.w	#HIDE,STATE(a1)
					beq		\continue

					clr.w	d1
					move.w	#SHIP_SHOT_STEP,d2
					jsr     MoveSprite
					beq		\continue

\outOfScreen    	move.w	#HIDE,STATE(a1)

\continue			adda.l	#SIZE_OF_SPRITE,a1
					dbra	d7,\loop

					jsr     SwapInvaderShots

\quit           	movem.l	(a7)+,a1/d1/d2/d7
					rts


IsShipHit			movem.l	d7/a1/a2,-(a7)

					; set ship param
					lea		Ship,a2

					move.w	#INVADER_SHOT_MAX-1,d7
					lea		InvaderShots,a1

\loop				jsr		IsSpriteColliding
					beq		\true

					adda.l	#SIZE_OF_SPRITE,a1

					dbra	d7,\loop

\false				andi.b	#%11111011,ccr
					bra.s	\quit

\true				ori.b	#%00000100,ccr
					
\quit				movem.l	(a7)+,d7/a1/a2
					rts


IsShipColliding		movem.l	d7/a1/a2,-(a7)

					; set ship param
					lea		Ship,a2

					move.w	#INVADER_COUNT-1,d7
					lea		Invaders,a1

\loop				jsr		IsSpriteColliding
					beq		\true

					adda.l	#SIZE_OF_SPRITE,a1

					dbra	d7,\loop

\false				andi.b	#%11111011,ccr
					bra.s	\quit

\true				ori.b	#%00000100,ccr
					
\quit				movem.l	(a7)+,d7/a1/a2
					rts


IsInvaderTooLow		movem.l	d7/a1/a2,-(a7)

					move.w	#INVADER_COUNT-1,d7
					lea		Invaders,a1

\loop				cmpi.w	#280,Y(a1)
					bhi		\true

					adda.l	#SIZE_OF_SPRITE,a1

					dbra	d7,\loop

\false				andi.b	#%11111011,ccr
					bra.s	\quit

\true				ori.b	#%00000100,ccr
					
\quit				movem.l	(a7)+,d7/a1/a2
					rts


IsGameOver			jsr		IsShipHit
					bne		\ShipColliding
					move.l	#SHIP_HIT,d0
					bra.s	\true
					
\ShipColliding		jsr		IsShipColliding
					bne		\InvaderTooLow
					move.l	#SHIP_COLLIDING,d0
					bra.s	\true
					
\InvaderTooLow		jsr		IsInvaderTooLow
					bne		\Count
					move.l	#INVADER_LOW,d0
					bra.s	\true

\Count				tst.w	InvaderCount
					bne		\false
					move.l	#SHIP_WIN,d0
				
\true				ori.b	#%00000100,ccr
					rts	
		
\false				andi.b	#%11111011,ccr
					rts

Print           	; Save registers on the stack.
					movem.l	d0/d1/a0,-(a7)
					
\loop           	; Load a character of the string into D0.
					; If the character is null, it is the end of the string.
					; We can exit the subroutine.
					move.b	(a0)+,d0
					beq  	\quit
					
					; Display the character.
					jsr     PrintChar
					
					; Increment the column where the next character will be displayed
					; and branch to \loop.
					addq.b	#1,d1
					bra  	\loop

\quit           	; Restore registers from the stack and return from subroutine.
					movem.l		(a7)+,d0/d1/a0
					rts

GameOver			movem.l	d0-d2/a0,-(a7)

					cmpi.l	#SHIP_WIN,d0
					bne		\ship_hit
					movea.l	#string_ship_win,a0
					bra.s 	\next
					
\ship_hit			cmpi.l	#SHIP_HIT,d0
					bne		\ship_collide
					movea.l	#string_ship_hit,a0
					bra.s 	\next
					
\ship_collide		cmpi.l	#SHIP_COLLIDING,d0
					bne		\invader_low
					movea.l	#string_ship_collide,a0
					bra.s 	\next

\invader_low		movea.l	#string_invaders_low,a0

\next				move.l	#14,d1
					move.l	#38,d2
					jsr		Print
					
					movem.l	(a7)+,d0-d2/a0
					rts
					
; ==============================
; Data
; ==============================
string_ship_win 	dc.b	"========You survived!!!========",0
string_ship_hit  	dc.b	"========They hit you!!!========",0
string_ship_collide dc.b	"=============(BOOM)============",0
string_invaders_low dc.b	"=====Invasion in coming!!!=====",0

Invaders            ds.b    INVADER_COUNT*SIZE_OF_SPRITE
InvaderShots        ds.b    SIZE_OF_SPRITE*INVADER_SHOT_MAX
InvaderX            dc.w	(VIDEO_WIDTH-(INVADER_PER_LINE*32))/2	; Global abscissa
InvaderY            dc.w	32										; Global ordinate
InvaderCurrentStep  dc.w	INVADER_STEP_X
InvaderCount        dc.w	INVADER_COUNT
InvaderSpeed        dc.w	8
InvaderShootUpdate  dc.w	6
SpeedLevels         dc.w	1,5,10,15,20,25,35,50

Ship                dc.w    SHOW
					dc.w	(VIDEO_WIDTH-24)/2,VIDEO_HEIGHT-32
					dc.l    Ship_Bitmap
					dc.l	0

ShipShot            dc.w    HIDE
					dc.w	0,0
					dc.l    ShipShot_Bitmap
					dc.l	0

InvaderA1_Bitmap 	dc.w	24,16									; Width, Height
					dc.b    %00000000,%11111111,%00000000 			; Dot matrix
					dc.b    %00000000,%11111111,%00000000
					dc.b    %00111111,%11111111,%11111100
					dc.b    %00111111,%11111111,%11111100
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111100,%00111100,%00111111
					dc.b    %11111100,%00111100,%00111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %00000011,%11000011,%11000000
					dc.b    %00000011,%11000011,%11000000
					dc.b    %00001111,%00111100,%11110000
					dc.b    %00001111,%00111100,%11110000
					dc.b    %11110000,%00000000,%00001111
					dc.b    %11110000,%00000000,%00001111

InvaderA2_Bitmap    dc.w	24,16
					dc.b    %00000000,%11111111,%00000000
					dc.b    %00000000,%11111111,%00000000
					dc.b    %00111111,%11111111,%11111100
					dc.b    %00111111,%11111111,%11111100
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111100,%00111100,%00111111
					dc.b    %11111100,%00111100,%00111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %00001111,%11000011,%11110000
					dc.b    %00001111,%11000011,%11110000
					dc.b    %00111100,%00111100,%00111100
					dc.b    %00111100,%00111100,%00111100
					dc.b    %00001111,%00000000,%11110000
					dc.b    %00001111,%00000000,%11110000

InvaderB1_Bitmap 	dc.w	22,16
					dc.b	%00001100,%00000000,%11000000
					dc.b	%00001100,%00000000,%11000000
					dc.b	%00000011,%00000011,%00000000
					dc.b	%00000011,%00000011,%00000000
					dc.b	%00001111,%11111111,%11000000
					dc.b	%00001111,%11111111,%11000000
					dc.b    %00001100,%11111100,%11000000
					dc.b    %00001100,%11111100,%11000000
					dc.b    %00111111,%11111111,%11110000
					dc.b    %00111111,%11111111,%11110000
					dc.b    %11001111,%11111111,%11001100
					dc.b    %11001111,%11111111,%11001100
					dc.b    %11001100,%00000000,%11001100
					dc.b    %11001100,%00000000,%11001100
					dc.b    %00000011,%11001111,%00000000
					dc.b    %00000011,%11001111,%00000000

InvaderB2_Bitmap    dc.w	22,16
					dc.b    %00001100,%00000000,%11000000
					dc.b    %00001100,%00000000,%11000000
					dc.b    %00000011,%00000011,%00000000
					dc.b    %00000011,%00000011,%00000000
					dc.b    %11001111,%11111111,%11001100
					dc.b    %11001111,%11111111,%11001100
					dc.b    %11001100,%11111100,%11001100
					dc.b    %11001100,%11111100,%11001100
					dc.b    %00111111,%11111111,%11110000
					dc.b    %00111111,%11111111,%11110000
					dc.b    %00001111,%11111111,%11000000
					dc.b    %00001111,%11111111,%11000000
					dc.b    %00001100,%00000000,%11000000
					dc.b    %00001100,%00000000,%11000000
					dc.b    %00110000,%00000000,%00110000
					dc.b    %00110000,%00000000,%00110000

InvaderC1_Bitmap 	dc.w	16,16
					dc.b    %00000011,%11000000
					dc.b    %00000011,%11000000
					dc.b    %00001111,%11110000
					dc.b    %00001111,%11110000
					dc.b    %00111111,%11111100
					dc.b    %00111111,%11111100
					dc.b    %11110011,%11001111
					dc.b    %11110011,%11001111
					dc.b    %11111111,%11111111
					dc.b    %11111111,%11111111
					dc.b    %00110011,%11001100
					dc.b    %00110011,%11001100
					dc.b    %11000000,%00000011
					dc.b    %11000000,%00000011
					dc.b    %00110000,%00001100
					dc.b    %00110000,%00001100

InvaderC2_Bitmap    dc.w	16,16
					dc.b    %00000011,%11000000
					dc.b    %00000011,%11000000
					dc.b    %00001111,%11110000
					dc.b    %00001111,%11110000
					dc.b    %00111111,%11111100
					dc.b    %00111111,%11111100
					dc.b    %11110011,%11001111
					dc.b    %11110011,%11001111
					dc.b    %11111111,%11111111
					dc.b    %11111111,%11111111
					dc.b    %00001100,%00110000
					dc.b    %00001100,%00110000
					dc.b    %00110011,%11001100
					dc.b    %00110011,%11001100
					dc.b    %11001100,%00110011
					dc.b    %11001100,%00110011

Ship_Bitmap      	dc.w	24,14
					dc.b    %00000000,%00011000,%00000000
					dc.b    %00000000,%00011000,%00000000
					dc.b    %00000000,%01111110,%00000000
					dc.b    %00000000,%01111110,%00000000
					dc.b    %00000000,%01111110,%00000000
					dc.b    %00000000,%01111110,%00000000
					dc.b    %00111111,%11111111,%11111100
					dc.b    %00111111,%11111111,%11111100
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111
					dc.b    %11111111,%11111111,%11111111

ShipShot_Bitmap 	dc.w	2,6
					dc.b	%11000000
					dc.b	%11000000
					dc.b	%11000000
					dc.b	%11000000
					dc.b	%11000000
					dc.b	%11000000

InvaderShot1_Bitmap dc.w	4,6
					dc.b    %11000000
					dc.b    %11000000
					dc.b    %00110000
					dc.b    %00110000
					dc.b    %11000000
					dc.b    %11000000

InvaderShot2_Bitmap dc.w	4,6
					dc.b    %00110000
					dc.b    %00110000
					dc.b    %11000000
					dc.b    %11000000
					dc.b    %00110000
					dc.b    %00110000
					
PrintChar           incbin	"PrintChar.bin"
