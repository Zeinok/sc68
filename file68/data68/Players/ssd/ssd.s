;;; -*- mode: asm; tab-width: 16; indent-tabs-mode: t; comment-column: 64
;;; 
;;; Animal Mine SidSoundDesigner Replayer by MC
;;;
;;; 1.00 reassembled,fixed and new stuff added by .defjam./.checkpoint.
;;; 1.01 Falcon-IDE-fix by FroST
;;; 1.02 Pattern fix, Optimisation, comments by Ben/OVR
;;; 1.03 Portamento fix, additional comments by Grazey/PHF
;;; 1.04 Position Independant Code (PIC) AKA PC Relative (PCR)
;;; 
;;; Uses Timers A,B & D
;;; 
;;; Timer C <-> B conversion - Ozk and Grazey
;;; 
;;; For a standalone version set 'test' to 0. The assemble as an excutable.
;;; Then remove the first $1c bytes from the start of the file.
;;; 
;;; Tabs to 16

	opt	a+,p+		; Auto PCR, Check for PCR
	near	a4		; Relocate a4 displacement

	opt	o-		; Don't opimize the jump table
music:	bra.w	init_music	; +0
	bra.w	my_replay_music	; +4
	bra.w	exit_player	; +8
	bra.w	set_music_off	; +12
	bra.w	set_music_on	; +16
	bra.w	set_sid_off	; +20
	bra.w	set_sid_on	; +24
	bra.w	set_screen_freq	; +28
	opt	o-		; $$$ CHANGE ME Optimize from now on

lall:	dc.b	"Sid Sound Designer by MC of Animal Mine 93-95 "
	dc.b	"ver. 1.04 ABD "
	dc.b	"fixed by .defjam./.checkpoint. "
	dc.b	"Falcon fix by FroST "
	dc.b	"Pattern Break fix by Ben/OVR "
	dc.b	"Portamento fix by Grazey/PHF"
	dc.b	"PCR by Ben/OVR "
	even

volume			SET $0E	;; 14
instr_type		SET $2C	;; 44
hardw_buzz		SET $3A	;; 58
extend_tone		SET $3D	;; 61
extend_wave		SET $3E	;; 62
tfmx_effect		SET $3F	;; 63
extend_noiz_adr		SET $40	;; 64
extend_tone_adr		SET $44	;; 68
extend_wave_adr		SET $48	;; 72
extend_noiz     	SET $4F	;; 79
extend_tone_flag 	SET $4E	;; 78
extend_buzz		SET $50	;; 80


my_replay_music:
	movem.l	D0-A6,-(a7)
	lea	music(pc),a4
	tst.b	do_replay_flag(a4)
	beq.s	no_replay

	bsr	replay_music

	tst.b	music_off_flag(a4)
	beq.s	no_music_off

	move	#$2700,SR
	bsr	clear_YM
	bsr	restore_MFP
	sf	do_replay_flag(a4)
	move	#$2300,SR
no_music_off:
no_replay:
	movem.l	(a7)+,D0-A6
	rts


set_music_off:
	lea	music(pc),a4
	bsr	clear_YM
	st	music_off_flag(a4)
	rts

set_music_on:	
	lea	music(pc),a4
	bsr	set_sid_on
	sf	music_off_flag(a4)
	st	do_replay_flag(a4)
	rts

music_off_flag:	DC.B	0
do_replay_flag:	DC.B	0


exit_player:
	movem.l	d0-a6,-(a7)
	lea	music(pc),a4
	bsr	restore_MFP
	bsr	restore_h200
	move	#$2700,SR
	bsr	clear_YM
	move	#$2300,SR
	movem.l	(a7)+,d0-a6
	rts

save_MFP:
	move	#$2700,SR
	lea	mfp_regs(PC),A0
	move.b	$FFFFFA07.w,(A0)+ ;IERA
	move.b	$FFFFFA09.w,(A0)+ ;IERB
	move.b	$FFFFFA13.w,(A0)+ ;IMRA
	move.b	$FFFFFA15.w,(A0)+ ;IMRB

	move.b	$FFFFFA17.w,(A0)+ ;Interrupt Vektor Register

	move.b	$FFFFFA19.w,(A0)+ ;Timer A Control
	move.b	$FFFFFA1B.w,(A0)+ ;Timer B Control
	move.b	$FFFFFA1D.w,(A0)+ ;Timer C/D Control

	bclr	#5,$FFFFFA07.w	;Timer A
	bclr	#0,$FFFFFA07.w	;Timer B
	bclr	#4,$FFFFFA09.w	;Timer D
	
	bclr	#5,$FFFFFA13.w	;Timer A
	bclr	#0,$FFFFFA13.w	;Timer B
	bclr	#4,$FFFFFA15.w	;Timer D

	lea	irq_vectors(PC),A0
	move.l	$00000120.w,(A0)+	;Timer B
	move.l	$00000110.w,(A0)+	;Timer D
	move.l	$00000134.w,(A0)+	;Timer A
	move.l	$00000060.w,(A0)+	;Spurious irq

	bclr	#3,$FFFFFA17.w	; software end of int.
	lea	VoidIrq(PC),A0
	move.l	A0,$00000060.w
	move.l	A0,$00000110.w
	move.l	A0,$00000120.w
	move.l	A0,$00000134.w
	move	#$2300,SR
	rts

mfp_regs:	ds.b	16
irq_vectors:	dc.l	0,0,0,0,0,0,0,0

restore_MFP:
	move	#$2700,SR

;Stop All Timers
	bclr	#5,$FFFFFA07.w	;Timer A
	bclr	#0,$FFFFFA07.w	;Timer B
	bclr	#4,$FFFFFA09.w	;Timer D
	
	bclr	#5,$FFFFFA13.w	;Timer A
	bclr	#0,$FFFFFA13.w	;Timer B
	bclr	#4,$FFFFFA15.w	;Timer D

	lea	mfp_regs(PC),A0
	move.b	(A0)+,$FFFFFA07.w	;IERA
	move.b	(A0)+,$FFFFFA09.w	;IERB
	move.b	(A0)+,$FFFFFA13.w	;IMRA
	move.b	(A0)+,$FFFFFA15.w	;IMRB

	move.b	(A0)+,$FFFFFA17.w	;Interrupt Vektor Register

	move.b	(A0)+,$FFFFFA19.w	;Timer A Control
	move.b	(A0)+,$FFFFFA1B.w	;Timer B Control

	move.l	d0,-(sp)
	move.b	(A0)+,d0
	and.b	#%11110000,$FFFFFA1D.w	;Timer C/D Control (only TD bits)
	and.b	#7,d0
	or.b	d0,$fffffa1d.w
	move.l	(sp)+,d0

	move.b 	#$C0,$FFFFFA21.w	;Timer B Data

	lea	irq_vectors(PC),A0
	move.l	(A0)+,$00000120.w	;Timer B
	move.l	(A0)+,$00000110.w	;Timer D
	move.l	(A0)+,$00000134.w	;Timer A
	move.l	(A0)+,$00000060.w	;Spurious irq

	move	#$2300,SR
	rts

set_sid_on:
	lea	music(pc),a4
	move	#$2700,SR
	pea	VoidIrq(pc)
	move.l	(a7)+,$00000060.w
	bsr	copy_sid_psg_rout

	move.l	#td0,$00000110.w	;Timer D
	andi.b	#$F0,$FFFFFA1D.w
	ori.b	#1,$FFFFFA1D.w
	move.b	#255,$FFFFFA25.w	;data
	bset	#4,$FFFFFA15.w
	bclr	#4,$FFFFFA09.w

	move.l	#tb0,$120.w		;Timer B
	move.b	#%11,$fffffa1b.w
	move.b	#255,$fffffa21.w	;data
	bset	#0,$fffffa13.w
	bclr	#0,$fffffa07.w

	move.l	#ta0,$00000134.w	;Timer A
	clr.b	$FFFFFA19.w
	move.b	#255,$FFFFFA1F.w	;data
	bset	#5,$FFFFFA13.w
	bclr	#5,$FFFFFA07.w

	bclr	#3,$FFFFFA17.w		; software end of int.
	move	#$2300,SR
	rts

set_sid_off:
	lea	music(pc),a4
	move	#$2700,SR

	lea	VoidIrq(PC),A0
	move.l	A0,$00000060.w
	move.l	A0,$00000110.w		;Timer D
	and.b	#%11110000,$fffffa1d.w	;Preserve Timer C bits!
	move.b	#255,$FFFFFA25.w
	bclr	#4,$FFFFFA15.w
	bclr	#4,$FFFFFA09.w

	move.l	A0,$120.w		;Timer B
	clr.b	$fffffa1b.w
	move.b	#255,$fffffa21.w
	bclr	#0,$fffffa07.w
	bclr	#0,$fffffa13.w

	move.l	A0,$00000134.w		;Timer A
	clr.b	$FFFFFA19.w
	move.b	#255,$FFFFFA1F.w
	bclr	#5,$FFFFFA13.w
	bclr	#5,$FFFFFA07.w
	move	#$2300,SR
	rts


save_h200:
	move	#$2700,SR
	lea	$00000200.w,A0
	lea	save200(PC),A1
do_s2:	move.w	#$0100/4-1,D0
sc0:	move.l	(A0)+,(A1)+
	dbra	D0,sc0
	move	#$2300,SR
	rts

restore_h200:
	move	#$2700,SR
	lea	$00000200.w,A1
	lea	save200(PC),A0
	bra.s	do_s2
	
save200:
	DS.B	$0100


copy_sid_psg_rout:
	lea	sid_irqs_s(PC),A0
	lea	$00000200.w,A1
	move.w	#sid_irqs_e-sid_irqs_s-1,D0
c0:	move.b	(A0)+,(A1)+
	dbra	D0,c0
	rts


