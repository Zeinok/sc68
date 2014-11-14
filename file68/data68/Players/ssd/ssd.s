;;; -*- asm -*-
;;;
;;; Animal Mine SidSoundDesigner Replayer by MC
;;;
;;; 1.00 reassembled,fixed and new stuff added by .defjam./.checkpoint.
;;; 1.01 Falcon-IDE-fix by FroST
;;; 1.02 Pattern fix, Optimisation, comments by Ben/OVR
;;; 1.03 Portamento fix, additional comments by Grazey/PHF
;;; 1.04 Position Independant Code (PIC) AKA PC Relative (PCR)
;;; 1.05 Structure reverse
;;;
;;; Tabs to 8 (standard)

        opt     a+,p+           ; Auto PCR, Check for PCR
        near    a4              ; Relocate a4 displacement

        opt     o-              ; Don't opimize the jump table

;;; ben: TODO: re-write the header to match the standard one, remove the
;;;      function we really don't care about. Keep it simple !
;;;

music:  bra.w   init_music      ; +0 a0:tvs a1:tri d0:'ABDC'
        bra.w   my_replay_music ; +4
        bra.w   exit_player     ; +8

        opt     o-              ; $$$ CHANGE ME Optimize from now on

lall:   dc.b    "Sid Sound Designer by MC of Animal Mine 93-95 "
        dc.b    "ver. 1.04 ABD "
        dc.b    "fixed by .defjam./.checkpoint. "
        dc.b    "Falcon fix by FroST "
        dc.b    "Pattern Break fix by Ben/OVR "
        dc.b    "Portamento fix by Grazey/PHF"
        dc.b    "PCR by Ben/OVR "
        dc.b    "Runtime timer selection by Ben/OVR "
        even

;; vcVol                  SET $0E ;; 14
;; vcInsTyp              SET $2C ;; 44
;; vcHwBuzz              SET $3A ;; 58

;; vcXtFlags:	SET $3C		; 60 Copied from 'INFO' block
;; vcXtNoiFlag:	SET vcXtFlags+0	;; 60 | 
;; vcXtTonFlag:	SET vcXtFlags+1 ;; 61 |
;; vcXtWavFlag:	SET vcXtFlags+2 ;; 62 |
;; vcTFMX:		SET vcXtFlags+3 ;; 63 |
	
;; vcXtNoiPtr:	SET $40 ;; 64
;; vcXtTonPtr:	SET $44 ;; 68
;; vcXtWavPtr:	SET $48 ;; 72
;; vcXtSets:	SET $4E ;; 78
;; vcXtTonSet:	SET vcXtSets+0
;; vcXtNoiset:	SET vcXtSets+1
;; vcXtBuzSet:	SET vcXtSets+2

my_replay_music:
        movem.l d0-a6,-(a7)
        lea     music(pc),a4
        tst.b   ReplayFlag(a4)
        beq.s   .off
        bsr     replay_music
.off:
        movem.l (a7)+,d0-A6
        rts


exit_player:
        movem.l d0-a6,-(a7)
        lea     music(pc),a4
        bsr     restore_MFP
        bsr     restore_h200
        bsr     clear_YM
        movem.l (a7)+,d0-a6
        rts

save_MFP:
        move    #$2700,SR
        lea     mfp_regs(pc),a0
        move.b  $FFFFFA07.w,(a0)+ ;IERA
        move.b  $FFFFFA09.w,(a0)+ ;IERB
        move.b  $FFFFFA13.w,(a0)+ ;IMRA
        move.b  $FFFFFA15.w,(a0)+ ;IMRB

        move.b  $FFFFFA17.w,(a0)+ ;Interrupt Vektor Register

        move.b  $FFFFFA19.w,(a0)+ ;Timer A Control
        move.b  $FFFFFA1B.w,(a0)+ ;Timer B Control
        move.b  $FFFFFA1D.w,(a0)+ ;Timer C/D Control

        bclr    #5,$FFFFFA07.w  ;Timer A
        bclr    #0,$FFFFFA07.w  ;Timer B
        bclr    #4,$FFFFFA09.w  ;Timer D

        bclr    #5,$FFFFFA13.w  ;Timer A
        bclr    #0,$FFFFFA13.w  ;Timer B
        bclr    #4,$FFFFFA15.w  ;Timer D

        lea     irq_vectors(pc),a0
        move.l  $00000120.w,(a0)+ ;Timer B
        move.l  $00000110.w,(a0)+ ;Timer D
        move.l  $00000134.w,(a0)+ ;Timer A
        move.l  $00000060.w,(a0)+ ;Spurious irq

        bclr    #3,$FFFFFA17.w  ; software end of int.
        lea     VoidIrq(pc),a0
        move.l  a0,$00000060.w
        move.l  a0,$00000110.w
        move.l  a0,$00000120.w
        move.l  a0,$00000134.w
        move    #$2300,SR
        rts

mfp_regs:       ds.b    16
irq_vectors:    dc.l    0,0,0,0,0,0,0,0

restore_MFP:
        move    #$2700,SR

;Stop All Timers
        bclr    #5,$FFFFFA07.w  ;Timer A
        bclr    #0,$FFFFFA07.w  ;Timer B
        bclr    #4,$FFFFFA09.w  ;Timer D

        bclr    #5,$FFFFFA13.w  ;Timer A
        bclr    #0,$FFFFFA13.w  ;Timer B
        bclr    #4,$FFFFFA15.w  ;Timer D

        lea     mfp_regs(pc),a0
        move.b  (a0)+,$FFFFFA07.w       ;IERA
        move.b  (a0)+,$FFFFFA09.w       ;IERB
        move.b  (a0)+,$FFFFFA13.w       ;IMRA
        move.b  (a0)+,$FFFFFA15.w       ;IMRB

        move.b  (a0)+,$FFFFFA17.w       ;Interrupt Vektor Register

        move.b  (a0)+,$FFFFFA19.w       ;Timer A Control
        move.b  (a0)+,$FFFFFA1B.w       ;Timer B Control

        move.l  d0,-(sp)
        move.b  (a0)+,d0
        and.b   #%11110000,$FFFFFA1D.w  ;Timer C/D Control (only TD bits)
        and.b   #7,d0
        or.b    d0,$fffffa1d.w
        move.l  (sp)+,d0

        move.b  #$C0,$FFFFFA21.w        ;Timer B Data

        lea     irq_vectors(pc),a0
        move.l  (a0)+,$00000120.w       ;Timer B
        move.l  (a0)+,$00000110.w       ;Timer D
        move.l  (a0)+,$00000134.w       ;Timer A
        move.l  (a0)+,$00000060.w       ;Spurious irq

        move    #$2300,SR
        rts

set_sid_on:
        lea     music(pc),a4
        move    #$2700,SR
        pea     VoidIrq(pc)
        move.l  (a7)+,$00000060.w
        bsr     copy_sid_psg_rout

        move.l  #td0,$00000110.w        ;Timer D
        andi.b  #$F0,$FFFFFA1D.w
        ori.b   #1,$FFFFFA1D.w
        move.b  #255,$FFFFFA25.w        ;data
        bset    #4,$FFFFFA15.w
        bclr    #4,$FFFFFA09.w

        move.l  #tb0,$120.w             ;Timer B
        move.b  #%11,$fffffa1b.w
        move.b  #255,$fffffa21.w        ;data
        bset    #0,$fffffa13.w
        bclr    #0,$fffffa07.w

        move.l  #ta0,$00000134.w        ;Timer A
        clr.b   $FFFFFA19.w
        move.b  #255,$FFFFFA1F.w        ;data
        bset    #5,$FFFFFA13.w
        bclr    #5,$FFFFFA07.w

        bclr    #3,$FFFFFA17.w          ; software end of int.
        move    #$2300,SR
        rts

set_sid_off:
        lea     music(pc),a4
        move    #$2700,SR

        lea     VoidIrq(pc),a0
        move.l  a0,$00000060.w
        move.l  a0,$00000110.w          ;Timer D
        and.b   #%11110000,$fffffa1d.w  ;Preserve Timer C bits!
        move.b  #255,$FFFFFA25.w
        bclr    #4,$FFFFFA15.w
        bclr    #4,$FFFFFA09.w

        move.l  a0,$120.w               ;Timer B
        clr.b   $fffffa1b.w
        move.b  #255,$fffffa21.w
        bclr    #0,$fffffa07.w
        bclr    #0,$fffffa13.w

        move.l  a0,$00000134.w          ;Timer A
        clr.b   $FFFFFA19.w
        move.b  #255,$FFFFFA1F.w
        bclr    #5,$FFFFFA13.w
        bclr    #5,$FFFFFA07.w
        move    #$2300,SR
        rts


save_h200:
        move    #$2700,SR
        lea     $00000200.w,a0
        lea     save200(pc),a1
do_s2:  move.w  #$0100/4-1,d0
sc0:    move.l  (a0)+,(a1)+
        dbra    d0,sc0
        move    #$2300,SR
        rts

restore_h200:
        move    #$2700,SR
        lea     $00000200.w,a1
        lea     save200(pc),a0
        bra.s   do_s2

save200:
        DS.B    $0100


copy_sid_psg_rout:
        lea     sid_irqs_s(pc),a0
        lea     $00000200.w,a1
        move.w  #sid_irqs_e-sid_irqs_s-1,d0
c0:     move.b  (a0)+,(a1)+
        dbra    d0,c0
        rts


clear_YM:                       ; Frost Fix / Ben Fix
        move.l  a5,-(a7)
        move.w  d0,-(a7)
        move    sr,-(a7)

        lea     $ffff8800.w,a5
        clr     d0
        or      #$700,sr        ; Do not distrub !

        ;; Volumes to 0
        move.b  #$8,(a5)
        move.b  d0,2(a5)
        move.b  #$9,(a5)
        move.b  d0,2(a5)
        move.b  #$a,(a5)
        move.b  d0,2(a5)

;;; Never modify bit 6 on a Falcon, coz bit 7 in YM's port A means
;;; "Falcon internal IDE drive on/off", changing bit 6 in reg 7 will
;;; provide a direction change of the flow from port A (do you follow
;;; me, coz I don't know how to explain in english).
;;;

;;; ben: WTF was that ? It's much more simple: simply don't assume one
;;; value over another, just *DONT MODIFY* the data port direction bits.

        ;; Deal with register 7
        move.b  #7,(a5)
        move.b  (a5),d0
        and.b   #%11000000,d0   ; Keep data port direction bits
        or.b    #%00111111,d0   ; Cutoff A/B/C tone and noise
        move.b  d0,2(a5)

;;; Ben: Everything else was totally useless and has been removed
        move    (a7)+,sr
        move.w  (a7)+,d0
        move.l  (a7)+,a5
        rts

replay_music:
        moveq   #0,d4
        lea     $ffff8800.w,a5
        subq.w  #1,NoteCount(a4)
        bne.s   do_fx_all

        movea.l ReNoteCntPtr(pc),a1
        move.w  (a1),NoteCount(a4)

        movea.l CurPatPtr(pc),a1
        move.w  PatPos(pc),d0
        move.w  d0,d1
        lsl.w   #3,d0
        lsl.w   #2,d1   ;       mul 12

        add.w   d1,d0
        adda.w  d0,a1
        move.l  a1,PatPtr(a4)
        tst.b   PatBreakFlag(a4)
        bpl.s   L0004
        move    #$40,PatPos(a4) ; $$$ ben: pattern break fix
        sf      PatBreakFlag(a4)
        bra.s   L0005

L0004:  addq.w  #1,PatPos(a4)
L0005:  cmpi.w  #$0040,PatPos(a4)       ; $$$
        bne.s   L0006

        bsr     load_pos

L0006:  lea     VoiceA(pc),a0
        bsr     getnote

        lea     VoiceB(pc),a0
        bsr     getnote

        lea     VoiceC(pc),a0
        bsr     getnote

do_fx_all:
        lea     VoiceB(pc),a0
        bsr     do_fx
        bsr     do_tone_control
        bsr     do_wave_control

        lea     VoiceC(pc),a0
        bsr     do_fx
        bsr     do_tone_control
        bsr     do_wave_control

        lea     VoiceA(pc),a0
        bsr     do_fx
        bsr     do_tone_control
        bsr     do_wave_control


        ;; DO sid stuff

        move    #$2700,SR


        lea     VoiceA(pc),a0
        tst.b   vcHwBuzz(a0)
        bne.s   no_wow_a

        tst.w   SidOnOffA(a4)
        bpl.s   no_wow_a
        cmpi.b  #5,vcTFMX(a0)
        bgt.s   no_wow_a
        cmpi.b  #3,vcTFMX(a0)
        blt.s   no_wow_a
        lea     wave_form1(pc),a6
        move.w  0(a0),d1
scan_more_a:
	tst.w   (a6)
        bmi.s   no_wow_a
        cmp.w   (a6)+,d1
        blt.s   do_tfm_a
        addq.l  #4,a6
        bra.s   scan_more_a

do_tfm_a:
	move.w  (a6)+,d2
        move.w  (a6)+,d3
        move.l  #26214400,d0	; ben: FIXME: MFP clock
        mulu    #$00A3,d2
        divu    d2,d0
        mulu    d1,d0
        tst.w   SidDephaseA(a4)
        bne.s   nix_dual_a
        addi.l  #$00002000,d0
nix_dual_a:
	addi.l  #$00002000,d0
        add.l   d0,d0
        add.l   d0,d0
        swap    D0
        move.b  vcVol(a0),tb1+4.w

        move.b  d0,$fffffa21.w
        move.b  d3,$fffffa1b.w
        or.b    #1,$fffffa07.w
        bra.s   DoSidVoiceB

no_wow_a:
        bclr    #0,$FFFFFA07.w

        cmpi.b  #5,vcTFMX(a0)
        bgt.s   DoSidVoiceB
        cmpi.b  #3,vcTFMX(a0)
        blt.s   DoSidVoiceB
        bclr    #0,vcInsTyp(a0)

        ;; Sid Voice B

DoSidVoiceB:
        lea     VoiceB(pc),a0
        tst.b   vcHwBuzz(a0)
        bne.s   no_wow

        tst.w   SidOnOffB(a4)
        bpl.s   no_wow
        cmpi.b  #5,vcTFMX(a0)
        bgt.s   no_wow
        cmpi.b  #3,vcTFMX(a0)
        blt.s   no_wow
        move.l  WavFormPtr(pc),a6
        move.w  0(a0),d1
scan_more:
        tst.w   (a6)
        bmi.s   no_wow
        cmp.w   (a6)+,d1
        blt.s   do_tfm
        addq.l  #4,a6
        bra.s   scan_more
do_tfm: move.w  (a6)+,d2
        move.w  (a6)+,d3
        move.l  #$01900000,d0
        mulu    #$00A3,d2
        divu    d2,d0
        mulu    d1,d0
        tst.w   SidDephaseB(a4)
        bne.s   nix_dual_b
        addi.l  #$00002000,d0
nix_dual_b:     addi.l  #$00002000,d0
        lsl.l   #2,d0
        swap    D0
        move.b  vcVol(a0),ta1+4.w

        move.b  d0,$FFFFFA1F.w
        move.b  d3,$FFFFFA19.w
        bset    #5,$FFFFFA07.w
        bra.s   DoSidVoiceC

no_wow: bclr    #5,$FFFFFA07.w
        cmpi.b  #5,vcTFMX(a0)
        bgt.s   DoSidVoiceC
        cmpi.b  #3,vcTFMX(a0)
        blt.s   DoSidVoiceC
        bclr    #0,vcInsTyp(a0)


DoSidVoiceC:
        lea     VoiceC(pc),a0
        tst.b   vcHwBuzz(a0)
        bne.s   no_wow_c
        tst.w   SidOnOffC(a4)
        bpl.s   no_wow_c
        cmpi.b  #5,vcTFMX(a0)
        bgt.s   no_wow_c
        cmpi.b  #3,vcTFMX(a0)
        blt.s   no_wow_c
        lea     wave_form1(pc),a6
        move.w  0(a0),d1
scan_more_c:
	tst.w   (a6)
        bmi.s   no_wow_c
        cmp.w   (a6)+,d1
        blt.s   do_tfm_c
        addq.l  #4,a6
        bra.s   scan_more_c
do_tfm_c:       move.w  (a6)+,d2
        move.w  (a6)+,d3
        move.l  #$01900000,d0
        mulu    #$00A3,d2
        divu    d2,d0
        mulu    d1,d0
        tst.w   SidDephaseC(a4)
        bne.s   nix_dual_c
        addi.l  #$00002000,d0
nix_dual_c:     addi.l  #$00002000,d0
        lsl.l   #2,d0
        swap    D0
        move.b  vcVol(a0),td1+4.w

        andi.b  #$F0,$FFFFFA1D.w
        or.b    d3,$FFFFFA1D.w
        move.b  d0,$FFFFFA25.w
        bset    #4,$FFFFFA09.w
        bra.s   wow_c

no_wow_c:       bclr    #4,$FFFFFA09.w
        cmpi.b  #5,vcTFMX(a0)
        bgt.s   wow_c
        cmpi.b  #3,vcTFMX(a0)
        blt.s   wow_c
        bclr    #0,vcInsTyp(a0)

wow_c:  move    #$2300,SR
        move.b  #$FF,d0 ; All OFF

        lea     VoiceC(pc),a0
        moveq   #2,d6
        moveq   #5,d7
        bsr     SetMixerAndNoise

        lea     VoiceB(pc),a0
        moveq   #1,d6
        moveq   #4,d7
        bsr     SetMixerAndNoise

        lea     VoiceA(pc),a0
        btst    #1,vcHwBuzz(a0)
        bne.s   SetMixer
        moveq   #0,d6
        moveq   #3,d7
        bsr     SetMixerAndNoise

SetMixer:
        move.w  #$0700,d7
        move.b  d0,d7
        movep.w d7,0(a5)

SetVoiceA:
        lea     VoiceA(pc),a0
        btst    #1,vcHwBuzz(a0)
        bne.s   SetVoiceB

        ;; Set voice A period
        move.w  0(a0),d0
        move.b  0(a0),d1
        moveq   #0,d7
        move.b  d0,d7
        movep.w d7,0(a5)
        move.w  #$0100,d7
        move.b  d1,d7
        movep.w d7,0(a5)

        tst.b   vcHwBuzz(a0)
        bne.s   no_hardw_auto_a
        tst.b   vcXtBuzSet(a0)
        beq.s   no_hardw_auto_a
        btst    #2,vcInsTyp(a0)
        beq.s   no_hardw_auto_a
        btst    #3,vcInsTyp(a0)
        beq.s   no_hardw_auto_a

        ;; Set envelop period maaped to channel A
        lsr.w   #4,d0
        move.w  #$0B00,d7
        move.b  d0,d7
        movep.w d7,0(a5)
        move.w  #$0C00,d7
        movep.w d7,0(a5)

no_hardw_auto_a:
        tst.w   SidOnOffA(a4)
        bpl.s   okay_a
        cmpi.b  #3,vcTFMX(a0)
        blt.s   okay_a
        cmpi.b  #5,vcTFMX(a0)
        bgt.s   okay_a
        bra.s   SetVoiceB

okay_a:
        ;; Set Voice A volume
        move.w  #$0800,d7
        move.b  vcVol(a0),d7
        movep.w d7,0(a5)

SetVoiceB:
        lea     VoiceB(pc),a0

        ;; Set voice B period
        move.w  0(a0),d0
        move.b  0(a0),d1
        move.w  #$0200,d7
        move.b  d0,d7
        movep.w d7,0(a5)
        move.w  #$0300,d7
        move.b  d1,d7
        movep.w d7,0(a5)

        tst.b   vcHwBuzz(a0)
        bne.s   no_hardw_auto_b
        tst.b   vcXtBuzSet(a0)
        beq.s   no_hardw_auto_b
        btst    #2,vcInsTyp(a0)
        beq.s   no_hardw_auto_b
        btst    #3,vcInsTyp(a0)
        beq.s   no_hardw_auto_b


        ;; Set envelop period to channel B
        lsr.w   #4,d0
        move.w  #$0B00,d7
        move.b  d0,d7
        movep.w d7,0(a5)
        move.w  #$0C00,d7
        movep.w d7,0(a5)

no_hardw_auto_b:
        tst.w   SidOnOffB(a4)
        bpl.s   okay_b
        cmpi.b  #3,vcTFMX(a0)
        blt.s   okay_b
        cmpi.b  #5,vcTFMX(a0)
        bgt.s   okay_b
        bra.s   SetVoiceC

okay_b:
        ;; Set Voice B volume
        move.w  #$0900,d7
        move.b  vcVol(a0),d7
        movep.w d7,0(a5)


SetVoiceC:
        lea     VoiceC(pc),a0
        ;; Set voice B period

        move.w  0(a0),d0
        move.b  0(a0),d1
        move.w  #$0400,d7
        move.b  d0,d7
        movep.w d7,0(a5)
        move.w  #$0500,d7
        move.b  d1,d7
        movep.w d7,0(a5)
        tst.b   vcHwBuzz(a0)
        bne.s   no_hardw_auto_c
        tst.b   vcXtBuzSet(a0)
        beq.s   no_hardw_auto_c
        btst    #2,vcInsTyp(a0)
        beq.s   no_hardw_auto_c
        btst    #3,vcInsTyp(a0)
        beq.s   no_hardw_auto_c
        lsr.w   #4,d0
        move.w  #$0B00,d7
        move.b  d0,d7
        movep.w d7,0(a5)
        move.w  #$0C00,d7
        movep.w         d7,0(a5)
no_hardw_auto_c:
        tst.w   SidOnOffC(a4)
        bpl.s   okay_c
        cmpi.b  #3,vcTFMX(a0)
        blt.s   okay_c
        cmpi.b  #5,vcTFMX(a0)
        bgt.s   okay_c
        bra.s   no_c
okay_c:
        ;; Set Voice B volume
        move.w  #$0A00,d7
        move.b  vcVol(a0),d7
        movep.w d7,0(a5)

no_c:   rts


; IN    a0      Voice struct
;       a1      Pattern Ptr
;

getnote:
        tst.b   (a1)
        beq     test_env
        bmi     digi_
        move.b  2(a1),d1
        cmpi.b  #$3F,d1
        bhi     same_instr
        move.b  d1,vcInstNum(a0)
        move.w  d4,vcArpIdx(a0)
        move.b  d4,vcSpecVal(a0)
        move.l  CurInfo(pc),a6
        ;; lea  BufInfo(pc),a6
        moveq   #0,d0
        move.b  vcInstNum(a0),d0
        add.w   d0,d0
        add.w   d0,d0
        move.l  0(a6,d0.w),d0 	; 60616263
        move.l  d0,vcXtFlags(a0)
        lsr.l   #8,d0		; ..606162
        tst.b   D0
        beq.s   no_wave_control
        lea     wave_table(pc),a6
        moveq   #0,d1
        move.b  d0,d1
        subq.b  #1,d1
        add.w   d1,d1
        adda.w  0(a6,d1.w),a6
        move.l  a6,vcXtWavPtr(a0)

no_wave_control:
        lsr.l   #8,d0		; ....6061
        tst.b   D0
        beq.s   no_tone_control
        lea     tone_table(pc),a6
        moveq   #0,d1
        move.b  d0,d1
        subq.b  #1,d1
        add.w   d1,d1
        adda.w  0(a6,d1.w),a6
        move.l  a6,vcXtTonPtr(a0)

no_tone_control:
        lsr.w   #8,d0		; ......60
        tst.b   D0
        beq.s   no_noise_control
        lea     nois_table(pc),a6
        moveq   #0,d1
        move.b  d0,d1
        subq.w  #1,d1
        add.w   d1,d1
        adda.w  0(a6,d1.w),a6
        move.l  a6,vcXtNoiPtr(a0)

no_noise_control:
        move.b  vcInstNum(a0),d1
        andi.w  #$003F,d1
        move.l  InstBasePtr(pc),a2
        asl.w   #8,d1
        adda.w  d1,a2
        move.w  0(a2),d0
        move.w  d0,42(a0)
        move.b  2(a2),vcInsTyp(a0)
        move.w  4(a2),vcEnvPer(a0)
        move.b  3(a2),vcBuzCtl(a0)
        move.l  6(a2),vcTremStp(a0)
        move.l  252(a2),vcTransX(a0)
        move.w  d4,vcAdsrIdx(a0)
        move.l  10(a2),vcAdsrSus(a0)
        move.w  14(a2),vcAdsrRel(a0)
        lea     16(a2),a3
        move.l  a3,vcAdsrPtr(a0)
        move.w  d4,32(a0)
        move.l  240(a2),34(a0)
        move.w  244(a2),38(a0)
        lea     128(a2),a3
        moveq   #0,d0
        move.b  (a1),d0
        move.w  d0,d1
        lsr.w   #4,d1
        lea     YmPeriods(pc),a2
        andi.w  #$000F,d0

        move.l  a3,vcPitchPtr(a0)
        st      vcPitchFlag(a0)
        andi.w  #$000F,d0
        move.w  d0,d2
        add.w   d2,d2
        move.w  0(a2,d2.w),d2
        lsr.w   d1,d2
        move.w  d2,vcTgtPer(a0)
        move.w  d0,d3
        add.w   vcTransX(a0),d3
        add.w   d3,d3
        move.w  0(a2,d3.w),d3
        lsr.w   d1,d3
        sub.w   d2,d3
        move.w  d3,vcDeltaPerX(a0)
        add.w   vcTransY(a0),d0
        add.w   d0,d0
        move.w  0(a2,d0.w),d0
        lsr.w   d1,d0
        sub.w   d2,d0
        move.w  d0,vcDeltaPerY(a0)

same_instr:
        move.b  1(a1),d1
        beq.s   no_calc_tie
        sub.w   vcCurPer(a0),d2
        ext.l   D2
        andi.l  #$000000FF,d1

        move.l  SpeedPtr(pc),a4
        muls    (a4),d1         ; Speed
        lea     music(pc),a4
        divs    d1,d2

        beq.s   no_calc_tie
        move.w  d2,6(a0)
        bra.s   yes_calc_tie

no_calc_tie:
        move.w  vcTgtPer(a0),vcCurPer(a0)
        move.w  d4,6(a0)
yes_calc_tie:
        move.b  3(a1),d0
        andi.w  #$001F,d0
        move.b  d0,vcVolMul(a0)

test_env:
        addq.l  #3,a1
        move.b  (a1)+,d1
        lsr.b   #6,d1
        tst.b   D1
        beq.s   same_env
        btst    #2,vcInsTyp(a0)
        bne.s   he_adsr
        cmpi.b  #1,d1
        beq.s   attack_env
        cmpi.b  #2,d1
        beq.s   sustain_env
        move.w  vcAdsrDec(a0),vcAdsrIdx(a0)
        move.w  36(a0),32(a0)
        bclr    #0,vcHwBuzz(a0)
        st      vcPitchFlag(a0)
        rts

sustain_env:
	move.w  vcAdsrSus(a0),vcAdsrIdx(a0)
        move.w  34(a0),32(a0)
        bclr    #0,vcHwBuzz(a0)
        st      vcPitchFlag(a0)
        rts

attack_env:
	move.w  d4,8(a0)
        move.w  d4,vcAdsrIdx(a0)
        move.w  d4,32(a0)
        bclr    #0,vcHwBuzz(a0)
        st      vcPitchFlag(a0)
        bclr    #1,vcHwBuzz(a0)
same_env:       rts

he_adsr:
        moveq   #$0B,d7
        moveq   #$0C,d6
        moveq   #$0D,d5
        tst.b   D1
        beq.s   cont_he
        cmpi.b  #2,d1
        beq.s   sustain_he
        cmpi.b  #3,d1
        beq     release_he
        move    #$2700,SR
        btst    #3,vcInsTyp(a0)
        bne.s   attack_hardw_aut
        move.b  d7,(a5)		; Set YM envelop period
        move.b  vcEnvPerL(a0),2(a5)
        move.b  d6,(a5)
        move.b  vcEnvPerH(a0),2(a5)
attack_hardw_aut:
	move.b  d5,(a5)		; Select YM envelop shape
        btst    #0,vcBuzCtl(a0)
        bne.s   yes_hardw_type
        move.b  (a5),d0		; Get hardware envelop shape
        cmp.b   vcBuzCtl(a0),d0	; Same ?
        beq.s   no_hardw_type
yes_hardw_type:
	move.b  vcBuzCtl(a0),2(a5)	; Set new YM envelop shape
no_hardw_type:
	move.w  d4,8(a0)
        move.w  d4,32(a0)
        st      vcPitchFlag(a0)
        bclr    #0,vcHwBuzz(a0)
        bclr    #1,vcHwBuzz(a0)
        beq.s   same_he
        bclr    #5,$FFFFFA07.w
        bclr    #0,$FFFFFA07.w
same_he:
	move    #$2300,SR
cont_he:
        rts

sustain_he:
	move    #$2700,SR
        btst    #3,vcInsTyp(a0)
        bne.s   sustain_hardw_au
        move.b  d7,(a5)
        move.b  vcEnvPerL(a0),2(a5)
        move.b  d6,(a5)
        move.b  vcEnvPerH(a0),2(a5)
sustain_hardw_au:       move.b  d5,(a5)
        btst    #0,vcBuzCtl(a0)
        bne.s   yes_sus_htype
        move.b  (a5),d0
        cmp.b   vcBuzCtl(a0),d0
        beq.s   no_sus_htype
yes_sus_htype:
	move.b  vcBuzCtl(a0),2(a5)
no_sus_htype:
	move.w  d4,8(a0)
        move.w  34(a0),32(a0)
        st      vcPitchFlag(a0)
        bclr    #0,vcHwBuzz(a0)
        bclr    #1,vcHwBuzz(a0)
        beq.s   same_sus_he
        bclr    #5,$FFFFFA07.w
        bclr    #0,$FFFFFA07.w
same_sus_he:
	move    #$2300,SR
        rts

release_he:
	bset    #0,vcHwBuzz(a0)
        rts

digi_:  btst    #2,vcHwBuzz(a0)
        bne.s   oka
        cmpi.b  #$0E,2(a1)
        bne.s   test_2
        move.w  #$FFFF,PatBreakFlag(a4)
        bra.s   oka

test_2: cmpi.b  #$0F,2(a1)
        bne.s   oka
        move.w  #$FFFF,what(a4)
oka:    move.b  2(a1),d0
        andi.w  #$000F,d0
        asl.w   #2,d0
        tst.w   what(a4)
        bmi.s   only_speed
        moveq   #0,d0
        move.b  3(a1),d0
        ext.w   D0
        add.w   d0,d0
        add.w   d0,d0
        jsr     FxJmpTbl(PC,d0.w)
L0040:  addq.l  #4,a1
        rts

only_speed:
        move.b  3(a1),d0
        ext.w   D0
        move.l  SpeedPtr(pc),a6
        move.w  d0,(a6)         ; Grazey Portamento Fix
        addq.l  #4,a1
        move.w  d4,what(a4)
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


tb0:    = $0200
tb1:    = tb0+$0010

ta0:    = tb1+$0010
ta1:    = ta0+$0010

td0:    = ta1+$0010
td1:    = td0+$0010

sid_irqs_s:
        move.l  #$08080000,$FFFF8800.w
        move.w  #tb1,$00000122.w
        rte

        move.l  #$08080000,$FFFF8800.w
        move.w  #tb0,$00000122.w
        rte

        move.l  #$09090000,$FFFF8800.w
        move.w  #ta1,$00000136.w
        rte

        move.l  #$09090000,$FFFF8800.w
        move.w  #ta0,$00000136.w
        rte

        move.l  #$0A0A0000,$FFFF8800.w
        move.w  #td1,$00000112.w
        rte

        move.l  #$0A0A0000,$FFFF8800.w
        move.w  #td0,$00000112.w
        rte
sid_irqs_e:

VoidIrq:        rte

do_tone_control:
        move.l  #$01010100,d6
        tst.b   vcXtTonFlag(a0)
        beq.s   no_more_tone
        movea.l vcXtTonPtr(a0),a6
        moveq   #0,d1
        move.b  (a6)+,d1

	cmp.b	#$FF,d1		; End
	beq.s	no_more_tone
	
        cmp.b   #$FE,d1		; sustain
        blo.s   tone_a
	
        subq    #2,a6		; Rewind
	move.b	(a6)+,d1	; Reload 

tone_a: move.l  a6,vcXtTonPtr(a0)
        add.w   d1,d1
        add.w   d1,d1
        move.l  tone_data(pc,d1.w),d7
        move.l  d7,vcXtSets(a0)
        rts

tone_a_bis:
        move.l  a6,vcXtTonPtr(a0)
no_more_tone:
	move.l  d6,vcXtSets(a0)
        rts

tone_data:			; NOISE,XX,XX,align
	dc.l $00000000  ; 000 Background, drum or snare effect
        dc.l $01000000	; 100
        dc.l $00010000	; 010
        dc.l $01010000	; 110
        dc.l $00000100	; 001
        dc.l $01000100	; 101
        dc.l $01010100	; 111

do_wave_control:
	tst.b   vcXtWavFlag(a0)
        beq.s   no_control_wave
        movea.l vcXtWavPtr(a0),a6
        move.b  (a6)+,d1

        cmpi.b  #$FF,d1         ; End
        beq.s   xno_hardw_auto_b

        cmpi.b  #$FE,d1         ; Sustain
        bne.s   wave_b

        subq    #2,a6		; Rewind to last value of sequence
        move.b  (a6)+,d1	; Reload it

wave_b: move.l  a6,vcXtWavPtr(a0)
        tst.b   d1
        bne.s   no_control_wave

xno_hardw_auto_b:
	bclr    #2,vcInsTyp(a0)
no_control_wave:
	rts

;;; SetMixerAndNoise:
;;;
;;; IN:  d6  pulse-bit
;;;      d7  noise-bit
;;;      d0  current mixer bits
;;;      a0  Voice data
;;;      a5  YM hardware
;;;
;;; OUT: d0  updated
;;;      YM  noise period updated
;;;
SetMixerAndNoise:
        btst    #0,vcInsTyp(a0)
        beq.s   .toneOFF

        tst.b   vcXtTonFlag(a0)
        bne.s   .toneON

        tst.b   vcXtNoiset(a0)
        beq.s   .toneOFF
.toneON:
        bclr    d6,d0

.toneOFF:
        btst    #1,vcInsTyp(a0)
        beq.s   .exit
        tst.b   vcXtNoiFlag(a0)
        bne.s   .noNoiseCtl

        move.w  #$0600,d5
        move.b  43(a0),d5
        movep.w d5,0(a5)
        bra.s   .noiseON

.noNoiseCtl:
        movea.l vcXtNoiPtr(a0),a6
        move.b  (a6)+,d1
        cmpi.b  #$FF,d1         ; NOP
        beq.s   .exit
        cmpi.b  #$FE,d1         ; END
        bne.s   .okSequence
        subq    #2,a6
        move.b  (a6)+,d1        ; Loop the sequence on the last value

.okSequence:
        move.l  a6,vcXtNoiPtr(a0)
        move.w  #$0600,d5
        move.b  d1,d5
        movep.w d5,0(a5)
        tst.b   vcXtSets(a0)
        beq.s   .exit
.noiseON:
        bclr    d7,d0
.exit:
        rts


	;; 72 values
buzzPeriods:
        DC.B $f0,$e2,$d4,$c8,$be,$b2,$a8,$9e,$96,$8e,$86,$7e,$78,$71,$6a,$64
        DC.B $5f,$59,$54,$4f,$4b,$47,$43,$3f,$3c,$38,$35,$32,$2f,$2d,$3b,$28
        DC.B $26,$24,$22,$20,$1e,$1c,$1b,$19,$18,$16,$15,$14,$13,$12,$11,$10
        DC.B $0f,$0e,$0d,$0c,$0b,$0a,$09,$08,$07,$06,$05,$04,$03,$02,$01,$00
        DC.B $16,$12,$28,$2b,$3b,$29,$28,$ff
	

do_fx:  tst.b   vcHwBuzz(a0)
        bne     do_silent
        btst    #2,vcInsTyp(a0)
        beq.s   no_he

        cmpi.b  #$0D,vcTFMX(a0)    ;******; ???
        bne.s   normal_buzz
        moveq   #1,d7
double:
	lea	$ffff8800.w,a6
        moveq   #0,d0
        move.b  0(a0),d0
        add.b   83(a0),d0
        andi.w  #$003F,d0
        move.b  #$0B,(a6)
        move.b  buzzPeriods(pc,d0.w),2(a6)
        move.b  #$0C,(a6)
        move.b  d4,2(a6)
        addq.b  #1,83(a0)
        cmpi.b  #$3F,83(a0)
        ble.s   do_loop
        move.b  d4,83(a0)
do_loop:
	bra.s   to_tie
	
normal_buzz:
	move.b  d4,83(a0)
        move.b  #$10,vcVol(a0)
        bra.s   to_tie

no_he:  move.w  vcAdsrIdx(a0),d0
        movea.l vcAdsrPtr(a0),a2
        move.b  0(a2,d0.w),d1
        move.b  vcVolMul(a0),d2
        asl.b   #4,d2
        add.b   d1,d2
        andi.w  #$00FF,d2
        lea     vol16x16(pc),a2
        move.b  0(a2,d2.w),vcVol(a0)
        addq.w  #1,d0
        cmp.w   vcAdsrDec(a0),d0
        bne.s   no_sus_rep
        move.w  vcAdsrSus(a0),d0
no_sus_rep:
	cmp.w   vcAdsrRel(a0),d0
        bcs.s   no_rel
        bset    #0,vcHwBuzz(a0)
no_rel: move.w  d0,vcAdsrIdx(a0)
to_tie: move.w  6(a0),d0
        beq.s   no_tie
        bmi.s   tie_lo
        add.w   vcCurPer(a0),d0
        cmp.w   vcTgtPer(a0),d0
        blt.s   no_reach_tie_hi
        move.w  d4,6(a0)
        move.w  vcTgtPer(a0),d0
no_reach_tie_hi:
	move.w  d0,vcCurPer(a0)
        bra.s   no_tie
tie_lo: add.w   vcCurPer(a0),d0
        cmp.w   vcTgtPer(a0),d0
        bgt.s   no_reach_tie_lo
        move.w  d4,6(a0)
        move.w  vcTgtPer(a0),d0
no_reach_tie_lo:
	move.w  d0,vcCurPer(a0)
no_tie: move.w  8(a0),d1
        add.w   vcTremStp(a0),d1
        cmpi.w  #$003B,d1
        bls.s   no_re_trem
        moveq   #0,d1
no_re_trem:
	move.w  d1,8(a0)
        add.w   d1,d1
        lea     trem_table(pc),a2
        move.w  0(a2,d1.w),d1
        move.w  vcTremVal(a0),d2
        asr.w   d2,d1
        add.w   vcCurPer(a0),d1
.noPatBrk:

; Hi End Pitch Arpeggio Effect

        cmpi.b  #1,vcTFMX(a0)
        bne.s   no_db_1_notex
        lea     db_table(pc),a1
        move.b  vcArpIdx(a0),d7
        ext.w   d7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcArpIdx(a0)
        cmpi.b  #1,vcArpIdx(a0)
        blt.s   no_db_1_notex
        move.b  #1,vcArpIdx(a0)

; SID Effect combined with Hi End Pitch Arpeggio

no_db_1_notex:
	cmpi.b  #4,vcTFMX(a0)
        bne.s   no_db_1_notey
        lea     db_table(pc),a1
        move.b  vcArpIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcArpIdx(a0)
        cmpi.b  #1,vcArpIdx(a0)
        blt.s   no_db_1_notey
        move.b  #1,vcArpIdx(a0)

; Arpeggio double with SID effect

no_db_1_notey:
	cmpi.b  #5,vcTFMX(a0)
        bne.s   no_db_1_note
        lea     da_table(pc),a1
        move.b  vcDatIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #2,vcDatIdx(a0)
        cmpi.b  #$0C,vcDatIdx(a0)
        blt     da_ok
        move.b  d4,vcDatIdx(a0)

; Different Arpeggio wavesynthesis 3pcs 6-9

no_db_1_note:
	cmpi.b  #6,vcTFMX(a0)
        bne.s   no_db_note2
        lea     db_1_table(pc),a1
        move.b  vcArpIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcArpIdx(a0)
        cmpi.b  #$40,vcArpIdx(a0)
        blt.s   no_db_note2
        move.b  #1,vcArpIdx(a0)

no_db_note2:
	cmpi.b  #7,vcTFMX(a0)
        bne.s   no_db_note3
        lea     db_2_table(pc),a1
        move.b  vcDatIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #2,vcDatIdx(a0)
        cmpi.b  #$20,vcDatIdx(a0)
        blt.s   no_db_note3
        move.b  #2,vcDatIdx(a0)

no_db_note3:
	cmpi.b  #8,vcTFMX(a0)
        bne.s   no_db_note4
        lea     da2_table(pc),a1
        move.b  vcArpIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcArpIdx(a0)
        cmpi.b  #$30,vcArpIdx(a0)
        blt.s   no_db_note4
        move.b  d4,vcArpIdx(a0)

no_db_note4:
	cmpi.b  #9,vcTFMX(a0)
        bne.s   no_db_note5
        lea     dx_table(pc),a1
        move.b  vcDatIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcDatIdx(a0)
        cmpi.b  #$20,vcDatIdx(a0)
        blt.s   no_db_note5
        move.b  d4,vcDatIdx(a0)

no_db_note5:
	cmpi.b  #$0A,vcTFMX(a0)
        bne.s   no_db_note6
        lea     dx_table(pc),a1
        move.b  vcDatIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcDatIdx(a0)
        cmpi.b  #$20,vcDatIdx(a0)
        blt.s   spe
        move.b  d4,vcDatIdx(a0)
spe:    lea     da_table(pc),a1
        move.b  vcArpIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcArpIdx(a0)
        cmpi.b  #4,vcArpIdx(a0)
        blt.s   no_db_note6
        move.b  d4,vcArpIdx(a0)

no_db_note6:
	cmpi.b  #$0B,vcTFMX(a0)
        bne.s   no_db_note7
        lea     arp_spec(pc),a1
        move.b  vcArpIdx(a0),d7
        ext.w   d7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcArpIdx(a0)
        cmpi.b  #$10,vcArpIdx(a0)
        blt.s   spe2
        move.b  d4,vcArpIdx(a0)
spe2:   lea     db_2_table(pc),a1
        move.b  vcDatIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #1,vcDatIdx(a0)
        cmpi.b  #$12,vcDatIdx(a0)
        blt.s   no_db_note7
        move.b  d4,vcDatIdx(a0)

no_db_note7:
	cmpi.b  #$0C,vcTFMX(a0)
        bne.s   no_db_note
        tst.b   vcHwBuzz(a0)
        bne.s   no_db_note
        btst    #2,vcInsTyp(a0)
        beq.s   no_db_note
        lea     spec(pc),a1
        move.b  vcSpecIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),d0
        move.b  d0,vcSpecVal(a0)
        andi.w  #$003F,d0
        add.w   d0,32(a0)
        addq.b  #1,vcSpecIdx(a0)
        cmpi.b  #$2A,vcSpecIdx(a0)
        blt.s   no_db_note
        move.b  d4,vcSpecIdx(a0)

