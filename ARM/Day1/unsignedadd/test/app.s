
;/* PROGRAM TO ------------------ */

;ARM CODE 
.code 32		;word align
.text	
.global main_asm

;/* Application code for assembly starts here */

main_asm:
	
	mov r1,#4
	mov r2,#5
	add r3,r1,r2 	
	
loop:   b loop
.end 
