// Payload wstawiany w stockowy Image pod offset 0x40 (zamiast stext). Kod wzgledny (pozycyjnie niezalezny).
// Wylacza watchdog TOPRGU, zamalowuje framebuffer LK na bialo i zatrzymuje CPU. Uzywa tylko x0-x2.
	.section .text
	.globl	_start
_start:
	ldr	x0, =0x10007000
	ldr	w1, =0x22000000
	str	w1, [x0]
	ldr	x0, =0x771e0000
	ldr	x1, =(2944 * 1440 / 8)
	mov	x2, #-1
1:	str	x2, [x0], #8
	subs	x1, x1, #1
	b.ne	1b
2:	wfe
	b	2b
	.ltorg
