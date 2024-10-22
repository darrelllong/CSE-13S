	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 11, 3
	.globl	_roll                           ; -- Begin function roll
	.p2align	2
_roll:                                  ; @roll
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	_random
	mov	x8, #18725
	movk	x8, #9362, lsl #16
	movk	x8, #37449, lsl #32
	movk	x8, #18724, lsl #48
	smulh	x8, x0, x8
	lsr	x9, x8, #1
	add	x8, x9, x8, lsr #63
	sub	x8, x8, x8, lsl #3
	add	x8, x0, x8
Lloh0:
	adrp	x9, _die@PAGE
Lloh1:
	add	x9, x9, _die@PAGEOFF
	ldr	w0, [x9, x8, lsl #2]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_next                           ; -- Begin function next
	.p2align	2
_next:                                  ; @next
	.cfi_startproc
; %bb.0:
	add	w8, w0, #1                      ; =1
	udiv	w9, w8, w1
	msub	w0, w9, w1, w8
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #224                    ; =224
	stp	x28, x27, [sp, #128]            ; 16-byte Folded Spill
	stp	x26, x25, [sp, #144]            ; 16-byte Folded Spill
	stp	x24, x23, [sp, #160]            ; 16-byte Folded Spill
	stp	x22, x21, [sp, #176]            ; 16-byte Folded Spill
	stp	x20, x19, [sp, #192]            ; 16-byte Folded Spill
	stp	x29, x30, [sp, #208]            ; 16-byte Folded Spill
	add	x29, sp, #208                   ; =208
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
Lloh2:
	adrp	x8, ___stack_chk_guard@GOTPAGE
Lloh3:
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
Lloh4:
	ldr	x8, [x8]
Lloh5:
	adrp	x9, l_.str.10@PAGE
Lloh6:
	add	x9, x9, l_.str.10@PAGEOFF
	stur	x8, [x29, #-88]
Lloh7:
	adrp	x8, l_.str.11@PAGE
Lloh8:
	add	x8, x8, l_.str.11@PAGEOFF
Lloh9:
	adrp	x10, l_.str.12@PAGE
Lloh10:
	add	x10, x10, l_.str.12@PAGEOFF
	stp	x9, x8, [sp, #80]
Lloh11:
	adrp	x8, l_.str.13@PAGE
Lloh12:
	add	x8, x8, l_.str.13@PAGEOFF
Lloh13:
	adrp	x9, l_.str.14@PAGE
Lloh14:
	add	x9, x9, l_.str.14@PAGEOFF
	stp	x10, x8, [sp, #96]
	str	x9, [sp, #112]
	str	xzr, [sp, #64]
	movi.2d	v0, #0000000000000000
	stp	q0, q0, [sp, #32]
Lloh15:
	adrp	x0, l_.str.15@PAGE
Lloh16:
	add	x0, x0, l_.str.15@PAGEOFF
	bl	_printf
	add	x8, sp, #28                     ; =28
	str	x8, [sp]
Lloh17:
	adrp	x0, l_.str.16@PAGE
Lloh18:
	add	x0, x0, l_.str.16@PAGEOFF
	bl	_scanf
Lloh19:
	adrp	x19, ___stderrp@GOTPAGE
Lloh20:
	ldr	x19, [x19, ___stderrp@GOTPAGEOFF]
	cmp	w0, #1                          ; =1
	b.ne	LBB2_2
; %bb.1:
	ldr	w8, [sp, #28]
	sub	w8, w8, #2                      ; =2
	cmp	w8, #9                          ; =9
	b.lo	LBB2_3
LBB2_2:
	ldr	x3, [x19]
Lloh21:
	adrp	x0, l_.str.17@PAGE
Lloh22:
	add	x0, x0, l_.str.17@PAGEOFF
	mov	w1, #44
	mov	w2, #1
	bl	_fwrite
	mov	w8, #2
	str	w8, [sp, #28]
LBB2_3:
	str	xzr, [sp, #16]
Lloh23:
	adrp	x0, l_.str.18@PAGE
Lloh24:
	add	x0, x0, l_.str.18@PAGEOFF
	bl	_printf
	add	x8, sp, #16                     ; =16
	str	x8, [sp]
Lloh25:
	adrp	x0, l_.str.19@PAGE
Lloh26:
	add	x0, x0, l_.str.19@PAGEOFF
	bl	_scanf
	cmp	w0, #1                          ; =1
	b.ne	LBB2_5
; %bb.4:
	ldr	x0, [sp, #16]
	lsr	x8, x0, #32
	cbz	x8, LBB2_6
LBB2_5:
	ldr	x3, [x19]
Lloh27:
	adrp	x0, l_.str.20@PAGE
Lloh28:
	add	x0, x0, l_.str.20@PAGEOFF
	mov	w1, #41
	mov	w2, #1
	bl	_fwrite
	mov	w0, #2021
LBB2_6:
                                        ; kill: def $w0 killed $w0 killed $x0
	bl	_srandom
	mov	w8, #0
Lloh29:
	adrp	x19, l_.str.21@PAGE
Lloh30:
	add	x19, x19, l_.str.21@PAGEOFF
	mov	x22, #18725
	movk	x22, #9362, lsl #16
	movk	x22, #37449, lsl #32
	movk	x22, #18724, lsl #48
Lloh31:
	adrp	x23, _die@PAGE
Lloh32:
	add	x23, x23, _die@PAGEOFF
Lloh33:
	adrp	x24, l___const.main.scores@PAGE
Lloh34:
	add	x24, x24, l___const.main.scores@PAGEOFF
	add	x25, sp, #32                    ; =32
	add	x26, sp, #80                    ; =80
Lloh35:
	adrp	x20, l_.str.22@PAGE
Lloh36:
	add	x20, x20, l_.str.22@PAGEOFF
	b	LBB2_8
LBB2_7:                                 ;   in Loop: Header=BB2_8 Depth=1
	mov	w0, #10
	bl	_putchar
	ldr	w8, [sp, #28]
	add	w9, w27, #1                     ; =1
	udiv	w10, w9, w8
	msub	w8, w10, w8, w9
	cmp	w28, #100                       ; =100
	b.hs	LBB2_11
LBB2_8:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB2_9 Depth 2
Lloh37:
	adrp	x9, _names@PAGE
Lloh38:
	add	x9, x9, _names@PAGEOFF
	ldr	x9, [x9, w8, uxtw #3]
	mov	w27, w8
	str	x9, [sp]
	mov	x0, x19
	bl	_printf
LBB2_9:                                 ;   Parent Loop BB2_8 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	bl	_random
	smulh	x8, x0, x22
	asr	x9, x8, #1
	add	x8, x9, x8, lsr #63
	sub	x8, x8, x8, lsl #3
	add	x21, x0, x8
	ldr	w8, [x23, x21, lsl #2]
	ldr	w9, [x24, x8, lsl #2]
	lsl	x10, x27, #2
	ldr	w11, [x25, x10]
	add	w28, w11, w9
	str	w28, [x25, x10]
	ldr	x8, [x26, x8, lsl #3]
	str	x8, [sp]
	mov	x0, x20
	bl	_printf
	cmp	x21, #2                         ; =2
	b.lo	LBB2_7
; %bb.10:                               ;   in Loop: Header=BB2_9 Depth=2
	cmp	w28, #100                       ; =100
	b.lo	LBB2_9
	b	LBB2_7
LBB2_11:
Lloh39:
	adrp	x8, _names@PAGE
Lloh40:
	add	x8, x8, _names@PAGEOFF
	ldr	x8, [x8, x27, lsl #3]
	add	x9, sp, #32                     ; =32
	ldr	w9, [x9, x27, lsl #2]
	stp	x8, x9, [sp]
Lloh41:
	adrp	x0, l_.str.24@PAGE
Lloh42:
	add	x0, x0, l_.str.24@PAGEOFF
	bl	_printf
	ldur	x8, [x29, #-88]
Lloh43:
	adrp	x9, ___stack_chk_guard@GOTPAGE
Lloh44:
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
Lloh45:
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	LBB2_13
; %bb.12:
	mov	w0, #0
	ldp	x29, x30, [sp, #208]            ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #192]            ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #176]            ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #160]            ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #144]            ; 16-byte Folded Reload
	ldp	x28, x27, [sp, #128]            ; 16-byte Folded Reload
	add	sp, sp, #224                    ; =224
	ret
LBB2_13:
	bl	___stack_chk_fail
	.loh AdrpLdrGot	Lloh19, Lloh20
	.loh AdrpAdd	Lloh17, Lloh18
	.loh AdrpAdd	Lloh15, Lloh16
	.loh AdrpAdd	Lloh13, Lloh14
	.loh AdrpAdd	Lloh11, Lloh12
	.loh AdrpAdd	Lloh9, Lloh10
	.loh AdrpAdd	Lloh7, Lloh8
	.loh AdrpAdd	Lloh5, Lloh6
	.loh AdrpLdrGotLdr	Lloh2, Lloh3, Lloh4
	.loh AdrpAdd	Lloh21, Lloh22
	.loh AdrpAdd	Lloh25, Lloh26
	.loh AdrpAdd	Lloh23, Lloh24
	.loh AdrpAdd	Lloh27, Lloh28
	.loh AdrpAdd	Lloh35, Lloh36
	.loh AdrpAdd	Lloh33, Lloh34
	.loh AdrpAdd	Lloh31, Lloh32
	.loh AdrpAdd	Lloh29, Lloh30
	.loh AdrpAdd	Lloh37, Lloh38
	.loh AdrpLdrGotLdr	Lloh43, Lloh44, Lloh45
	.loh AdrpAdd	Lloh41, Lloh42
	.loh AdrpAdd	Lloh39, Lloh40
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"Wilbur"

l_.str.1:                               ; @.str.1
	.asciz	"Charlotte"

l_.str.2:                               ; @.str.2
	.asciz	"John"

l_.str.3:                               ; @.str.3
	.asciz	"Fern"

l_.str.4:                               ; @.str.4
	.asciz	"Templeton"

l_.str.5:                               ; @.str.5
	.asciz	"Avery"

l_.str.6:                               ; @.str.6
	.asciz	"Homer"

l_.str.7:                               ; @.str.7
	.asciz	"Henry"

l_.str.8:                               ; @.str.8
	.asciz	"Dr. Dorian"

l_.str.9:                               ; @.str.9
	.asciz	"Aranea"

	.section	__DATA,__data
	.globl	_names                          ; @names
	.p2align	3
_names:
	.quad	l_.str
	.quad	l_.str.1
	.quad	l_.str.2
	.quad	l_.str.3
	.quad	l_.str.4
	.quad	l_.str.5
	.quad	l_.str.6
	.quad	l_.str.7
	.quad	l_.str.8
	.quad	l_.str.9

	.section	__TEXT,__const
	.globl	_die                            ; @die
	.p2align	2
_die:
	.long	0                               ; 0x0
	.long	0                               ; 0x0
	.long	1                               ; 0x1
	.long	2                               ; 0x2
	.long	3                               ; 0x3
	.long	4                               ; 0x4
	.long	4                               ; 0x4

	.p2align	2                               ; @__const.main.scores
l___const.main.scores:
	.long	0                               ; 0x0
	.long	0                               ; 0x0
	.long	10                              ; 0xa
	.long	10                              ; 0xa
	.long	15                              ; 0xf
	.long	5                               ; 0x5
	.long	5                               ; 0x5

	.section	__TEXT,__cstring,cstring_literals
l_.str.10:                              ; @.str.10
	.asciz	"on side"

l_.str.11:                              ; @.str.11
	.asciz	"on back"

l_.str.12:                              ; @.str.12
	.asciz	"upright"

l_.str.13:                              ; @.str.13
	.asciz	"on snout"

l_.str.14:                              ; @.str.14
	.asciz	"on ear"

l_.str.15:                              ; @.str.15
	.asciz	"How many players? "

l_.str.16:                              ; @.str.16
	.asciz	"%d"

l_.str.17:                              ; @.str.17
	.asciz	"Invalid number of players. Using 2 instead.\n"

l_.str.18:                              ; @.str.18
	.asciz	"Random seed: "

l_.str.19:                              ; @.str.19
	.asciz	"%lld"

l_.str.20:                              ; @.str.20
	.asciz	"Invalid random seed. Using 2021 instead.\n"

l_.str.21:                              ; @.str.21
	.asciz	"%s rolls the pig..."

l_.str.22:                              ; @.str.22
	.asciz	" pig lands %s"

l_.str.24:                              ; @.str.24
	.asciz	"%s wins with %u points!\n"

.subsections_via_symbols
