%include	"header.h"

global _start
extern integer2BCD
extern BCD2ascii
extern memcopy

section .data
msg1: 	DB	'Hola mi PID es: '
l1	EQU	$-msg1
msg2:	DB	CR,'Me voy a dormir 2 segundos',CR
l2	EQU	$-msg2
msg3:	DB	CR,'Aca estoy despierto otra vez.... saliendo... bye!!',CR
l3	EQU	$-msg3
timespec:		;Estructura timespec
	DD	2	;segundos
	DD	0	;nanosegundos

section .bss
pid: 
	RESB	int64_nDigits	;Variable para almacenar el PID
	RESB	1	;Queda preparada para terminar en NULL
			;Útil para manejar cadenas ASCII

section .text
_start:
; Primero ponemos un mensaje con el PID del proceso
	mov	rax,4		;Syscall sys_write()
	mov	rbx,1		;File desscriptor STDOUT_FILENO
	mov	rdx,l1		;Longitud del mensaje
	mov	rcx,msg1 	;Puntero al mensaje
	INT 	0x80

; Obtenemos el PID del proceso con getpid
	mov	rax,0x14	;Syscall getpid
	INT	0x80		;Al regreso rax tiene el PID

;Llamamos a una rutina  para convertir número en string
	mov	byte [pid+int64_nDigits],NULL	;Terminamos en NULL el buffer pid
	mov	rdi,rax		;Argumento #1 PID
	mov	rsi,pid		;Argumento #2 puntero a memoria para almacenar la cadena
	call	integer2BCD
	
	mov	rdi,rax		; Ahora rsi apunta al satring BCD del PID
	call	BCD2ascii

	mov	rsi,rdi		;rsi apunta a la cadena origen
	mov	rdi,pid		;rdi apunta a la cadena destino
	mov	rdx,rax		;rdx = cantidad de caracteres a copiar 
	call 	memcopy 	

;Terminamos de imprimir el mensaje con el valor del PID
	mov	rdx,rax		;Longitud del mensaje
	mov	rax,4		;Syscall write otra vez
	mov	rbx,1		;File desscriptor STDOUT_FILENO
	mov	rcx,pid 	;Puntero al mensaje
	INT 	0x80

;Vamos a imprimir el mensaje para dormir
	mov	rdx,l2		;Longitud del mensaje
	mov	rax,4		;Syscall sys_write() otra vez
	mov	rbx,1		;File desscriptor STDOUT_FILENO
	mov	rcx,msg2 	;Puntero al mensaje
	INT 	0x80

;Dormimos 2 segundos 
	mov	rax,0xa2	;syscall sys_nanosleep()
	mov	rbx,timespec	;rbx = puntero a estructura timespec
	mov	rcx,NULL	;rch apunta a NULL
	INT	0x80

;Vamos a imprimir el mensaje para despertar
	mov	rdx,l3		;Longitud del mensaje
	mov	rax,4		;Syscall sys_write() otra vez
	mov	rbx,1		;File desscriptor STDOUT_FILENO
	mov	rcx,msg3 	;Puntero al mensaje
	INT 	0x80

;Finalmente volvemos el control al proceso padre con exit ()
	mov	rax,1
	mov	rbx,0		;código de retorno
	INT	0x80