; New Arpeggio (Double) with wavesynthesis

no_db_note:
	cmpi.b  #2,vcTFMX(a0)
        bne.s   da_ok
        lea     da_table(pc),a1
        move.b  vcDatIdx(a0),d7
        ext.w   D7
        move.b  0(a1,d7.w),vcSpecVal(a0)
        addq.b  #2,vcDatIdx(a0)
        cmpi.b  #$0C,vcDatIdx(a0)
        blt.s   da_ok
        move.b  d4,vcDatIdx(a0)
da_ok:  move.b  vcSpecVal(a0),d0
        beq.s   intr_0

        cmpi.b  #1,d0
        beq.s   intr_1
        add.w   vcDeltaPerY(a0),d1
        move.b  d4,vcSpecVal(a0)
        bra.s   intr_2
intr_1: add.w   vcDeltaPerX(a0),d1
intr_0: addq.b  #1,vcSpecVal(a0)
intr_2: tst.b   vcPitchFlag(a0)
        beq.s   no_pitch
        move.w  32(a0),d0
        movea.l vcPitchPtr(a0),a2
        move.b  0(a2,d0.w),d2
        ext.w   D2
        tst.b   vcXtNoiFlag(a0)
        beq.s   no_noise_pitch2
        add.w   d2,d1
        add.w   d2,d1
        add.w   d2,d1