clear_YM:				; Frost Fix
	move.b	#$a,$ffff8800.w
	clr.b	$ffff8802.w
	move.b	#9,$ffff8800.w
	clr.b	$ffff8802.w
	move.b	#8,$ffff8800.w
	clr.b	$ffff8802.w

;;; Never modify bit 6 on a Falcon, coz bit 7 in YM's port A means
;;; "Falcon internal IDE drive on/off", changing bit 6 in reg 7 will
;;; provide a direction change of the flow from port A (do you follow
;;; me, coz I don't know how to explain in english).

	move.b	#7,$ffff8800.w
	move.b	$ffff8802.w,d0
	move.b	#%11000000,$ffff8802.w

	moveq	#6,d0
clr_ym_r	
	move.b	d0,$ffff8800.w
	clr.b	$ffff8802.w
	dbra	d0,clr_ym_r
	clr.b	$FFFF8800.w
	clr.b	$FFFF8802.w
	move.b	#$07,$FFFF8800.w
	move.b	#$FF,$FFFF8802.w
	rts

;D0 = Freq.	( D0>=50 )
set_screen_freq:
	move.l	D0,screen_freq(a4)
	rts

screen_freq:	dc.l	50

vbl_timer_inc:	dc.l	0
vbl_timer:	dc.l	0

my_screen_freq:	dc.l	0
lv:	DC.W	0

replay_music:
	move.l	screen_freq(PC),D1
	cmp.b	#50,D1
	beq.s	dorep

	cmp.l	my_screen_freq(PC),D1
	beq.s	freq_da

;Initialise New Screenfrequency

	move.l	D1,my_screen_freq(a4)
	move.l	#50*1000,D0
	divu	D1,D0
	and.l	#$0000FFFF,D0
	move.l	D0,vbl_timer_inc(a4)
	clr.l	vbl_timer(a4)
	
freq_da:
	
	move.l	vbl_timer(PC),D0
	add.l	vbl_timer_inc(PC),D0
	cmp.l	#(50*1000),D0
	blt.s	lvt
	sub.l	#(50*1000),D0
lvt:
	move.l	D0,vbl_timer(a4)

	divu	#1000,D0
	move.w	lv(PC),D1
	move.w	D0,lv(a4)
	sub.w	D1,D0
	tst.w	D0
	bne.s	dorep
	rts

;; replay_music:

dorep:	moveq	#0,D4
	lea	$FFFF8800.w,A5
	subq.w	#1,note_count(a4)
	bne.s	do_fx_all
	
	movea.l	ReNoteCntPtr(pc),a1
	move.w	(a1),note_count(a4)
	
	movea.l	CurPatPtr(PC),A1
	move.w	PatPos(PC),D0
	move.w	D0,D1
	lsl.w	#3,D0
	lsl.w	#2,D1	;	mul 12

	add.w	D1,D0
	adda.w	D0,A1
	move.l	A1,PatPtr(a4)
	tst.b	PatBreakFlag(a4)
	bpl.s	L0004
	move	#$40,PatPos(a4)	; $$$ ben: pattern break fix
	sf	PatBreakFlag(a4)
	bra.s	L0005
	
L0004:	addq.w	#1,PatPos(a4)
L0005:	cmpi.w	#$0040,PatPos(a4)	; $$$
	bne.s	L0006
	
	bsr	load_pos	
	
L0006:	lea	VoiceData_A(PC),A0
	bsr	getnote
	
	lea	VoiceData_B(PC),A0
	bsr	getnote
	
	lea	VoiceData_C(PC),A0
	bsr	getnote

do_fx_all:	lea	VoiceData_B(PC),A0
	bsr	do_fx
	bsr	do_tone_control
	bsr	do_wave_control
	
	lea	VoiceData_C(PC),A0
	bsr	do_fx
	bsr	do_tone_control
	bsr	do_wave_control
	
	lea	VoiceData_A(PC),A0
	bsr	do_fx
	bsr	do_tone_control
	bsr	do_wave_control


	;; DO sid stuff
	
	move	#$2700,SR

	
	lea	VoiceData_A(PC),A0
	tst.b	hardw_buzz(A0)
	bne.s	no_wow_a
	
	tst.w	SidOnOffA(a4)
	bpl.s	no_wow_a
	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	no_wow_a
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	no_wow_a
	lea	wave_form1(PC),A6
	move.w	0(A0),D1
scan_more_a:	tst.w	(A6)
	bmi.s	no_wow_a
	cmp.w	(A6)+,D1
	blt.s	do_tfm_a
	addq.l	#4,A6
	bra.s	scan_more_a
	
do_tfm_a:	move.w	(A6)+,D2
	move.w	(A6)+,D3
	move.l	#$01900000,D0
	mulu	#$00A3,D2
	divu	D2,D0
	mulu	D1,D0
	tst.w	SidDephaseA(a4)
	bne.s	nix_dual_a
	addi.l	#$00002000,D0
nix_dual_a:	addi.l	#$00002000,D0
	add.l	D0,D0
	add.l	D0,D0
	swap	D0
	move.b	volume(A0),tb1+4.w

	move.b	d0,$fffffa21.w
	move.b	d3,$fffffa1b.w
	or.b	#1,$fffffa07.w
	bra.s	DoSidVoiceB
	
no_wow_a:	
	bclr	#0,$FFFFFA07.w

	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	DoSidVoiceB
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	DoSidVoiceB
	bclr	#0,instr_type(A0)

	;; Sid Voice B
	
DoSidVoiceB:
	lea	VoiceData_B(PC),A0
	tst.b	hardw_buzz(A0)
	bne.s	no_wow
	
	tst.w	SidOnOffB(a4)
	bpl.s	no_wow
	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	no_wow
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	no_wow
	move.l	WavFormPtr(pc),A6
	move.w	0(A0),D1
scan_more:
	tst.w	(A6)
	bmi.s	no_wow
	cmp.w	(A6)+,D1
	blt.s	do_tfm
	addq.l	#4,A6
	bra.s	scan_more
do_tfm:	move.w	(A6)+,D2
	move.w	(A6)+,D3
	move.l	#$01900000,D0
	mulu	#$00A3,D2
	divu	D2,D0
	mulu	D1,D0
	tst.w	SidDephaseB(a4)
	bne.s	nix_dual_b
	addi.l	#$00002000,D0
nix_dual_b:	addi.l	#$00002000,D0
	lsl.l	#2,D0
	swap	D0
	move.b	volume(A0),ta1+4.w

	move.b	D0,$FFFFFA1F.w
	move.b	D3,$FFFFFA19.w
	bset	#5,$FFFFFA07.w
	bra.s	DoSidVoiceC
	
no_wow:	bclr	#5,$FFFFFA07.w
	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	DoSidVoiceC
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	DoSidVoiceC
	bclr	#0,instr_type(A0)

	
DoSidVoiceC:
	lea	VoiceData_C(PC),A0
	tst.b	hardw_buzz(A0)
	bne.s	no_wow_c
	tst.w	SidOnOffC(a4)
	bpl.s	no_wow_c
	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	no_wow_c
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	no_wow_c
	lea	wave_form1(PC),A6
	move.w	0(A0),D1
scan_more_c:	tst.w	(A6)
	bmi.s	no_wow_c
	cmp.w	(A6)+,D1
	blt.s	do_tfm_c
	addq.l	#4,A6
	bra.s	scan_more_c
do_tfm_c:	move.w	(A6)+,D2
	move.w	(A6)+,D3
	move.l	#$01900000,D0
	mulu	#$00A3,D2
	divu	D2,D0
	mulu	D1,D0
	tst.w	SidDephaseC(a4)
	bne.s	nix_dual_c
	addi.l	#$00002000,D0
nix_dual_c:	addi.l	#$00002000,D0
	lsl.l	#2,D0
	swap	D0
	move.b	volume(A0),td1+4.w

	andi.b	#$F0,$FFFFFA1D.w
	or.b	D3,$FFFFFA1D.w
	move.b	D0,$FFFFFA25.w
	bset	#4,$FFFFFA09.w
	bra.s	wow_c
	
no_wow_c:	bclr	#4,$FFFFFA09.w
	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	wow_c
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	wow_c
	bclr	#0,instr_type(A0)

wow_c:	move	#$2300,SR
	move.b	#$FF,D0	; All OFF
	
	lea	VoiceData_C(PC),A0
	moveq	#2,D6
	moveq	#5,D7
	bsr	SetMixerAndNoise
	
	lea	VoiceData_B(PC),A0
	moveq	#1,D6
	moveq	#4,D7
	bsr	SetMixerAndNoise
	
	lea	VoiceData_A(PC),A0
	btst	#1,hardw_buzz(A0)
	bne.s	SetMixer
	moveq	#0,D6
	moveq	#3,D7
	bsr	SetMixerAndNoise

	
SetMixer:
	move.w	#$0700,D7
	move.b	D0,D7
	movep.w D7,0(A5)
	
SetVoiceA:
	lea	VoiceData_A(PC),A0
	btst	#1,hardw_buzz(A0)
	bne.s	SetVoiceB

	;; Set voice A period
	move.w	0(A0),D0
	move.b	0(A0),D1
	moveq	#0,D7
	move.b	D0,D7
	movep.w D7,0(A5)
	move.w	#$0100,D7
	move.b	D1,D7
	movep.w	D7,0(A5)
	
	tst.b	hardw_buzz(A0)
	bne.s	no_hardw_auto_a
	tst.b	extend_buzz(A0)
	beq.s	no_hardw_auto_a
	btst	#2,instr_type(A0)
	beq.s	no_hardw_auto_a
	btst	#3,instr_type(A0)
	beq.s	no_hardw_auto_a

	;; Set envelop period maaped to channel A
	lsr.w	#4,D0
	move.w	#$0B00,D7
	move.b	D0,D7
	movep.w	D7,0(A5)
	move.w	#$0C00,D7
	movep.w D7,0(A5)
	
no_hardw_auto_a:	tst.w	SidOnOffA(a4)
	bpl.s	okay_a
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	okay_a
	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	okay_a
	bra.s	SetVoiceB
	
okay_a:
	;; Set Voice A volume
	move.w	#$0800,D7
	move.b	volume(A0),D7
	movep.w	D7,0(A5)

SetVoiceB:	lea	VoiceData_B(PC),A0

	;; Set voice B period
	move.w	0(A0),D0
	move.b	0(A0),D1
	move.w	#$0200,D7
	move.b	D0,D7
	movep.w	D7,0(A5)
	move.w	#$0300,D7
	move.b	D1,D7
	movep.w	D7,0(A5)
	
	tst.b	hardw_buzz(A0)
	bne.s	no_hardw_auto_b
	tst.b	extend_buzz(A0)
	beq.s	no_hardw_auto_b
	btst	#2,instr_type(A0)
	beq.s	no_hardw_auto_b
	btst	#3,instr_type(A0)
	beq.s	no_hardw_auto_b

	
	;; Set envelop period to channel B
	lsr.w	#4,D0
	move.w	#$0B00,D7
	move.b	D0,D7
	movep.w	D7,0(A5)
	move.w	#$0C00,D7
	movep.w	D7,0(A5)
	
no_hardw_auto_b:	tst.w	SidOnOffB(a4)
	bpl.s	okay_b
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	okay_b
	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	okay_b
	bra.s	SetVoiceC
	
okay_b:
	;; Set Voice B volume
	move.w	#$0900,D7
	move.b	volume(A0),D7
	movep.w	D7,0(A5)

	
SetVoiceC:
	lea	VoiceData_C(PC),A0
	;; Set voice B period
	
	move.w	0(A0),D0
	move.b	0(A0),D1
	move.w	#$0400,D7
	move.b	D0,D7
	movep.w	D7,0(A5)
	move.w	#$0500,D7
	move.b	D1,D7
	movep.w	D7,0(A5)
	tst.b	hardw_buzz(A0)
	bne.s	no_hardw_auto_c
	tst.b	extend_buzz(A0)
	beq.s	no_hardw_auto_c
	btst	#2,instr_type(A0)
	beq.s	no_hardw_auto_c
	btst	#3,instr_type(A0)
	beq.s	no_hardw_auto_c
	lsr.w	#4,D0
	move.w	#$0B00,D7
	move.b	D0,D7
	movep.w	D7,0(A5)
	move.w	#$0C00,D7
	movep.w 	D7,0(A5)
no_hardw_auto_c:
	tst.w	SidOnOffC(a4)
	bpl.s	okay_c
	cmpi.b	#3,tfmx_effect(A0)
	blt.s	okay_c
	cmpi.b	#5,tfmx_effect(A0)
	bgt.s	okay_c
	bra.s	no_c
okay_c:
	;; Set Voice B volume
	move.w	#$0A00,D7
	move.b	volume(A0),D7
	movep.w	D7,0(A5)
	
no_c:	rts

	
; IN	a0	Voice struct
;	a1	Pattern Ptr
; 

getnote:
	tst.b	(A1)
	beq	test_env
	bmi	digi_
	move.b	2(A1),D1
	cmpi.b	#$3F,D1
	bhi	same_instr
	move.b	D1,57(A0)
	move.w	D4,76(A0)
	move.b	D4,56(A0)
	lea	CurInfo(PC),A6
	moveq	#0,D0
	move.b	57(A0),D0
	add.w	D0,D0
	add.w	D0,D0
	move.l	0(A6,D0.w),D0
	move.l	D0,60(A0)
	lsr.l	#8,D0
	tst.b	D0
	beq.s	no_wave_control
	lea	wave_table(PC),A6
	moveq	#0,D1
	move.b	D0,D1
	subq.b	#1,D1
	add.w	D1,D1
	add.w	D1,D1
	adda.l	0(A6,D1.w),a6
	move.l	A6,extend_wave_adr(A0)

no_wave_control:
	lsr.l	#8,D0
	tst.b	D0
	beq.s	no_tone_control
	lea	tone_table(PC),A6
	moveq	#0,D1
	move.b	D0,D1
	subq.b	#1,D1
	add.w	D1,D1
	add.w	D1,D1
	adda.l	0(A6,D1.w),a6
	move.l	A6,extend_tone_adr(A0)

no_tone_control:
	lsr.w	#8,D0
	tst.b	D0
	beq.s	no_noise_control
	lea	nois_table(PC),A6
	moveq	#0,D1
	move.b	D0,D1
	subq.w	#1,D1
	add.w	D1,D1
	add.w	D1,D1
	adda.l	0(A6,D1.w),a6
	move.l	A6,extend_noiz_adr(A0)

no_noise_control:
	move.b	57(A0),D1
	andi.w	#$003F,D1
	move.l	InstBasePtr(pc),A2
	asl.w	#8,D1
	adda.w	D1,A2
	move.w	0(A2),D0
	move.w	D0,42(A0)
	move.b	2(A2),instr_type(A0)
	move.w	4(A2),46(A0)
	move.b	3(A2),45(A0)
	move.l	6(A2),10(A0)
	move.l	252(A2),48(A0)
	move.w	D4,20(A0)
	move.l	10(A2),22(A0)
	move.w	volume(A2),26(A0)
	lea	16(A2),A3
	move.l	A3,16(A0)
	move.w	D4,32(A0)
	move.l	240(A2),34(A0)
	move.w	244(A2),38(A0)
	lea	128(A2),A3
	moveq	#0,D0
	move.b	(A1),D0
	move.w	D0,D1
	lsr.w	#4,D1
	lea	note_2_freq(PC),A2
	andi.w	#$000F,D0

	move.l	A3,28(A0)
	st	40(A0)
	andi.w	#$000F,D0
	move.w	D0,D2
	add.w	D2,D2
	move.w	0(A2,D2.w),D2
	lsr.w	D1,D2
	move.w	D2,2(A0)
	move.w	D0,D3
	add.w	48(A0),D3
	add.w	D3,D3
	move.w	0(A2,D3.w),D3
	lsr.w	D1,D3
	sub.w	D2,D3
	move.w	D3,52(A0)
	add.w	50(A0),D0
	add.w	D0,D0
	move.w	0(A2,D0.w),D0
	lsr.w	D1,D0
	sub.w	D2,D0
	
	move.w	D0,54(A0)
	
same_instr:
	move.b	1(A1),D1
	beq.s	no_calc_tie
	sub.w	4(A0),D2
	ext.l	D2
	andi.l	#$000000FF,D1

	move.l	SpeedPtr(pc),a4
	muls	(a4),d1		; Speed
	lea	music(pc),a4
	divs	D1,D2
	
	beq.s	no_calc_tie
	move.w	D2,6(A0)
	bra.s	yes_calc_tie
	
no_calc_tie:
	move.w	2(A0),4(A0)
	move.w	D4,6(A0)
yes_calc_tie:
	move.b	3(A1),D0
	andi.w	#$001F,D0
	move.b	D0,15(A0)
	
test_env:
	addq.l	#3,A1
	move.b	(A1)+,D1
	lsr.b	#6,D1
	tst.b	D1
	beq.s	same_env
	btst	#2,instr_type(A0)
	bne.s	he_adsr
	cmpi.b	#1,D1
	beq.s	attack_env
	cmpi.b	#2,D1
	beq.s	sustain_env
	move.w	24(A0),20(A0)
	move.w	36(A0),32(A0)
	bclr	#0,hardw_buzz(A0)
	st	40(A0)
	rts
	
sustain_env:	move.w	22(A0),20(A0)
	move.w	34(A0),32(A0)
	bclr	#0,hardw_buzz(A0)
	st	40(A0)
	rts
	
attack_env:	move.w	D4,8(A0)
	move.w	D4,20(A0)
	move.w	D4,32(A0)
	bclr	#0,hardw_buzz(A0)
	st	40(A0)
	bclr	#1,hardw_buzz(A0)	
same_env:	rts
	
he_adsr:	moveq	#$0B,D7
	moveq	#$0C,D6
	moveq	#$0D,D5
	tst.b	D1
	beq.s	cont_he
	cmpi.b	#2,D1
	beq.s	sustain_he
	cmpi.b	#3,D1
	beq	release_he
	move	#$2700,SR
	btst	#3,instr_type(A0)
	bne.s	attack_hardw_aut
	move.b	D7,(A5)
	move.b	47(A0),2(A5)
	move.b	D6,(A5)
	move.b	46(A0),2(A5)
attack_hardw_aut:	move.b	D5,(A5)
	btst	#0,45(A0)
	bne.s	yes_hardw_type
	move.b	(A5),D0
	cmp.b	45(A0),D0
	beq.s	no_hardw_type
yes_hardw_type	move.b	45(A0),2(A5)
no_hardw_type	move.w	D4,8(A0)
	move.w	D4,32(A0)
	st	40(A0)
	bclr	#0,hardw_buzz(A0)
	bclr	#1,hardw_buzz(A0)
	beq.s	same_he
	bclr	#5,$FFFFFA07.w
	bclr	#0,$FFFFFA07.w	
same_he	move	#$2300,SR
cont_he:	rts
	
sustain_he:	move	#$2700,SR
	btst	#3,instr_type(A0)
	bne.s	sustain_hardw_au
	move.b	D7,(A5)
	move.b	47(A0),2(A5)
	move.b	D6,(A5)
	move.b	46(A0),2(A5)
sustain_hardw_au:	move.b	D5,(A5)
	btst	#0,45(A0)
	bne.s	yes_sus_htype
	move.b	(A5),D0
	cmp.b	45(A0),D0
	beq.s	no_sus_htype
yes_sus_htype	move.b	45(A0),2(A5)
no_sus_htype	move.w	D4,8(A0)
	move.w	34(A0),32(A0)
	st	40(A0)
	bclr	#0,hardw_buzz(A0)
	bclr	#1,hardw_buzz(A0)
	beq.s	same_sus_he
	bclr	#5,$FFFFFA07.w
	bclr	#0,$FFFFFA07.w
same_sus_he	move	#$2300,SR
	rts

release_he:	bset	#0,hardw_buzz(A0)
	rts
	
digi_:	btst	#2,hardw_buzz(A0)
	bne.s	oka
	cmpi.b	#$0E,2(A1)
	bne.s	test_2
	move.w	#$FFFF,PatBreakFlag(a4)
	bra.s	oka
	
test_2:	cmpi.b	#$0F,2(A1)
	bne.s	oka
	move.w	#$FFFF,what(a4)
oka:	move.b	2(A1),D0
	andi.w	#$000F,D0
	asl.w	#2,D0
	tst.w	what(a4)
	bmi.s	only_speed
	moveq	#0,D0
	move.b	3(A1),D0
	ext.w	D0
	add.w	D0,D0
	add.w	D0,D0
	jsr	FxJmpTbl(PC,D0.w)
L0040:	addq.l	#4,A1
	rts
	
only_speed:
	move.b	3(A1),D0
	ext.w	D0
	move.l	SpeedPtr(pc),a6
	move.w	D0,(a6)		; Grazey Portamento Fix
	addq.l	#4,A1
	move.w	D4,what(a4)
	rts
	
FxJmpTbl:
	BRA.W FxNone
	BRA.W ToggleSidDephaseA
	BRA.W ToggleSidDephaseB
	BRA.W ToggleSidDephaseC
	BRA.W ActivSidVoiceA
	BRA.W StopSidVoiceA
	BRA.W ActivSidVoiceB
	BRA.W StopSidVoiceB
	BRA.W ActivSidVoiceC
	BRA.W StopSidVoiceC
	BRA.W do_wave_1
	BRA.W do_wave_2
	BRA.W do_wave_3
	BRA.W reset_all
	BRA.W FxNone
	BRA.W FxNone
	
	
tb0:	= $0200
tb1:	= tb0+$0010

ta0:	= tb1+$0010
ta1:	= ta0+$0010

td0:	= ta1+$0010
td1:	= td0+$0010

sid_irqs_s:
	move.l	#$08080000,$FFFF8800.w
	move.w	#tb1,$00000122.w
	rte
	
	move.l	#$08080000,$FFFF8800.w
	move.w	#tb0,$00000122.w
	rte

	move.l	#$09090000,$FFFF8800.w
	move.w	#ta1,$00000136.w
	rte

	move.l	#$09090000,$FFFF8800.w
	move.w	#ta0,$00000136.w
	rte

	move.l	#$0A0A0000,$FFFF8800.w
	move.w	#td1,$00000112.w
	rte

	move.l	#$0A0A0000,$FFFF8800.w
	move.w	#td0,$00000112.w
	rte
sid_irqs_e:

VoidIrq:	rte
	
do_tone_control:
	move.l	#$01010100,D6
	tst.b	extend_tone(A0)
	beq.s	L004C
	movea.l extend_tone_adr(A0),A6
	moveq	#0,D1
	move.b	(A6)+,D1

	;; $$$ ben: that's my fix for the bug below
	cmp.b	#$FE,d1
	blo.s	tone_a
	subq	#1,a6
	bra.s	tone_a_bis

	;; $$$ ben:
	;; Meaningless! Previous moveq prevent $FE in d1 !!!
	;; Btw, $FE will not be matched if the moveq was removed
	;; since previous test with $FF skips the $FE test code.
	;; Currently the whole thing can be replaced by:
	;; cmp.b	#$FF,D1
	;; seq	d1
	;; But more likely this is not what it intends to do.
		
;	cmp.b	#$FF,D1		; NOP
;	bne.s	L004B
;	moveq	#0,D1

;	cmp.b	#$FE,D1		; END
;	bne.s	L004B
;	subq.l	#1,A6
;	move.b	-1(A6),D1
	
tone_a:	move.l	A6,extend_tone_adr(A0)
	add.w	D1,D1
	add.w	D1,D1
	move.l	tone_data(PC,D1.w),D7
	move.l	D7,extend_tone_flag(A0)
	rts

tone_a_bis:		
	move.l	A6,extend_tone_adr(A0)	
L004C:	move.l	D6,extend_tone_flag(A0)
	rts
	
tone_data:	DC.L $00000000	; Background, drum or snare effect
	DC.L $01000000
	DC.L $00010000
	DC.L $01010000
	DC.L $00000100
	DC.L $01000100
	DC.L $01010100
	
do_wave_control	tst.b	extend_wave(A0)
	beq.s	no_control_wave
	movea.l extend_wave_adr(A0),A6
	move.b	(A6)+,D1
	
	cmpi.b	#$FF,D1		; NOP 
	beq.s	xno_hardw_auto_b

	cmpi.b	#$FE,D1		; End
	bne.s	wave_b

	; $$$ ben:	
	; subq.b #1,d1 is useless since d1 value is written by the
	; move.b -1(A6),D1 (which is always the current d1 value !!!)
	; According to other part in this source I assume it is a:
	; subq #1,a6
	; Which has the effect to load previous value. That seems more
	; meaningfull to me.
	; This bug is probably the reason of time calculation bugs in
	; many SidSound file...
	;
	;	subq.b	#1,D1

	subq	#1,a6
	move.b	-1(A6),D1
	
wave_b:	move.l	A6,extend_wave_adr(A0)
	tst.b	D1
	bne.s	no_control_wave
	
xno_hardw_auto_b:	bclr	#2,instr_type(A0)
no_control_wave	rts

;;; 
;;; 
;;; IN:	d6	sound-bit
;;;	d7	noise-bit
;;;	d0	current mixer bits
;;;	a0	Voice data
;;;	a5	YM hardware
;;;
;;; OUT:	d0	updated
;;;	YM noise period updated 
;;;
;;; 
SetMixerAndNoise:
	btst	#0,instr_type(A0)
	beq.s	.toneOFF
	
	tst.b	extend_tone(A0)
	bne.s	.toneON
	
	tst.b	extend_noiz(A0)
	beq.s	.toneOFF
.toneON:
	bclr	D6,D0
	
.toneOFF:
	btst	#1,instr_type(A0)
	beq.s	.exit
	tst.b	60(A0)
	bne.s	.noNoiseCtl
	
	move.w	#$0600,D5
	move.b	43(A0),D5
	movep.w	D5,0(A5)
	bra.s	.noiseON
	
.noNoiseCtl:
	movea.l	extend_noiz_adr(A0),A6
	move.b	(A6)+,D1
	cmpi.b	#$FF,D1		; NOP
	beq.s	.exit
	cmpi.b	#$FE,D1		; END
	bne.s	.okSequence
	
	subq.l	#1,A6
	move.b	-1(A6),D1
	
.okSequence:
	move.l	A6,extend_noiz_adr(A0)
	move.w	#$0600,D5
	move.b	D1,D5
	movep.w	D5,0(A5)
	tst.b	extend_tone_flag(A0)
	beq.s	.exit
.noiseON:
	bclr	D7,D0
.exit:
	rts



	
do_fx:	tst.b	hardw_buzz(A0)
	bne	do_silent
	btst	#2,instr_type(A0)
	beq.s	no_he

	cmpi.b	#$0D,tfmx_effect(A0)	;******; ???
	bne.s	normal_buzz
	moveq	#1,D7
double:	movea.w	#$8800,A6
	moveq	#0,D0
	move.b	0(A0),D0
	add.b	83(A0),D0
	lea	buzzfreqtab(PC),A2
	andi.w	#$003F,D0
	move.b	#$0B,(A6)
	move.b	0(A2,D0.w),2(A6)
	move.b	#$0C,(A6)
	move.b	#0,2(A6)
	addq.b	#1,83(A0)
	cmpi.b	#$3F,83(A0)
	ble.s	do_loop
	move.b	D4,83(A0)
do_loop:	bra.s	to_tie
normal_buzz:	move.b	D4,83(A0)
	move.b	#$10,volume(A0)
	bra.s	to_tie
	
no_he:	move.w	20(A0),D0
	movea.l	16(A0),A2
	move.b	0(A2,D0.w),D1
	move.b	15(A0),D2
	asl.b	#4,D2
	add.b	D1,D2
	andi.w	#$00FF,D2
	lea	vols_y(PC),A2
	move.b	0(A2,D2.w),volume(A0)
	addq.w	#1,D0
	cmp.w	24(A0),D0
	bne.s	no_sus_rep
	move.w	22(A0),D0
no_sus_rep:	cmp.w	26(A0),D0
	bcs.s	no_rel
	bset	#0,hardw_buzz(A0)
no_rel:	move.w	D0,20(A0)
to_tie:	move.w	6(A0),D0
	beq.s	no_tie
	bmi.s	tie_lo
	add.w	4(A0),D0
	cmp.w	2(A0),D0
	blt.s	no_reach_tie_hi
	move.w	D4,6(A0)
	move.w	2(A0),D0
no_reach_tie_hi	move.w	D0,4(A0)
	bra.s	no_tie
tie_lo:	add.w	4(A0),D0
	cmp.w	2(A0),D0
	bgt.s	no_reach_tie_lo
	move.w	D4,6(A0)
	move.w	2(A0),D0
no_reach_tie_lo	move.w	D0,4(A0)
no_tie:	move.w	8(A0),D1
	add.w	10(A0),D1
	cmpi.w	#$003B,D1
	bls.s	no_re_trem
	moveq	#0,D1
no_re_trem:	move.w	D1,8(A0)
	add.w	D1,D1
	lea	trem_table(PC),A2
	move.w	0(A2,D1.w),D1
	move.w	12(A0),D2
	asr.w	D2,D1
	add.w	4(A0),D1
.noPatBrk:		

; Hi End Pitch Arpeggio Effect

	cmpi.b	#1,tfmx_effect(A0)
	bne.s	no_db_1_notex
	lea	db_table(PC),A1
	move.b	76(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,76(A0)
	cmpi.b	#1,76(A0)
	blt.s	no_db_1_notex
	move.b	#1,76(A0)
	
; SID Effect combined with Hi End Pitch Arpeggio

no_db_1_notex:	cmpi.b	#4,tfmx_effect(A0)
	bne.s	no_db_1_notey
	lea	db_table(PC),A1
	move.b	76(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,76(A0)
	cmpi.b	#1,76(A0)
	blt.s	no_db_1_notey
	move.b	#1,76(A0)
	
; Arpeggio double with SID effect

no_db_1_notey:	cmpi.b	#5,tfmx_effect(A0)
	bne.s	no_db_1_note
	lea	da_table(PC),A1
	move.b	77(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#2,77(A0)
	cmpi.b	#$0C,77(A0)
	blt	da_ok
	move.b	D4,77(A0)
	
; Different Arpeggio wavesynthesis 3pcs 6-9

no_db_1_note:	cmpi.b	#6,tfmx_effect(A0)
	bne.s	no_db_note2
	lea	db_1_table(PC),A1
	move.b	76(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,76(A0)
	cmpi.b	#$40,76(A0)
	blt.s	no_db_note2
	move.b	#1,76(A0)
	
no_db_note2:	cmpi.b	#7,tfmx_effect(A0)
	bne.s	no_db_note3
	lea	db_2_table(PC),A1
	move.b	77(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#2,77(A0)
	cmpi.b	#$20,77(A0)
	blt.s	no_db_note3
	move.b	#2,77(A0)
	
no_db_note3:	cmpi.b	#8,tfmx_effect(A0)
	bne.s	no_db_note4
	lea	da2_table(PC),A1
	move.b	76(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,76(A0)
	cmpi.b	#$30,76(A0)
	blt.s	no_db_note4
	move.b	D4,76(A0)
	
no_db_note4:	cmpi.b	#9,tfmx_effect(A0)
	bne.s	no_db_note5
	lea	dx_table(PC),A1
	move.b	77(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,77(A0)
	cmpi.b	#$20,77(A0)
	blt.s	no_db_note5
	move.b	D4,77(A0)
	
no_db_note5:	cmpi.b	#$0A,tfmx_effect(A0)
	bne.s	no_db_note6
	lea	dx_table(PC),A1
	move.b	77(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,77(A0)
	cmpi.b	#$20,77(A0)
	blt.s	spe
	move.b	D4,77(A0)
spe:	lea	da_table(PC),A1
	move.b	76(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,76(A0)
	cmpi.b	#4,76(A0)
	blt.s	no_db_note6
	move.b	D4,76(A0)
	
no_db_note6:	cmpi.b	#$0B,tfmx_effect(A0)
	bne.s	no_db_note7
	lea	arp_spec(PC),A1
	move.b	76(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,76(A0)
	cmpi.b	#$10,76(A0)
	blt.s	spe2
	move.b	D4,76(A0)
spe2:	lea	db_2_table(PC),A1
	move.b	77(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#1,77(A0)
	cmpi.b	#$12,77(A0)
	blt.s	no_db_note7
	move.b	D4,77(A0)
	
no_db_note7:	cmpi.b	#$0C,tfmx_effect(A0)
	bne.s	no_db_note
	tst.b	hardw_buzz(A0)
	bne.s	no_db_note
	btst	#2,instr_type(A0)
	beq.s	no_db_note
	lea	spec(PC),A1
	move.b	82(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),D0
	move.b	D0,56(A0)
	andi.w	#$003F,D0
	add.w	D0,32(A0)
	addq.b	#1,82(A0)
	cmpi.b	#$2A,82(A0)
	blt.s	no_db_note
	move.b	D4,82(A0)
	
; New Arpeggio (Double) with wavesynthesis

no_db_note:	cmpi.b	#2,tfmx_effect(A0)
	bne.s	da_ok
	lea	da_table(PC),A1
	move.b	77(A0),D7
	ext.w	D7
	move.b	0(A1,D7.w),56(A0)
	addq.b	#2,77(A0)
	cmpi.b	#$0C,77(A0)
	blt.s	da_ok
	move.b	D4,77(A0)
da_ok:	move.b	56(A0),D0
	beq.s	intr_0

	cmpi.b	#1,D0
	beq.s	intr_1
	add.w	54(A0),D1
	move.b	D4,56(A0)
	bra.s	intr_2
intr_1:	add.w	52(A0),D1
intr_0:	addq.b	#1,56(A0)
intr_2:	tst.b	40(A0)
	beq.s	no_pitch
	move.w	32(A0),D0
	movea.l	28(A0),A2
	move.b	0(A2,D0.w),D2
	ext.w	D2
	tst.b	60(A0)
	beq.s	no_noise_pitch2
	add.w	D2,D1
	add.w	D2,D1
	add.w	D2,D1
no_noise_pitch2	add.w	D2,D1
	addq.w	#1,D0
	cmp.w	36(A0),D0
	bne.s	no_pit_sus_rep
	move.w	34(A0),D0
no_pit_sus_rep:	cmp.w	38(A0),D0
	bcs.s	no_pit_rel
	move.b	D4,40(A0)
no_pit_rel:	move.w	D0,32(A0)
no_pitch:	move.w	D1,0(A0)
	cmpi.b	#$0D,tfmx_effect(A0)
	bne.s	no_special_buzz
	dbra	D7,double
no_special_buzz	moveq	#0,D7
	rts
do_silent:	move.b	D4,volume(A0)
	rts

buzzfreqtab:	DC.B $F0,$E2,$D4,$C8,$BE,$B2,$A8,$9E
	DC.B $96,$8E,$86,'~xqjd'
	DC.B '_YTOKGC?'
	DC.B '<852/-;('
	DC.B '&','$',$22,' ',$1E,$1C,$1B,$19
	DC.B $18,$16,$15,$14,$13,$12,$11,$10
	DC.B $0F,$0E,$0D,$0C,$0B,$0A,$09,$08
	DC.B $07,$06,$05,$04,$03,$02,$01,$00
db_table:	DC.B $01,$00,$FF
db_1_table:	DC.B $01,$00,$01,$00,$02,$00,$02,$00,$03
	DC.B $00,$FF
db_2_table:	DC.B $00,$00,$01,$01,$02,$02,$03,$03
	DC.B $04,$04,$05,$05,$06,$06,$05,$05
	DC.B $04,$04,$03,$03,$02,$02,$01,$01
	DC.B $00,$00,$FF
da_table:	DC.B $00,$00,$01,$01,$02,$02,$FF
da2_table:	DC.B $09,$09,$05,$05,$03,$03,$01,$01
	DC.B $02,$01,$01,$02,$02,$03,$03,$03
	DC.B $04,$04,$05,$00,$01,$02,$FF
dx_table:	DC.B $00,$08,$00,$07,$00,$06,$00,$05,$00
	DC.B $04,$00,$03,$00,$02,$00,$01,$00
	DC.B $01,$01,$00,$02,$00,$03,$00,$04
	DC.B $00,$05,$00,$06,$00,$FF,$01,$01
	DC.B $01,$00,$00,$00,$02,$02,$02,$00
	DC.B $00,$00,$03,$03,$03,$00,$00,$00
	DC.B $04,$04,$04,$00,$00,$00,$05,$05
	DC.B $05,$FF
spec:	DC.B $10,$0F,$10,$0E,$10,$0D,$10,$0C
	DC.B $10,$0B,$10,$0A,$10,$09,$10,$08
	DC.B $10,$07,$10,$06,$10,$05,$10,$04
	DC.B $10,$03,$10,$02,$10,$01,$10,$10
	DC.B $10,$10,$09,$08,$06,$04,$02,$00
	DC.B $02,$FF
arp_spec:	DC.B $00,$09,$00,$08,$00,$07,$00,$06
	DC.B $00,$05,$00,$04,$00,$03,$00,$02
	DC.B $00,$01,$00,$01,$02,$03,$04,$05
	DC.B $06,$07,$08,$09,$FF,$00
	
ToggleSidDephaseA:
	not.w	SidDephaseA(a4)
	rts
	
ToggleSidDephaseB:	
	not.w	SidDephaseB(a4)
	rts
	
ToggleSidDephaseC:	
	not.w	SidDephaseC(a4)
	rts
	
ActivSidVoiceA:
	pea	tb0.w
	move.l	(a7)+,$00000120.w
	st	SidOnOffA(a4)
	rts
	
ActivSidVoiceB:
	pea	ta0.w
	move.l	(a7)+,$00000134.w
	st	SidOnOffB(a4)
	rts
	
ActivSidVoiceC:
	pea	td0.w
	move.l	(a7)+,$00000110.w
	st	SidOnOffC(a4)
	rts

StopSidVoiceA:
	pea	VoidIrq(pc)
	move.l	(a7)+,$00000120.w
	sf	SidOnOffA(a4)
	rts
	
StopSidVoiceB:	
	pea	VoidIrq(pc)
	move.l	(a7)+,$00000134.w
	sf	SidOnOffB(a4)
	rts

StopSidVoiceC:	
	pea	VoidIrq(pc)
	move.l	(a7)+,$00000110.w
	sf	SidOnOffC(a4)
	rts

do_wave_1:
	pea	wave_form1(pc)
	move.l	(a7)+,WavFormPtr(a4)
	rts
	
do_wave_2:
	pea	wave_form2(pc)
	move.l	(a7)+,WavFormPtr(a4)
	rts
	
do_wave_3:
	pea	wave_form3(pc)
	move.l	(a7)+,WavFormPtr(a4)
FxNone:	rts
	
	
reset_all:
	pea	wave_form1(pc)
	move.l	(a7)+,WavFormPtr(a4)
	move.w	merk_speed(PC),offset(a4)
	st	SidDephaseA(a4)
	st	SidDephaseB(a4)
	st	SidDephaseC(a4)
	st	SidOnOffA(a4)
	st	SidOnOffB(a4)
	st	SidOnOffC(a4)
	rts

i_flag:	DC.W 0

init_music:
	lea	music(pc),a4
	movem.l A0-A1,-(a7)

	
	
	lea	i_flag(PC),A3
	tst.w	(A3)
	bne.s	already_in_relo
	st	(A3)

	bsr	save_MFP
	bsr	save_h200
already_in_relo:
	movem.l (a7)+,A0-A1

	moveq	#0,D0		;3 SID-voices
	move.w	D0,merk_d1(a4)

	move	#$2700,SR

	move.l	A0,VoiceSetPtr(a4)	;Voiceset
	move.l	A1,SoundDataPtr(a4)	;Sounddata

	movea.l	VoiceSetPtr(PC),A0
	lea	516(A0),A0
	move.l	A0,InstBasePtr(a4)
	
	movea.l	SoundDataPtr(PC),A0
	addq.l	#4,A0
	move.w	(A0),merk_speed(a4)

	bsr.s	reset_all
	
	move.l	A0,ReNoteCntPtr(a4)
	move.l	A0,SpeedPtr(a4)
	addq.l	#2,A0
	move.l	A0,SongRePtr(a4)
	addq.l	#2,A0
	move.l	A0,SongLenPtr(a4)
	addq.l	#2,A0
	move.l	A0,SMC9E(a4)
	addq.l	#2,A0
	move.l	A0,SongBasePtr(a4)
	lea	120(A0),A0
	move.l	A0,SMCA0(a4)
	move.l	A0,RePatPtr(a4)
	move.l	A0,ReCurPatPtr(a4)
	lea	CurInfo(PC),A1
	
	movea.l VoiceSetPtr(PC),A0
	lea	16900(A0),A0
	cmpi.l	#"INFO",(A0)+
	beq.s	.okInfo
	lea	DefaultInfo(PC),A0
.okInfo:	
	moveq	#$1F,D0			; $$$ ben: overflow !!!
.cpyInfo:	
	move.l	(A0)+,(A1)+
	move.l	(A0)+,(A1)+
	dbra	D0,.cpyInfo
	
	bsr	L009C

	bclr	#3,$FFFFFA17.w	;auto eoi

	bsr	copy_sid_psg_rout

	move.l	#td0,$00000110.w ;Timer D
	andi.b	#$F0,$FFFFFA1D.w
	ori.b	#1,$FFFFFA1D.w
	clr.b	$FFFFFA25.w
	bset	#4,$FFFFFA15.w
	bclr	#4,$FFFFFA09.w

L0096:	move.l	#tb0,$120.w	;Timer B
	move.b	#%11,$fffffa1b.w
	clr.b	$fffffa21.w
	bset	#0,$fffffa13.w
	bclr	#0,$fffffa07.w

L0097:	move.l	#ta0,$00000134.w ;Timer A
	clr.b	$FFFFFA19.w
	move.b	#1,$FFFFFA19.w
	move.b	#1,$FFFFFA1F.w
	bset	#5,$FFFFFA13.w
	bclr	#5,$FFFFFA07.w
	bsr.s	YmReset

	st	do_replay_flag(a4)
	sf	music_off_flag(a4)
	move	#$2300,SR
	rts


YmReset:	
	lea	VoiceData_B(PC),A0
	bclr	#5,$FFFFFA07.w	;Timer A
	bclr	#4,$FFFFFA09.w	;Timer D
	bset	#0,224(A0)
	bclr	#1,224(A0)
	bset	#0,60(A0)
	bset	#0,142(A0)
	st	223(A0)
	st	59(A0)
	st	141(A0)
	lea	$FFFF8800.w,A0
	lea	YmShadow(PC),A1
	moveq	#$0C,D1
.lpReg:	move.w	(A1)+,D0
	movep.w	D0,0(A0)
	dbra	D1,.lpReg
	move.b	#7,(A0)
	move.b	(A0),D0
	andi.b	#$C0,D0
	ori.b	#$3F,D0
	move.b	D0,2(A0)
	rts

YmShadow:
	DC.B $00,$00,$01,$00,$02,$00,$03,$00
	DC.B $04,$00,$05,$00,$06,$00,$08,$00
	DC.B $09,$00,$0A,$00,$0B,$00,$0C,$00
	DC.B $0D,$00

	
L009C:	bsr.s	L009D
	clr.w	PatPos(a4)
	clr.b	ta1+4.w	;8
	bra	L00AD

L009D:	
	move.l	SMC9E(PC),A0
	move.w	#1,(A0)
	
	moveq	#$1F,D0
	move.l	SMCA0(pc),A0
	
	move.l	#$00020005,D7
	lea	PatPtrTbl(PC),A1
L00A1:	move.l	A0,(A1)+
	lea	768(A0),A0
	dbra	D0,L00A1
	move.w	#1,note_count(a4)
	clr.w	CurSongPos(a4)
	move.l	RePatPtr(pc),PatPtr(a4)
	move.l	ReCurPatPtr(pc),CurPatPtr(a4)

	lea	L012E(PC),A1
	lea	VoiceData_B(PC),A0
	moveq	#9,D1
	moveq	#2,D2
.lpChannel:
	clr.b	46(A0)
	move.l	A1,16(A0)
	move.l	D7,24(A0)
	move.w	D1,28(A0)
	move.l	A1,30(A0)
	move.l	D7,36(A0)
	move.w	D1,40(A0)
	lea	82(A0),A0
	dbra	D2,.lpChannel
	rts

		
load_pos:	
	move.w	D4,PatPos(a4)
	move.w	CurSongPos(PC),D0
	addq.w	#1,D0

	move.l	SongLenPtr(pc),a0
	cmp.w	(a0),D0
	bls.s	L00AC

	move.l	SongRePtr(pc),a0
	move.w	(a0),D0
L00AC:
	move.w	D0,CurSongPos(a4)
L00AD:	
	move.l	SongBasePtr(pc),a0

	move.w	CurSongPos(PC),D0
	move.b	0(A0,D0.w),D0
	andi.w	#$007F,D0
	move.w	D0,CurPatNum(a4)
	lea	PatPtrTbl(PC),A0
	add.w	D0,D0
	add.w	D0,D0
	move.l	0(A0,D0.w),CurPatPtr(a4)
	rts

SidDephaseA:	DC.B $00,$01
SidDephaseB:	DC.B $00,$01
SidDephaseC:	DC.B $00,$01
	
SidOnOffA	DC.B $FF,$FF
SidOnOffB:	DC.B $FF,$FF
SidOnOffC:	DC.B $FF,$FF
	
wave_form1:
	DC.B $00,'h',$00,$04,$00,$01,$01,$05
	DC.B $00,$0A,$00,$02,$01,$A2,$00,$10
	DC.B $00,$03,$05,$1A,$00,'2',$00,$04
	DC.B $06,$88,$00,'@',$00,$05,$0A,'5'
	DC.B $00,'d',$00,$06,$0E,$EF,$00,$C8
	DC.B $00,$07,$FF,$FF
	
wave_form2:
	DC.B $00,'h',$00,$04,$00,$01,$01,$05
	DC.B $00,$14,$00,$02,$01,$A0,$00,$10
	DC.B $00,$03,$05,$19,$00,'d',$00,$04
	DC.B $03,'@',$00,'@',$00,$05,$0A,'2'
	DC.B $00,$C8,$00,$06,$0E,$A0,$01,$00
	DC.B $00,$07,$0F,'K',$00,'h',$00,$04
	DC.B $00,$01,$01,$05,$00,$14,$00,$02
	DC.B $01,$A0,$00,$10,$00,$03,$05,$19
	DC.B $00,'d',$00,$04,$03,'@',$00,'@'
	DC.B $00,$05,$0A,'2',$00,$C8,$00,$06
	DC.B $0E,$A0,$01,$00,$00,$07,$0F,'K'
	DC.B $FF,$FF
	
wave_form3:
	DC.B $00,'h',$00,$04,$00,$01,$01,$05
	DC.B $00,$1E,$00,$02,$01,$A0,$00,$10
	DC.B $00,$03,$05,$19,$00,$96,$00,$04
	DC.B $03,'@',$00,'@',$00,$05,$0A,'2'
	DC.B $01,',',$00,$06,$0E,$A0,$01,$00
	DC.B $00,$07,$0F,'K',$00,'h',$00,$04
	DC.B $00,$01,$01,$05,$00,$1E,$00,$02
	DC.B $01,$A0,$00,$10,$00,$03,$05,$19
	DC.B $00,$96,$00,$04,$03,'@',$00,'@'
	DC.B $00,$05,$0A,'2',$01,',',$00,$06
	DC.B $0E,$A0,$01,$00,$00,$07,$0F,'K'
	DC.B $FF,$FF
	
DefaultInfo:
	DC.B $01,$01,$00,$00,$02,$02,$00,$00
	DC.B $00,$00,$01,$00,$00,$00,$00,$03
	DC.B $00,$00,$00,$03,$03,$03,$00,$01
	DC.B $00,$00,$00,$01,$00,$00,$00,$02
	DC.B $00,$00,$00,$02,$00,$00,$00,$02
	DC.B $00,$00,$00,$02,$00,$00,$00,$01
	DC.B $00,$00,$00,$03,$00,$00,$00,$00
	DS.W 17
	DC.B $00,$03,$00,$00,$00,$00,$00,$00
	DS.W 77

	
nois_table	
	DC.L nois_BC-nois_table
	DC.L nois_BD-nois_table
	DC.L nois_BE-nois_table
	DC.L nois_BF-nois_table
	DC.L nois_C0-nois_table
	DC.L nois_C1-nois_table
	DC.L nois_C2-nois_table
	DC.L nois_C3-nois_table
	DC.L nois_C4-nois_table
	DC.L nois_C5-nois_table
	DC.L nois_C6-nois_table
	DC.L nois_C7-nois_table
	DC.L nois_C8-nois_table
	DC.L nois_C9-nois_table
	DC.L nois_CA-nois_table
	DC.L nois_CB-nois_table
	DC.L nois_CC-nois_table
	DC.L nois_CD-nois_table
	DC.L nois_CE-nois_table
	DC.L nois_CF-nois_table
	DC.L nois_D0-nois_table
	DC.L nois_D1-nois_table
	DC.L nois_D2-nois_table
	DC.L nois_D3-nois_table
	DC.L nois_D4-nois_table
	DC.L nois_D5-nois_table
	DC.L nois_D6-nois_table
	DC.L nois_D7-nois_table
	DC.L nois_D8-nois_table
	DC.L nois_D9-nois_table
	DC.L nois_DA-nois_table
	DC.L nois_DB-nois_table
	DC.L nois_DC-nois_table
	DC.L nois_DD-nois_table
	DC.L nois_DE-nois_table
	DC.L nois_DF-nois_table
	DC.L nois_E0-nois_table
	DC.L nois_E1-nois_table
	DC.L nois_E2-nois_table
	DC.L nois_E3-nois_table
	DC.L nois_E4-nois_table
	DC.L nois_E5-nois_table
	DC.L nois_E6-nois_table
	DC.L nois_E7-nois_table
	DC.L nois_E8-nois_table
	DC.L nois_E9-nois_table
	DC.L nois_EA-nois_table
	
tone_table:
	DC.L tone_EB-tone_table
	DC.L tone_EC-tone_table
	DC.L tone_ED-tone_table
	DC.L tone_EE-tone_table
	DC.L tone_EF-tone_table
	DC.L tone_F0-tone_table
	DC.L tone_F1-tone_table
	DC.L tone_F2-tone_table
	DC.L tone_F3-tone_table
	DC.L tone_F4-tone_table
	DC.L tone_F5-tone_table
	DC.L tone_F6-tone_table
	DC.L tone_F7-tone_table
	DC.L tone_F8-tone_table
	DC.L tone_F9-tone_table
	DC.L tone_FA-tone_table
	DC.L tone_FB-tone_table
	DC.L tone_FC-tone_table
	DC.L tone_FD-tone_table
	DC.L tone_FE-tone_table
	DC.L tone_FF-tone_table
	DC.L tone_00-tone_table
	DC.L tone_01-tone_table
	DC.L tone_02-tone_table
	DC.L tone_03-tone_table
	DC.L tone_04-tone_table
	DC.L tone_05-tone_table
	DC.L tone_06-tone_table
	DC.L tone_07-tone_table
	DC.L tone_08-tone_table
	DC.L tone_09-tone_table
	DC.L tone_0A-tone_table
	DC.L tone_0B-tone_table
	DC.L tone_0C-tone_table
	
wave_table:
	DC.L wave_0D-wave_table
	DC.L wave_0E-wave_table
	DC.L wave_0F-wave_table
	DC.L wave_10-wave_table
	DC.L wave_11-wave_table
	DC.L wave_12-wave_table
	DC.L wave_13-wave_table
	DC.L wave_14-wave_table
	DC.L wave_15-wave_table
	DC.L wave_16-wave_table
	DC.L wave_17-wave_table
	DC.L wave_18-wave_table
	DC.L wave_19-wave_table
	DC.L wave_1A-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table
	DC.L wave_1B-wave_table

	;; Noise sequences
	
nois_BC:	DC.B $10,$FF
nois_BD:	DC.B $0A,$11,$0F,$11,$0D,$0C,$08,$FF
nois_BE:	DC.B $04,$FF
nois_BF:	DC.B $03,$FF
nois_C0:	DC.B $20,$20,$FF
nois_C1:	DC.B $09,$20,$20,$FF
nois_C2:	DC.B $20,$09,$0C,$20,$FF
nois_C3:	DC.B $20,$04,$08,$0E,$12,$16,$1A,$1E,$FE
nois_C4:	DC.B $20,$FE
nois_C5:	DC.B $20,$FE
nois_C6:	DC.B $11,$FE
nois_C7:	DC.B $20,$08,$FE
nois_C8:	DC.B $0D,$09,$FE
nois_C9:	DC.B $13,$0E,$0C,$0B,$04,$FE
nois_CA:	DC.B $20,$20,$19,$0A,$05,$03,$02,$01,$FE
nois_CB:	DC.B $01,$02,$03,$04,$05,$06,$07,$08
		DC.B $07,$06,$05,$04,$03,$02,$01,$FE
nois_CC:	DC.B $0D,$03,$0D,$05,$0D,$11,$06,$11
		DC.B $06,$FE
nois_CD:	DC.B $0E,$15,$FE
nois_CE:	DC.B $0F,$18,$FE
nois_CF:	DC.B $18,$FF
nois_D0:	DC.B $06,$15,$0C,$15,$07,$15,$04,$15
		DC.B $08,$FF
nois_D1:	DC.B $20,$FF
nois_D2:	DC.B $0F,$16,$14,$16,$12,$11,$0D,$FF
nois_D3:	DC.B $20,$20,$18,$10,$08,$FF
nois_D4:	DC.B $14,$1B,$19,$1B,$17,$16,$12,$FF
nois_D5:	DC.B '0',$08,$FF
nois_D6:	DC.B '/',$05,'/',$03,";",$01,$FF
nois_D7:	DC.B $08,$10,'P',$FF
nois_D8:	DC.B $14,$12,$10,$0E,$0C,$0A,$08,$FF
nois_D9:	DC.B $12,$08,$FF
nois_DA:	DC.B $16,$12,"(+;)(",$FF
nois_DB:	DC.B $04,$FF
nois_DC:	DC.B $02,$12,'1',$01,'%',$05,'6',$06,$FF
nois_DD:	DC.B $0A,$0B,$0C,$0D,$0E,$0F,$FF
nois_DE:	DC.B ')',$01,$FF
nois_DF:	DC.B $0B,$FF
nois_E0:	DC.B $0A,$00,$00,$00,$0E,$04,$04,$FF
nois_E1:	DC.B '+',$FF
nois_E2:	DC.B $15,$02,$FF
nois_E3:	DC.B $0C,$01,$02,$01,$FF
nois_E4:	DC.B $0D,$0D,$0D,$01,$03,$FF
nois_E5:	DC.B $01,$FF
nois_E6:	DC.B $14,$04,$0A,$02,$FF
nois_E7:	DC.B $0C,$01,$04,$0E,$04,$10,$04,$12,$FF
nois_E8:	DC.B $00,$00,$09,$FF
nois_E9:	DC.B $00,$00,$09,$0C,$FF
nois_EA:	DC.B $00,$00,$00,$01,$01,$01,$00,$00,$01
		DC.B $01,$00,$01,$FF
tone_EB:	DC.B $01,$02,$FE
tone_EC:	DC.B $03,$02,$02,$02,$01,$01,$FE
tone_ED:	DC.B $03,$02,$FE
tone_EE:	DC.B $07,$04,$04,$04,$02,$02,$FF
tone_EF:	DC.B $01,$00,$00,$00,$01,$00,$0E,$0F
		DC.B $0F,$0E,$0C,$08,$00,$FE
tone_F0:	DC.B $00,$02,$02,$02,$02,$03,$00,$0E
		DC.B $0D,$0C,$FE
tone_F1:	DC.B $03,$00,$0E,$00,$0D,$0B,$09,$07,$05
		DC.B $03,$01,$00,$FE
tone_F2:	DC.B $02,$0E,$00,$00,$02,$02,$00,$01
		DC.B $10,$FE
tone_F3:	DC.B $0C,$04,$00,$00,$0C,$0D,$FE
tone_F4:	DC.B $02,$03,$03,$00,$00,$0C,$0D,$FE
tone_F5:	DC.B $01,$00,$00,$00,$02,$00,$0F,$0F,$0E
		DC.B $0D,$0C,$0C,$00,$FE
tone_F6:	DC.B $06,$00,$00,$00,$00,$FE
tone_F7:	DC.B $19,$14,$0F,$0A,$05,$00,$FE
tone_F8:	DC.B $00,$01,$00,$07,$0C,$FE
tone_F9:	DC.B $00,$00,$18,$0C,$00,$FE
tone_FA:	DC.B $14,$0A,$00,$FE
tone_FB:	DC.B $03,$04,$FE
tone_FC:	DC.B $08,$05,$05,$05,$05,$05,$05,$04,$04
		DC.B $04,$03,$03,$02,$02,$01,$FE
tone_FD:	DC.B $08,$07,$FE
tone_FE:	DC.B $05,$04,$03,$02,$01,$02,$03,$04
		DC.B $05,$06,$07,$08,$09,$0A,$FE
tone_FF:	DC.B $08,$0A,$FE
tone_00:	DC.B $09,$08,$08,$08,$08,$08,$08,$08
		DC.B $07,$07,$07,$06,$06,$05,$FE
tone_01:	DC.B $09,$06,$FE
tone_02:	DC.B $0A,$0B,$0C,$0D,$0E,$0F,$FE
tone_03:	DC.B $0B,$0C,$0D,$0E,$0F,$FE
tone_04:	DC.B $01,$0B,$01,$0C,$01,$0D,$FE
tone_05:	DC.B '(',$04,'(',$04,'(',$04,'(',$FE
tone_06:	DC.B $01,$02,$03,$04,$05,$06,$07,$08
		DC.B $09,$0A,$09,$08,$07,$06,$05,$04
		DC.B $03,$02,$01,$FE
tone_07:	DC.B $09,$08,$07,$06,$05,$04,$03,$02
		DC.B $01,$02,$03,$04,$05,$06,$07,$08
		DC.B $09,$FE
tone_08:	DC.B $0A,$00,$09,$00,$08,$00,$07,$00
		DC.B $06,$00,$05,$00,$04,$00,$03,$00
		DC.B $02,$00,$01,$00,$FE
tone_09:	DC.B $14,$12,$10,$0E,$0C,$0A,$08,$07,$06
		DC.B $05,$04,$03,$02,$01,$FE
tone_0A:	DC.B $20,$01,$10,$02,$FE
tone_0B:	DC.B $0A,$14,$00,$14,$0A,$FE
tone_0C:	DC.B $0A,$05,$FE

wave_0D:	DC.B $01,$01,$01,$FF
wave_0E:	DC.B $01,$01,$FF
wave_0F:	DC.B $01,$FF
wave_10:	DC.B $01,$01,$01,$01,$FE
wave_11:	DC.B $0A,$0B,$0C,$0D,$0E,$0F,$FE
wave_12:	DC.B $01,$01,$01,$01,$01,$FE
wave_13:	DC.B $01,$01,$01,$01,$01,$01,$FE
wave_14:	DC.B '(((((((',$FE
wave_15:	DC.B $01,$01,$01,$01,$01,$01,$01,$01
		DC.B $FE
wave_16:	DC.B $01,$02,$01,$02,$01,$02,$01,$02,$FE
wave_17:	DC.B $02,$01,$02,$01,$02,$01,$02,$01
		DC.B $FE
wave_18:	DC.B $01,$03,$01,$03,$01,$03,$01,$03,$FE
wave_19:	DC.B $03,$01,$03,$01,$03,$01,$03,$01,$FE
wave_1A:	DC.B $01,$04,$01,$04,$01,$04,$01,$04,$FE
wave_1B:	DC.B $01,$01,$04,$01,$04,$01,$04,$01
		DC.B $FE,$01,$01,$04,$01,$04,$01,$04
		DC.B $01,$FE,$01,$01,$04,$01,$04,$01
		DC.B $04,$01,$FE,$01,$01,$04,$01,$04
		DC.B $01,$04,$01,$FE

trem_table:	DC.B $00,$00,$01,$AC,$03,'T',$04,$F2
		DC.B $06,$82,$08,$00,$09,'h',$0A,$B5
		DC.B $0B,$E4,$0C,$F2,$0D,$DB,$0E,$9E
		DC.B $0F,'8',$0F,$A6,$0F,$EA,$10,$00
		DC.B $0F,$EA,$0F,$A6,$0F,'8',$0E,$9E
		DC.B $0D,$DB,$0C,$F2,$0B,$E4,$0A,$B5
		DC.B $09,'h',$08,$00,$06,$82,$04,$F2
		DC.B $03,'T',$01,$AC,$00,$00,$FE,'T'
		DC.B $FC,$AC,$FB,$0E,$F9,'~',$F8,$00
		DC.B $F6,$98,$F5,'K',$F4,$1C,$F3,$0E
		DC.B $F2,'%',$F1,'b',$F0,$C8,$F0,'Z'
		DC.B $F0,$16,$F0,$00,$F0,$16,$F0,'Z'
		DC.B $F0,$C8,$F1,'b',$F2,'%',$F3,$0E
		DC.B $F4,$1C,$F5,'K',$F6,$98,$F8,$00
		DC.B $F9,'~',$FB,$0E,$FC,$AC,$FE,'T'

merk_d0:	DC.B $00,$00
merk_d1:	DC.B $00,$00
offset:		DC.B $00,$00
PatBreakFlag:	DC.B $00,$00	; Pattern Break command
what:		DC.B $00,$00	; Speed change flag
merk_speed:	DC.B $00,$00	; Speed merken
	
vols_y:	DS.W 12
	DC.B $01,$01,$01,$01,$01,$01,$01,$01
	DS.W 2
	DC.B $01,$01,$01,$01,$01,$01,$01,$01
	DC.B $02,$02,$02,$02,$00,$00,$00,$01
	DC.B $01,$01,$01,$01,$02,$02,$02,$02
	DC.B $02,$03,$03,$03,$00,$00,$01,$01
	DC.B $01,$01,$02,$02,$02,$02,$03,$03
	DC.B $03,$03,$04,$04,$00,$00,$01,$01
	DC.B $01,$02,$02,$02,$03,$03,$03,$04
	DC.B $04,$04,$05,$05,$00,$00,$01,$01
	DC.B $02,$02,$02,$03,$03,$04,$04,$04
	DC.B $05,$05,$06,$06,$00,$00,$01,$01
	DC.B $02,$02,$03,$03,$04,$04,$05,$05
	DC.B $06,$06,$07,$07,$00,$01,$01,$02
	DC.B $02,$03,$03,$04,$04,$05,$05,$06
	DC.B $06,$07,$07,$08,$00,$01,$01,$02
	DC.B $02,$03,$04,$04,$05,$05,$06,$07
	DC.B $07,$08,$08,$09,$00,$01,$01,$02
	DC.B $03,$03,$04,$05,$05,$06,$07,$07
	DC.B $08,$08,$09,$0A,$00,$01,$01,$02
	DC.B $03,$04,$04,$05,$06,$07,$07,$08
	DC.B $09,$0A,$0A,$0B,$00,$01,$02,$02
	DC.B $03,$04,$05,$06,$06,$07,$08,$09
	DC.B $0A,$0A,$0B,$0C,$00,$01,$02,$03
	DC.B $03,$04,$05,$06,$07,$08,$09,$0A
	DC.B $0A,$0B,$0C,$0D,$00,$01,$02,$03
	DC.B $04,$05,$06,$07,$07,$08,$09,$0A
	DC.B $0B,$0C,$0D,$0E,$00,$01,$02,$03
	DC.B $04,$05,$06,$07,$08,$09,$0A,$0B
	DC.B $0C,$0D,$0E,$0F,$FF,$00
	
note_2_freq:
	DC.W $0FD1,$0EEE,$0E17,$0D4D,$0CBE,$0BD9,$0B2F,$0A8E
	DC.W $09F7,$0967,$08E0,$0861
	DC.W $07E8,$0777,$070C,$06A7,$0647,$05ED,$0598,$0547
	DC.W $04FC,$04B4,$0470,$0431
	DC.W $03F4,$03BB,$0385,$0353,$0323,$02F6,$02CB,$02A3
	DC.W $027D,$0259,$0238,$0218
	DC.W $01FA,$01DD,$01C2,$01A9,$0191,$017B,$0165,$0151
	DC.W $013E,$012C,$011C,$010C
	DC.W $00FD,$00EE,$00E1,$00D4,$00C8,$00BD,$00B2,$00A8
	DC.W $009F,$0096,$008E,$0086
	DC.W $007E,$0077,$0070,$006A,$0064,$005E,$0059,$0054
	DC.W $004F,$004B,$0047,$0043
	DC.W $003F,$003B,$0038,$0035,$0032,$002F,$002C,$003B
	DC.W $0027,$0025,$0023,$0021
	DC.W $001F,$001D,$001C,$001A,$0019,$0017,$0016,$0015
	DC.W $0013,$0012,$0011,$0010
	DC.W $000F,$000E,$000D,$000C,$000B,$000A,$0009,$0008
	DC.W $0007,$0006,$0005,$0004,$0003,$0002,$0001,$0000
	DC.W $0000

;;; $$$ ben: starting to reverse that voice struct :)
;;; 
;;; +0.W tone period
;;; +14.b current volume

;;; +43.b direct noise period
;;; +58.b flags [bit-1 desactive chan A]
;;; +60.b direct noise control (0:off)
;;; +61.b flag, affect tone-mixer (!0:force tone ON (may be sid))
;;; +78.l mask from mask table
;;; +79.b sound-flag (0:tone-off)

;;; +64.l pointer to noise sequence
;;; 
;;; 
;;; +72.l pointer to some sequence

;;; +44.b voice-flags
;;;	bit-0:tone
;;;	bit-1:noise
;;;	bit-2:involved in env control (0:no-env control)
;;;	bit-3:idem
;;; +78.b yet another noise affect flag [0:no noise but sequence read]
	
		rsreset
vc_dummy:	rs.b	82
vc_size:	rs.b	0

VoiceData_B:	DS.B	vc_size ;43-2	;Playdata
VoiceData_C:	DS.B	vc_size ;43-2	;Playdata
VoiceData_A:	DS.B	vc_size ;43-2	;Playdata

CurInfo:	DS.W	126	; $$$ ben: init copies 32*8 = 256 bytes !
CurSongPos:	DC.W	0
CurPatNum:	DC.W	0
PatPtrTbl:	DS.L	120	; $$$ ben: should be 128 ?
CurPatPtr:	DC.L	0
PatPos:		DC.W	0
PatPtr		DC.L	0
L012E:		DS.W	5
note_count:	DC.W 	0
	
;;; ben: added to remove self modified code and make PCR
ReNoteCntPtr:	DC.L	0
WavFormPtr:	DC.L	0
SpeedPtr:	DC.L	0
SMCA0:		DC.L	0
SMC9E:		DC.L	0
RePatPtr:	DC.L	0
ReCurPatPtr:	DC.L	0
SongBasePtr:	DC.L	0
SongLenPtr:	DC.L	0
SongRePtr:	DC.L	0
InstBasePtr:	DC.L	0
SoundDataPtr:	DC.L	0	; sound
VoiceSetPtr:	DC.L	0	; vset
