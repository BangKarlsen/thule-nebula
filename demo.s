
; test program for 8 bitplane startup code with ADPCM music
; compiles with PhxAss at least

	MACHINE 68020

	include "init.i"
	include "init-8bpl.i"
	include "adpcm/adpcm-player.i"

    public _Start;

	section	code,code

_Start
	bsr initC2P8bpl ; set parameters for chunky to planar routine
	bsr init ; switch off system and set custom copperlist etc
	move.l #copperlist8bpl,$dff080	; Set our copperlist

	lea soundtrack,a0
	bsr initMusic

    ;lea pal_back3d,a0       ; set initial palette
    lea pal_feedyourhead,a0       ; set initial palette
	move.l a0,newPalette    ; save address of palette in #newPalette
	move.w #1,updatePaletteFlag ; set flag to update palette in next vblank int

	lea screen,a0
    ;bsr _init_feedyourhead     ; a0 screen  
    bsr _init_3d

.mainloop
	
    lea screen,a0
    move.l sync,d0
    bsr _maintick

	lea screen,a0
	bsr flipScreen8bpl

	tst.w exitflag

	beq .mainloop

    ;bsr _deinit_feedyourhead
    bsr _deinit_3d
	bsr stopMusic
	bsr deinit

	rts

;_drawBackground
;    clr.w $102
;	tst.w pal_back3d_is_set
;	bne .pal_back_set
;	move.w #1,pal_back3d_is_set
;	lea pal_back3d,a0
;	move.l a0,newPalette    ; save address of palette in #newPalette
;	move.w #1,updatePaletteFlag ; set flag to update palette in next vblank int
;.pal_back_set
;
;    lea screen,a0
;    lea back3d,a1
;    bsr _drawback
;    rts

drawTestScreen
	; draws a scrolling texture to chunky screen

	lea screen,a0
	lea texture,a1

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

	adda.l #320,a1

	dbra d0,.yloop

	rts


	section data,bss

screen
	blk.b 320*176,0

	section data,data

texture
	incbin "data/texture.uc"

soundtrack
	; incbin "data/igen.wav"          ; stereo, 22050 Hz
	incbin "data/thulenebula.wav" ; mono, 22050 Hz

pal2
    blk.b 1024,255

back3d
    incbin "data/back_3d.chunky"

pal_back3d
    incbin "data/back_3d.pal"
pal_back3d_is_set: dc.l 0
pal_back3d_end: dc.b "back3d_end"


pal_feedyourhead
    incbin "data/feedyourhead.pal"
pal_feedyourhead_is_set: dc.l 0
pal_feedyourhead_end: dc.b "feedyourhead_end"