no_noise_pitch2:
	add.w   d2,d1
        addq.w  #1,d0
        cmp.w   36(a0),d0
        bne.s   no_pit_sus_rep
        move.w  34(a0),d0
no_pit_sus_rep:
	cmp.w   38(a0),d0
        bcs.s   no_pit_rel
        move.b  d4,vcPitchFlag(a0)
no_pit_rel:
	move.w  d0,32(a0)
no_pitch:
	move.w  d1,vcTonPer(a0)
        cmpi.b  #$0D,vcTFMX(a0)
        bne.s   no_special_buzz
        dbra    d7,double
no_special_buzz moveq   #0,d7
        rts
do_silent:      move.b  d4,vcVol(a0)
        rts

	;; Volume pre-multication table
vol16x16:
        DC.B $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
        DC.B $00,$00,$00,$00,$00,$00,$00,$00,$01,$01,$01,$01,$01,$01,$01,$01
        DC.B $00,$00,$00,$00,$01,$01,$01,$01,$01,$01,$01,$01,$02,$02,$02,$02
        DC.B $00,$00,$00,$01,$01,$01,$01,$01,$02,$02,$02,$02,$02,$03,$03,$03
        DC.B $00,$00,$01,$01,$01,$01,$02,$02,$02,$02,$03,$03,$03,$03,$04,$04
        DC.B $00,$00,$01,$01,$01,$02,$02,$02,$03,$03,$03,$04,$04,$04,$05,$05
        DC.B $00,$00,$01,$01,$02,$02,$02,$03,$03,$04,$04,$04,$05,$05,$06,$06
        DC.B $00,$00,$01,$01,$02,$02,$03,$03,$04,$04,$05,$05,$06,$06,$07,$07
        DC.B $00,$01,$01,$02,$02,$03,$03,$04,$04,$05,$05,$06,$06,$07,$07,$08
        DC.B $00,$01,$01,$02,$02,$03,$04,$04,$05,$05,$06,$07,$07,$08,$08,$09
        DC.B $00,$01,$01,$02,$03,$03,$04,$05,$05,$06,$07,$07,$08,$08,$09,$0a
        DC.B $00,$01,$01,$02,$03,$04,$04,$05,$06,$07,$07,$08,$09,$0a,$0a,$0b
        DC.B $00,$01,$02,$02,$03,$04,$05,$06,$06,$07,$08,$09,$0a,$0a,$0b,$0c
        DC.B $00,$01,$02,$03,$03,$04,$05,$06,$07,$08,$09,$0a,$0a,$0b,$0c,$0d
        DC.B $00,$01,$02,$03,$04,$05,$06,$07,$07,$08,$09,$0a,$0b,$0c,$0d,$0e
        DC.B $00,$01,$02,$03,$04,$05,$06,$07,$08,$09,$0a,$0b,$0c,$0d,$0e,$0f


