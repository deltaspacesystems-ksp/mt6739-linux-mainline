// Mikro-"kernel" arm64 do testu lancucha rozruchu LK -> nasz kod (bez Linuksa).
// Jesli po wgraniu telefon NIE restartuje sie po ok. 30 s, to LK skoczyl do tego kodu (watchdog zostal wylaczony).
// Dodatkowo probuje zamalowac na bialo framebuffer LK (0x771e0000).
	.section .text
	.globl	_start
_start:
	b	entry				// code0: skok za naglowek
	.word	0				// code1
	.quad	0x80000			// text_offset (jak stock)
	.quad	25227264			// image_size (jak stock)
	.quad	0xa				// flags (jak stock)
	.quad	0				// res2
	.quad	0				// res3
	.quad	0				// res4
	.ascii	"ARM\x64"			// magic naglowka Image
	.word	0				// res5

entry:
	// 1) wylacz watchdog TOPRGU (MediaTek: MODE = KEY(0x22) <<24 | ENABLE=0)
	ldr	x0, =0x10007000
	ldr	w1, =0x22000000
	str	w1, [x0]

	// 2) wypelnij pierwsza strone framebuffera LK na bialo
	ldr	x0, =0x771e0000
	ldr	x1, =(2944 * 1440 / 8)
	mov	x2, #-1
1:	str	x2, [x0], #8
	subs	x1, x1, #1
	b.ne	1b

	// 3) zatrzymaj sie (watchdog juz wylaczony)
2:	wfe
	b	2b

	.ltorg
	.balign	8
_end:
