LA: EQU 1
;MOD_A: BEGIN
SECTION

;	R: EXTERN	;isto eh um comentario oioioi

;	MOD_B: EXTERN
;	PUBLIC A
;	PUBLIC B
;	PUBLIC L1
	INPUT A
	COPY A, B
	INPUT B
	IF LA
	JMP MOD_B
	JMP MOD_B
	JMP MOD_B
	JMP MOD_B

	L1: OUTPUT R + 1
	STOP
	STOP

SECTION DATA
A: SPACE 1
B: CONST 2
;C: CONST
;END
;
;;;;;;;
;cometariooooooooooooooooooooooooooooooooooooooooooooo