db_table:
	DC.B $01,$00,$FF
db_1_table:
	DC.B $01,$00,$01,$00,$02,$00,$02,$00,$03
        DC.B $00,$FF
db_2_table:
	DC.B $00,$00,$01,$01,$02,$02,$03,$03
        DC.B $04,$04,$05,$05,$06,$06,$05,$05
        DC.B $04,$04,$03,$03,$02,$02,$01,$01
        DC.B $00,$00,$FF
da_table:
	DC.B $00,$00,$01,$01,$02,$02,$FF
da2_table:
	DC.B $09,$09,$05,$05,$03,$03,$01,$01,$02,$01,$01,$02,$02,$03,$03,$03
        DC.B $04,$04,$05,$00,$01,$02,$FF
	
dx_table:
	;; 60 values (.b)
	DC.B $00,$08,$00,$07,$00,$06,$00,$05,$00,$04,$00,$03,$00,$02,$00,$01
        DC.B $00,$01,$01,$00,$02,$00,$03,$00,$04,$00,$05,$00,$06,$00,$FF,$01
        DC.B $01,$01,$00,$00,$00,$02,$02,$02,$00,$00,$00,$03,$03,$03,$00,$00
        DC.B $00,$04,$04,$04,$00,$00,$00,$05,$05,$05,$FF
	
spec:
	;; 42 values (.b)
	DC.B $10,$0F,$10,$0E,$10,$0D,$10,$0C,$10,$0B,$10,$0A,$10,$09,$10,$08
        DC.B $10,$07,$10,$06,$10,$05,$10,$04,$10,$03,$10,$02,$10,$01,$10,$10
        DC.B $10,$10,$09,$08,$06,$04,$02,$00,$02,$FF
	
arp_spec:
	;; 30 values (.b)
	DC.B $00,$09,$00,$08,$00,$07,$00,$06,$00,$05,$00,$04,$00,$03,$00,$02
	DC.B $00,$01,$00,$01,$02,$03,$04,$05,$06,$07,$08,$09,$FF,$00

ToggleSidDephaseA:
        not.w   SidDephaseA(a4)
        rts

ToggleSidDephaseB:
        not.w   SidDephaseB(a4)
        rts

ToggleSidDephaseC:
        not.w   SidDephaseC(a4)
        rts

ActivSidVoiceA:
        pea     tb0.w
        move.l  (a7)+,$00000120.w
        st      SidOnOffA(a4)
        rts

ActivSidVoiceB:
        pea     ta0.w
        move.l  (a7)+,$00000134.w
        st      SidOnOffB(a4)
        rts

ActivSidVoiceC:
        pea     td0.w
        move.l  (a7)+,$00000110.w
        st      SidOnOffC(a4)
        rts

StopSidVoiceA:
        pea     VoidIrq(pc)
        move.l  (a7)+,$00000120.w
        sf      SidOnOffA(a4)
        rts

StopSidVoiceB:
        pea     VoidIrq(pc)
        move.l  (a7)+,$00000134.w
        sf      SidOnOffB(a4)
        rts

StopSidVoiceC:
        pea     VoidIrq(pc)
        move.l  (a7)+,$00000110.w
        sf      SidOnOffC(a4)
        rts

do_wave_1:
        pea     wave_form1(pc)
        move.l  (a7)+,WavFormPtr(a4)
        rts

do_wave_2:
        pea     wave_form2(pc)
        move.l  (a7)+,WavFormPtr(a4)
        rts

do_wave_3:
        pea     wave_form3(pc)
        move.l  (a7)+,WavFormPtr(a4)
FxNone: rts


reset_all:
        pea     wave_form1(pc)
        move.l  (a7)+,WavFormPtr(a4)
        ;; move.w  merk_speed(pc),offset(a4)
        st      SidDephaseA(a4)
        st      SidDephaseB(a4)
        st      SidDephaseC(a4)
        st      SidOnOffA(a4)
        st      SidOnOffB(a4)
        st      SidOnOffC(a4)
        rts

i_flag: DC.W 0

;;;
;;;
;;; IN:  D0  Select which timer the replay will use for which voice.
;;;          To be valid it must be a combination of the upper case letter
;;;          'A','B','C' and 'D' each of which representing the
;;;          corresponding timer. The order of the letter gives the voice
;;;          allocation: higher order byte is YM channel A, lower order
;;;          byte is the unused timer.
;;;
;;;      A0  Pointer to the .TVS buffer
;;;      A1  Pointer to the .TRI buffer
;;;
;;; USE: A4  Pointer to music for PIC access
	
init_music:
        lea     music(pc),a4
        movem.l A0-A1,-(a7)

	;;D ELME: just checking struct size while working on it
	move	#vcSize,d7
	cmp.w	#84,d7
	beq.s	.okCheckSize
	illegal
	stop	#$2700
	reset
.okCheckSize:	

        lea     i_flag(pc),a3
        tst.w   (a3)
        bne.s   already_in_relo
        st      (a3)

        bsr     save_MFP
        bsr     save_h200
	
