LA: EQU 1
MOD_A: BEGIN

SECTION TEXT


	R: EXTERN	;isto eh um comentario oioioi


	MOD_B: MOD_B: EXTERN
	PUBLIC A
	PUBLIC B
	PUBLIC L1
	INPUT A
	COPY A, B
	INPUT B
	IF LA
	JMP MOD_B
	L1: OUTPUT R + 1
	STOP

SECTION DATA
A: SPACE 1
B: SPACE 1
END
;
;;;;;;;
;cometariooooooooooooooooooooooooooooooooooooooooooooo
