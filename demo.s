
; test program for 8 bitplane startup code with ADPCM music
; compiles with PhxAss at least

	MACHINE 68020

	include "init.i"
	include "init-8bpl.i"
	include "adpcm/adpcm-player.i"

        public _Start;, _newPalette, _updatePaletteFlag

	section	code,code

_Start
	bsr initC2P8bpl ; set parameters for chunky to planar routine
	bsr init ; switch off system and set custom copperlist etc
	move.l #copperlist8bpl,$dff080	; Set our copperlist

	lea soundtrack,a0
	bsr initMusic

    lea pal_feedyourhead,a0       ; set initial palette
    ; lea back3dpal,a0       ; set initial palette
	move.l a0,newPalette    ; save address of palette in #newPalette
	move.w #1,updatePaletteFlag ; set flag to update palette in next vblank int

	lea screen,a0
    bsr _init_feedyourhead     ; a0 screen  

.mainloop
	
    ;bsr drawBackground
    bsr drawFeedYourHead

	lea screen,a0
	bsr flipScreen8bpl

	tst.w exitflag

	beq .mainloop

    bsr _deinit_feedyourhead
	bsr stopMusic
	bsr deinit

	rts

; This is how we call c functions :o
drawFeedYourHead
        lea screen,a0
        ;lea fire, a1
        move.l sync,d0
        bsr _tick_feedyourhead
        rts

drawBackground
        lea screen,a0
        lea back3d,a1
        bsr _drawback
        rts

drawTestScreen
	; draws a scrolling texture to chunky screen

	lea screen,a0
	lea texture,a1
	adda.l #6+256*4,a1 ; skip .uc header and palette

	move.l sync,d2 ; sync is our global timer incremented in vblank interrupt 50 times a second
	move.l #176-1,d0
.yloop

	movea.l a1,a2
	adda.l d2,a2

	move.l #(320/8)-1,d1

.xloop
		rept 8
			move.b (a2)+,(a0)+
			addq.l #1,a2
		endr
	dbra d1,.xloop

	adda.l #256,a1

	dbra d0,.yloop

	rts


	section data,bss

screen
	blk.b 320*176,0

fire
	blk.b 320*64,0

	section data,data

texture
	incbin "data/texture.uc"
soundtrack
	; incbin "data/igen.wav"          ; stereo, 22050 Hz
	incbin "data/thulenebula.wav" ; mono, 22050 Hz

back3d
        incbin "data/back_3d.chunky"
back3dpal
        incbin "data/back_3d.pal"

pal_feedyourhead
        incbin "data/feedyourhead.pal"