already_in_relo:
        movem.l (a7)+,a0-A1

        moveq   #0,d0           ;3 SID-voices
	move.w  d0,merk_d1(a4)

        move    #$2700,SR

        move.l  a0,VoiceSetPtr(a4)      ;Voiceset
        move.l  a1,SoundDataPtr(a4)     ;Sounddata

        movea.l VoiceSetPtr(pc),a0
        lea     516(a0),a0
        move.l  a0,InstBasePtr(a4)

        movea.l SoundDataPtr(pc),a0
        addq.l  #4,a0
        move.w  (a0),merk_speed(a4)

        bsr.s   reset_all

        move.l  a0,ReNoteCntPtr(a4)
        move.l  a0,SpeedPtr(a4)
        addq.l  #2,a0
        move.l  a0,SongRePtr(a4)
        addq.l  #2,a0
        move.w  (a0),SongLen(a4)
        addq.l  #2,a0
        move.l  a0,SMC9E(a4)
        addq.l  #2,a0
        move.l  a0,SongBasePtr(a4)
        lea     120(a0),a0
        move.l  a0,PattBasePtr(a4)
        move.l  a0,RePatPtr(a4)
        move.l  a0,ReCurPatPtr(a4)


;;; ben: Removed the info block copy and use a pointer to either the
;;;      file provided one or the default one. It should be safe as it
;;;      looks like it's read-only data.

        lea     DefaultInfo(pc),a1
        movea.l VoiceSetPtr(pc),a0
        lea     16900(a0),a0    ; $$$ ben: FIXME add a symbol for this
        cmpi.l  #"INFO",(a0)+
        bne.s   .keepDefInfo
        move.l  a0,a1
.keepDefInfo:
        move.l  a1,CurInfo(a4)

        ;; lea  BufInfo(pc),a1
        ;; move.l       a1,CurInfo(a4)
        ;; movea.l VoiceSetPtr(pc),a0
        ;; lea  16900(a0),a0


;;; ben: Copy info block. The copy routine copy 256 bytes, but the
;;;      DefaultInfo struct is only 252 bytes long and so is the
;;;      BufInfo struct. My best guest is that the info block is 256
;;;      bytes including the "INFO" tag. Obviously there was an
;;;      overflow in that copy routine.

;;; ben: I'm not even sure this need to be done, may be a pointer to
;;;      the 'INFO' block would do just right. Depend if it's read-only
;;;      or not.

;;      cmpi.l  #"INFO",(a0)+
;;      beq.s   .okInfo

;;      lea     DefaultInfo(pc),a0
;; .okInfo:
;;      moveq   #252/4-1,d0
;; .cpyInfo:
;;      move.l  (a0)+,(a1)+
;;      dbra    d0,.cpyInfo

        bsr     L009C

        bclr    #3,$FFFFFA17.w  ;auto eoi

        bsr     copy_sid_psg_rout

        move.l  #td0,$00000110.w ;Timer D
        andi.b  #$F0,$FFFFFA1D.w
        ori.b   #1,$FFFFFA1D.w
        clr.b   $FFFFFA25.w
        bset    #4,$FFFFFA15.w
        bclr    #4,$FFFFFA09.w

L0096:  move.l  #tb0,$120.w     ;Timer B
        move.b  #%11,$fffffa1b.w
        clr.b   $fffffa21.w
        bset    #0,$fffffa13.w
        bclr    #0,$fffffa07.w

L0097:  move.l  #ta0,$00000134.w ;Timer A
        clr.b   $FFFFFA19.w
        move.b  #1,$FFFFFA19.w
        move.b  #1,$FFFFFA1F.w
        bset    #5,$FFFFFA13.w
        bclr    #5,$FFFFFA07.w
        bsr.s   YmReset

        st      ReplayFlag(a4)
        move    #$2300,SR
        rts


YmReset:
        lea     Voices(pc),a0
	
        bclr    #5,$FFFFFA07.w  ;Timer A
        bclr    #4,$FFFFFA09.w  ;Timer D

;;; $$$ ben: Clearly this routine expects the Voice structure to be 82
;;;     bytes long (0+82=142 142+82=224).
;;;     This is weird because an other part of the code suggest it
;;;     should be larger (84).
	
        ;; bset    #0,224(a0)
        ;; bclr    #1,224(a0)
        ;; bset    #0,60(a0)
        ;; bset    #0,142(a0)
        ;; st      223(a0)
        ;; st      59(a0)
        ;; st      141(a0)

	;; ;; ben: replace the code commented just above
        ;; bset    #0,60+0*vcSize(a0)
        ;; bset    #0,60+1*vcSize(a0)
        ;; bset    #0,60+2*vcSize(a0)
        ;; bclr    #1,60+2*vcSize(a0) ; ben: why on this channel ?
        ;; st      59+0*vcSize(a0)
        ;; st      59+1*vcSize(a0)
        ;; st      59+2*vcSize(a0)

	;; ben: however this 59 thingy does not make sense...
        bset    #0,60+0*vcSize(a0)
        bset    #0,60+1*vcSize(a0)
        bset    #0,60+2*vcSize(a0)
        bclr    #1,60+2*vcSize(a0) ; ben: why on this channel ?
        st      59+0*vcSize(a0)
        st      59+1*vcSize(a0)
        st      59+2*vcSize(a0)

	
        lea     $FFFF8800.w,a0
        lea     YmShadow(pc),a1
        moveq   #12,d1
.lpReg:
	move.w  (a1)+,d0
        movep.w d0,0(a0)
        dbra    d1,.lpReg
	
        move.b  #7,(a0)
        move.b  (a0),d0
        andi.b  #$C0,d0
        ori.b   #$3F,d0
        move.b  d0,2(a0)
	
        rts

YmShadow:
        DC.B $00,$00,$01,$00,$02,$00,$03,$00
        DC.B $04,$00,$05,$00,$06,$00,$08,$00
        DC.B $09,$00,$0A,$00,$0B,$00,$0C,$00
        DC.B $0D,$00


L009C:  bsr.s   L009D
        clr.w   PatPos(a4)
        clr.b   ta1+4.w ;8
        bra     L00AD

L009D:
        move.l  SMC9E(pc),a0
        move.w  #1,(a0)

	
        moveq   #$1F,d0		; $$$ ben: only 32 patterns ? I don't get it !
        move.l  PattBasePtr(pc),a0
	
        lea     PatPtrTbl(pc),a1
L00A1:  move.l  a0,(a1)+
        lea     768(a0),a0	; ben: FIXME make 768 symbolic (patSize)
        dbra    d0,L00A1
	
        move.w  #1,NoteCount(a4)
        clr.w   CurSongPos(a4)
        move.l  RePatPtr(pc),PatPtr(a4)
        move.l  ReCurPatPtr(pc),CurPatPtr(a4)


;;; $$$ ben: This thing does not make sense to me, not only it has the
;;;     wrong size (82) for the voice struct, but the offset used does
;;;     not seem to match either.

;;; $$$ ben: Ok so this loop is meant to initialise both volume and
;;;     pitch envelop. Clearly the displacement were wrong.
	
        lea     VoidSeq(pc),a1
        lea     Voices(pc),a0
        moveq   #3-1,d2		; For 3 channels
.lpChannel:
	clr.b   vcInsTyp(a0)
	clr.b   vcPitchFlag(a0)
	
        move.l  a1,vcAdsrPtr(a0)
	move.w	#$0000,vcAdsrIdx(a0)
	move.w	#$0002,vcAdsrSus(a0)
	move.w	#$0005,vcAdsrDec(a0)
	move.w	#$0009,vcAdsrRel(a0)

        move.l  a1,vcPitchPtr(a0)
	move.w	#$0000,vcPitchIdx(a0)
	move.w	#$0002,vcPitchSus(a0)
	move.w	#$0005,vcPitchDec(a0)
	move.w	#$0009,vcPitchRel(a0)
	
        lea     vcSize(a0),a0
        dbra    d2,.lpChannel
        rts


load_pos:
        move.w  d4,PatPos(a4)
        move.w  CurSongPos(pc),d0
        addq.w  #1,d0

        cmp.w   SongLen(pc),d0
        bls.s   L00AC

        move.l  SongRePtr(pc),a0
        move.w  (a0),d0
L00AC:
        move.w  d0,CurSongPos(a4)
L00AD:
        move.l  SongBasePtr(pc),a0

        move.w  CurSongPos(pc),d0
        move.b  0(a0,d0.w),d0
        andi.w  #$007F,d0
        move.w  d0,CurPatNum(a4)
        lea     PatPtrTbl(pc),a0
        add.w   d0,d0
        add.w   d0,d0
        move.l  0(a0,d0.w),CurPatPtr(a4)
        rts

SidDephaseA:    DC.B $00,$01
SidDephaseB:    DC.B $00,$01
SidDephaseC:    DC.B $00,$01

SidOnOffA       DC.B $FF,$FF
SidOnOffB:      DC.B $FF,$FF
SidOnOffC:      DC.B $FF,$FF

;;; ben: reformat these tables (easier on the eyes)
wave_form1:
        DC.W $0068,$0004,$0001,$0105,$000a,$0002,$01a2,$0010
        DC.W $0003,$051a,$0032,$0004,$0688,$0040,$0005,$0a35
        DC.W $0064,$0006,$0eef,$00c8,$0007,$ffff

wave_form2:
        DC.W $0068,$0004,$0001,$0105,$0014,$0002,$01a0,$0010
        DC.W $0003,$0519,$0064,$0004,$0340,$0040,$0005,$0a32
        DC.W $00c8,$0006,$0ea0,$0100,$0007,$0f4b,$0068,$0004
        DC.W $0001,$0105,$0014,$0002,$01a0,$0010,$0003,$0519
        DC.W $0064,$0004,$0340,$0040,$0005,$0a32,$00c8,$0006
        DC.W $0ea0,$0100,$0007,$0f4b,$ffff

wave_form3:
        DC.W $0068,$0004,$0001,$0105,$001e,$0002,$01a0,$0010
        DC.W $0003,$0519,$0096,$0004,$0340,$0040,$0005,$0a32
        DC.W $012c,$0006,$0ea0,$0100,$0007,$0f4b,$0068,$0004
        DC.W $0001,$0105,$001e,$0002,$01a0,$0010,$0003,$0519
        DC.W $0096,$0004,$0340,$0040,$0005,$0a32,$012c,$0006
        DC.W $0ea0,$0100,$0007,$0f4b,$ffff

	;; This is the default "INFO" block
DefaultInfo:
        DC.L $01010000,$02020000,$00000100,$00000003
        DC.L $00000003,$03030001,$00000001,$00000002
        DC.L $00000002,$00000002,$00000002,$00000001
        DC.L $00000003,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000003,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000,$00000000
        DC.L $00000000,$00000000,$00000000

;;; $$$ ben: PCR and optimize at the same time (no need for long words)
;;;     Only thing is to be sure those table are really indepedant else
;;;     crossing over will not work properly. One solution to avoid this
;;;     problem would be to use the same base for all 3 tables.
	
nois_table:
        DC.W nois_BC-nois_table	;00
        DC.W nois_BD-nois_table
        DC.W nois_BE-nois_table
        DC.W nois_BF-nois_table
        DC.W nois_C0-nois_table	;04
        DC.W nois_C1-nois_table
        DC.W nois_C2-nois_table
        DC.W nois_C3-nois_table
        DC.W nois_C4-nois_table	;08
        DC.W nois_C5-nois_table
        DC.W nois_C6-nois_table
        DC.W nois_C7-nois_table
        DC.W nois_C8-nois_table	;12
        DC.W nois_C9-nois_table
        DC.W nois_CA-nois_table
        DC.W nois_CB-nois_table
        DC.W nois_CC-nois_table	;16
        DC.W nois_CD-nois_table
        DC.W nois_CE-nois_table
        DC.W nois_CF-nois_table
        DC.W nois_D0-nois_table	;20
        DC.W nois_D1-nois_table
        DC.W nois_D2-nois_table
        DC.W nois_D3-nois_table
        DC.W nois_D4-nois_table	;24
        DC.W nois_D5-nois_table
        DC.W nois_D6-nois_table
        DC.W nois_D7-nois_table
        DC.W nois_D8-nois_table	;28
        DC.W nois_D9-nois_table
        DC.W nois_DA-nois_table
        DC.W nois_DB-nois_table
        DC.W nois_DC-nois_table	;32
        DC.W nois_DD-nois_table
        DC.W nois_DE-nois_table
        DC.W nois_DF-nois_table
        DC.W nois_E0-nois_table	;36
        DC.W nois_E1-nois_table
        DC.W nois_E2-nois_table
        DC.W nois_E3-nois_table
        DC.W nois_E4-nois_table	;40
        DC.W nois_E5-nois_table
        DC.W nois_E6-nois_table
        DC.W nois_E7-nois_table
        DC.W nois_E8-nois_table	;44
        DC.W nois_E9-nois_table
        DC.W nois_EA-nois_table

tone_table:
        DC.W tone_EB-tone_table	;00
        DC.W tone_EC-tone_table
        DC.W tone_ED-tone_table
        DC.W tone_EE-tone_table
        DC.W tone_EF-tone_table	;04
        DC.W tone_F0-tone_table
        DC.W tone_F1-tone_table
        DC.W tone_F2-tone_table
        DC.W tone_F3-tone_table	;08
        DC.W tone_F4-tone_table
        DC.W tone_F5-tone_table
        DC.W tone_F6-tone_table
        DC.W tone_F7-tone_table	;12
        DC.W tone_F8-tone_table
        DC.W tone_F9-tone_table
        DC.W tone_FA-tone_table
        DC.W tone_FB-tone_table	;16
        DC.W tone_FC-tone_table
        DC.W tone_FD-tone_table
        DC.W tone_FE-tone_table
        DC.W tone_FF-tone_table	;20
        DC.W tone_00-tone_table
        DC.W tone_01-tone_table
        DC.W tone_02-tone_table
        DC.W tone_03-tone_table	;24
        DC.W tone_04-tone_table
        DC.W tone_05-tone_table
        DC.W tone_06-tone_table
        DC.W tone_07-tone_table	;28
        DC.W tone_08-tone_table
        DC.W tone_09-tone_table
        DC.W tone_0A-tone_table
        DC.W tone_0B-tone_table	;32
        DC.W tone_0C-tone_table

wave_table:
        DC.W wave_0D-wave_table	;00
        DC.W wave_0E-wave_table
        DC.W wave_0F-wave_table
        DC.W wave_10-wave_table
        DC.W wave_11-wave_table	;04
        DC.W wave_12-wave_table
        DC.W wave_13-wave_table
        DC.W wave_14-wave_table
        DC.W wave_15-wave_table	;08
        DC.W wave_16-wave_table
        DC.W wave_17-wave_table
        DC.W wave_18-wave_table
        DC.W wave_19-wave_table	;12
        DC.W wave_1A-wave_table
        DC.W wave_1B-wave_table
        DC.W wave_1B-wave_table
        DC.W wave_1B-wave_table	;16
        DC.W wave_1B-wave_table
        DC.W wave_1B-wave_table
        DC.W wave_1B-wave_table
        DC.W wave_1B-wave_table	;20
        DC.W wave_1B-wave_table
        DC.W wave_1B-wave_table
        DC.W wave_1B-wave_table

        ;; Noise sequences

nois_BC: DC.B $10,$FF
nois_BD: DC.B $A,$11,$F,$11,$D,$C,$8,$FF
nois_BE: DC.B $4,$FF
nois_BF: DC.B $3,$FF
nois_C0: DC.B $20,$20,$FF
nois_C1: DC.B $9,$20,$20,$FF
nois_C2: DC.B $20,$9,$C,$20,$FF
nois_C3: DC.B $20,$4,$8,$E,$12,$16,$1A,$1E,$FE
nois_C4: DC.B $20,$FE
nois_C5: DC.B $20,$FE
nois_C6: DC.B $11,$FE
nois_C7: DC.B $20,$8,$FE
nois_C8: DC.B $D,$9,$FE
nois_C9: DC.B $13,$E,$C,$B,$4,$FE
nois_CA: DC.B $20,$20,$19,$A,$5,$3,$2,$1,$FE
nois_CB: DC.B $1,$2,$3,$4,$5,$6,$7,$8,$7,$6,$5,$4,$3,$2,$1,$FE
nois_CC: DC.B $D,$3,$D,$5,$D,$11,$6,$11,$6,$FE
nois_CD: DC.B $E,$15,$FE
nois_CE: DC.B $F,$18,$FE
nois_CF: DC.B $18,$FF
nois_D0: DC.B $6,$15,$C,$15,$7,$15,$4,$15,$8,$FF
nois_D1: DC.B $20,$FF
nois_D2: DC.B $F,$16,$14,$16,$12,$11,$D,$FF
nois_D3: DC.B $20,$20,$18,$10,$8,$FF
nois_D4: DC.B $14,$1B,$19,$1B,$17,$16,$12,$FF
nois_D5: DC.B $30,$8,$FF
nois_D6: DC.B $2F,$5,$2F,$3,$3B,$1,$FF
nois_D7: DC.B $8,$10,$50,$FF
nois_D8: DC.B $14,$12,$10,$E,$C,$A,$8,$FF
nois_D9: DC.B $12,$8,$FF
nois_DA: DC.B $16,$12,$28,$2B,$3B,29,$28,$FF
nois_DB: DC.B $4,$FF
nois_DC: DC.B $2,$12,$31,$1,$25,$5,$36,$6,$FF
nois_DD: DC.B $A,$B,$C,$D,$E,$F,$FF
nois_DE: DC.B $29,$1,$FF
nois_DF: DC.B $B,$FF
nois_E0: DC.B $A,$0,$0,$0,$E,$4,$4,$FF
nois_E1: DC.B $2B,$FF
nois_E2: DC.B $15,$2,$FF
nois_E3: DC.B $C,$1,$2,$1,$FF
nois_E4: DC.B $D,$D,$D,$1,$3,$FF
nois_E5: DC.B $1,$FF
nois_E6: DC.B $14,$4,$A,$2,$FF
nois_E7: DC.B $C,$1,$4,$E,$4,$10,$4,$12,$FF
nois_E8: DC.B $0,$0,$9,$FF
nois_E9: DC.B $0,$0,$9,$C,$FF
nois_EA: DC.B $0,$0,$0,$1,$1,$1,$0,$0,$1,$1,$0,$1,$FF

	;; Tone (Pulse) sequences
	
tone_EB: DC.B $1,$2,$FE
tone_EC: DC.B $3,$2,$2,$2,$1,$1,$FE
tone_ED: DC.B $3,$2,$FE
tone_EE: DC.B $7,$4,$4,$4,$2,$2,$FF
tone_EF: DC.B $1,$0,$0,$0,$1,$0,$E,$F,$F,$E,$C,$8,$0,$FE
tone_F0: DC.B $0,$2,$2,$2,$2,$3,$0,$E,$D,$C,$FE
tone_F1: DC.B $3,$0,$E,$0,$D,$B,$9,$7,$5,$3,$1,$0,$FE
tone_F2: DC.B $2,$E,$0,$0,$2,$2,$0,$1,$10,$FE
tone_F3: DC.B $C,$4,$0,$0,$C,$D,$FE
tone_F4: DC.B $2,$3,$3,$0,$0,$C,$D,$FE
tone_F5: DC.B $1,$0,$0,$0,$2,$0,$F,$F,$E,$D,$C,$C,$0,$FE
tone_F6: DC.B $6,$0,$0,$0,$0,$FE
tone_F7: DC.B $19,$14,$F,$A,$5,$0,$FE
tone_F8: DC.B $0,$1,$0,$7,$C,$FE
tone_F9: DC.B $0,$0,$18,$C,$0,$FE
tone_FA: DC.B $14,$A,$0,$FE
tone_FB: DC.B $3,$4,$FE
tone_FC: DC.B $8,$5,$5,$5,$5,$5,$5,$4,$4,$4,$3,$3,$2,$2,$1,$FE
tone_FD: DC.B $8,$7,$FE
tone_FE: DC.B $5,$4,$3,$2,$1,$2,$3,$4,$5,$6,$7,$8,$9,$A,$FE
tone_FF: DC.B $8,$A,$FE
tone_00: DC.B $9,$8,$8,$8,$8,$8,$8,$8,$7,$7,$7,$6,$6,$5,$FE
tone_01: DC.B $9,$6,$FE
tone_02: DC.B $A,$B,$C,$D,$E,$F,$FE
tone_03: DC.B $B,$C,$D,$E,$F,$FE
tone_04: DC.B $1,$B,$1,$C,$1,$D,$FE
tone_05: DC.B $28,$4,$28,$4,$28,$4,$28,$FE
tone_06: DC.B $1,$2,$3,$4,$5,$6,$7,$8,$9,$A,$9,$8,$7,$6,$5,$4,$3,$2,$1,$FE
tone_07: DC.B $9,$8,$7,$6,$5,$4,$3,$2,$1,$2,$3,$4,$5,$6,$7,$8,$9,$FE
tone_08: DC.B $A,$0,$9,$0,$8,$0,$7,$0,$6,$0,$5,$0,$4,$0,$3,$0,$2,$0,$1,$0,$FE
tone_09: DC.B $14,$12,$10,$E,$C,$A,$8,$7,$6,$5,$4,$3,$2,$1,$FE
tone_0A: DC.B $20,$1,$10,$2,$FE
tone_0B: DC.B $A,$14,$0,$14,$A,$FE
tone_0C: DC.B $A,$5,$FE

	;; Wave sequences
	
wave_0D: DC.B $1,$1,$1,$FF
wave_0E: DC.B $1,$1,$FF
wave_0F: DC.B $1,$FF
wave_10: DC.B $1,$1,$1,$1,$FE
wave_11: DC.B $A,$B,$C,$D,$E,$F,$FE
wave_12: DC.B $1,$1,$1,$1,$1,$FE
wave_13: DC.B $1,$1,$1,$1,$1,$1,$FE
wave_14: DC.B $28,$28,$28,$28,$28,$28,$28,$FE
wave_15: DC.B $1,$1,$1,$1,$1,$1,$1,$1,$FE
wave_16: DC.B $1,$2,$1,$2,$1,$2,$1,$2,$FE
wave_17: DC.B $2,$1,$2,$1,$2,$1,$2,$1,$FE
wave_18: DC.B $1,$3,$1,$3,$1,$3,$1,$3,$FE
wave_19: DC.B $3,$1,$3,$1,$3,$1,$3,$1,$FE
wave_1A: DC.B $1,$4,$1,$4,$1,$4,$1,$4,$FE
wave_1B: DC.B $1,$1,$4,$1,$4,$1,$4,$1,$FE

;;; $$$ ben: FIXME: this data should not be access but it is somehow
	DC.B $1,$1,$4,$1,$4,$1,$4,$1,$FE
	;; DC.B $1,$1,$4,$1,$4,$1,$4,$1,$FE
	;; DC.B $1,$1,$4,$1,$4,$1,$4,$1,$FE
	
	;; 60 values (5 octaves?)
trem_table:
        DC.W $0000,$01ac,$0354,$04f2,$0682,$0800,$0968,$0ab5
        DC.W $0be4,$0cf2,$0ddb,$0e9e,$0f38,$0fa6,$0fea,$1000
        DC.W $0fea,$0fa6,$0f38,$0e9e,$0ddb,$0cf2,$0be4,$0ab5
        DC.W $0968,$0800,$0682,$04f2,$0354,$01ac,$0000,$fe54
        DC.W $fcac,$fb0e,$f97e,$f800,$f698,$f54b,$f41c,$f30e
        DC.W $f225,$f162,$f0c8,$f05a,$f016,$f000,$f016,$f05a
        DC.W $f0c8,$f162,$f225,$f30e,$f41c,$f54b,$f698,$f800
        DC.W $f97e,$fb0e,$fcac,$fe54

	;; ben: these 2 bytes are not part of the volume table.
	;; 
	;; DC.B $FF,$00 

YmPeriods:
        DC.W $0FD1,$0EEE,$0E17,$0D4D,$0CBE,$0BD9,$0B2F,$0A8E
        DC.W $09F7,$0967,$08E0,$0861,$07E8,$0777,$070C,$06A7
        DC.W $0647,$05ED,$0598,$0547,$04FC,$04B4,$0470,$0431
        DC.W $03F4,$03BB,$0385,$0353,$0323,$02F6,$02CB,$02A3
        DC.W $027D,$0259,$0238,$0218,$01FA,$01DD,$01C2,$01A9
        DC.W $0191,$017B,$0165,$0151,$013E,$012C,$011C,$010C
        DC.W $00FD,$00EE,$00E1,$00D4,$00C8,$00BD,$00B2,$00A8
        DC.W $009F,$0096,$008E,$0086,$007E,$0077,$0070,$006A
        DC.W $0064,$005E,$0059,$0054,$004F,$004B,$0047,$0043
        DC.W $003F,$003B,$0038,$0035,$0032,$002F,$002C,$003B
        DC.W $0027,$0025,$0023,$0021,$001F,$001D,$001C,$001A
        DC.W $0019,$0017,$0016,$0015,$0013,$0012,$0011,$0010
        DC.W $000F,$000E,$000D,$000C,$000B,$000A,$0009,$0008
        DC.W $0007,$0006,$0005,$0004,$0003,$0002,$0001,$0000
        DC.W $0000

;;; $$$ ben: starting to reverse that voice struct :)
;;;
;;; +0.W tone period
;;; +14.b current volume

;;; +43.b direct noise period
;;; +57.b instrument number [0..63]
;;; +58.b flags [bit-1 desactive chan A]
;;; +60.l is a copy of the 'INFO' block entry
;;;  +60.b direct noise control (0:off)
;;;        extended noise control (0:off >0:index in nois_table)
;;;  +61.b flag, affect tone-mixer (!0:force tone ON (may be sid))
;;;        extended tone control (0:off >0:index in tone_table)
;;;  +62.b extended wave control (0:off >0:index in wave_table)
;;;  +63.b ???

;;; +78.l mask from mask table
;;; +79.b sound-flag (0:tone-off)

;;; +64.l pointer to noise sequence
;;;
;;;
;;; +72.l pointer to some sequence

;;; +44.b voice-flags
;;;     bit-0:tone
;;;     bit-1:noise
;;;     bit-2:involved in env control (0:no-env control)
;;;     bit-3:idem
;;; +78.b yet another noise affect flag [0:no noise but sequence read]

        rsreset
vcTonPer:	rs.w	1	; 0
vcTgtPer:	rs.w	1	; 2
vcCurPer:	rs.w	1	; 4
vcStpPer:	rs.w	1	; 6
vcTremIdx:	rs.w	1	; 8
vcTremStp:	rs.w	1	; 10
vcTremVal:	rs.w	1	; 12
vcVol:		rs.b	1	; 14
vcVolMul:	rs.b	1	; 15

vcAdsrPtr:	rs.l	1	; 16
vcAdsrIdx:	rs.w	1	; 20
vcAdsrSus:	rs.w	1	; 22
vcAdsrDec:	rs.w	1	; 24
vcAdsrRel:	rs.w	1	; 26

vcPitchPtr:	rs.l	1	; 28
vcPitchIdx:	rs.w	1	; 32
vcPitchSus:	rs.w	1	; 34
vcPitchDec:	rs.w	1	; 36
vcPitchRel:	rs.w	1	; 38
	
vcPitchFlag:	rs.b	1	; 40 ? or maybe release flag ?
vcByte41:	rs.b	1	; 41 ? unused ?

vcWord42:	rs.w	1	; 42 ? Written from INFO but never used
vcInsTyp:	rs.b	1	; 44 #0:tone #1:noise #2-3:buzz
vcBuzCtl:	rs.b	1	; 45 envelop shape and control bits

vcEnvPer:	rs.w	0	; 46 YM envelop period 
vcEnvPerH:	rs.b	1	; 46 YM envelop period MSB
vcEnvPerL:	rs.b	1	; 47 YM envelop period LSB

vcTransX:	rs.w	1	; 48
vcTransY:	rs.w	1	; 50

vcDeltaPerX:	rs.w	1	; 52
vcDeltaPerY:	rs.w	1	; 54
	
vcSpecVal:	rs.b	1	; 56
vcInstNum:	rs.b	1	; 57

vcHwBuzz:	rs.b	1	; 58
vcByte59:	rs.b	1	; 59 unused ? align 

vcXtFlags:	rs.l	0	; 60 extended flags
vcXtNoiFlag:	rs.b	1	; 60
vcXtTonFlag:	rs.b	1	; 61
vcXtWavFlag:	rs.b	1	; 62
vcTFMX:		rs.b	1	; 63

vcXtNoiPtr:	rs.l	1	; 64
vcXtTonPtr:	rs.l	1	; 68
vcXtWavPtr:	rs.l	1	; 72

vcArpIdx:	rs.b	1	; 76
vcDatIdx:	rs.b	1	; 77
	
vcXtSets:	rs.l	0	; 78
vcXtTonSet:	rs.b	1	; 78
vcXtNoiset:	rs.b	1	; 79
vcXtBuzSet:	rs.b	1	; 80
vcByte81:	rs.b	1	; 81
vcSpecIdx:	rs.b	1	; 82
vcBuzzTr:	rs.b	1	; 83
vcSize:		rs.b    0

;; vcSize = 84
	
;;; $$$ ben: Why are these buffers in that order ?
Voices:	
VoiceB: 	DS.B    vcSize
VoiceC:    	DS.B    vcSize
VoiceA:    	DS.B    vcSize

;;; ben: Replaced this buffer by a simple pointer instead of copying
;;;      the whole instrument info block.
        ;; BufInfo:     DS.B    252
CurInfo:        DC.L    0       ; Pointer to info block

;;; $$$ ben: FIXME: without this guardband we have a memory commando
;;;     attack on the next variable ! This need to be sorted out as it
;;;     is obviously nasty bug.
;;;
;;; $$$ ben: Seems like a bug mess up with the voice structure size
;;;     and some access not relative to the voice struct itself.
;;;     Should be fixed now (see 82 vs 84).
                ;; DS.W 2

CurSongPos:     DC.W    0
CurPatNum:      DC.W    0

;;; $$$ ben: changed this table to 128 entry insead of 120 as in the
;;;     code the index is from 0 to 127 (unless something prevent it,
;;;     It's a safe change anyway.
PatPtrTbl:      ds.l    128	; Pointer to pattern data
CurPatPtr:      DC.L    0
PatPos:         DC.W    0
PatPtr          DC.L    0
VoidSeq:	DS.W    5
NoteCount:      DC.W    0


;;; ben: added to remove self modified code and make PCR
ReNoteCntPtr:   DC.L    0
WavFormPtr:     DC.L    0
SpeedPtr:       DC.L    0
PattBasePtr:    DC.L    0
SMC9E:          DC.L    0
RePatPtr:       DC.L    0
ReCurPatPtr:    DC.L    0
SongBasePtr:    DC.L    0
SongRePtr:      DC.L    0
InstBasePtr:    DC.L    0
SoundDataPtr:   DC.L    0       ; sound
VoiceSetPtr:    DC.L    0       ; vset

SongLen:        DC.W    0       ; Number of song position


merk_d1:        DC.B $00,$00
	;; offset:         DC.B $00,$00
	
PatBreakFlag:   DC.B $00,$00    ; Pattern Break command
what:           DC.B $00,$00    ; Speed change flag
merk_speed:     DC.B $00,$00    ; Speed merken
   
	
ReplayFlag:     DC.B    0       ; 0:off


