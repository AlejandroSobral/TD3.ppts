%include        "header.h"


global integer2BCD
global BCD2ascii
global memcopy
section  .text
;/**
;* integer2BCD
;* Recibe:
;* En rdi el entero a convertir
;* En rsi un puntero al buffer en el que almacenará el resultado 
;* que será la string con el valor Binario codificado a Decimal (BCD)
;* terminada en Retorno de carro lista para ser enviada a stdout.
;* Devuelve en rax la cantidad de bytes que ocupa el número convertido 
;*/
integer2BCD:
	push	rdx
	push	rbx

	mov     byte [rsi+int64_nDigits-1],10 ;Finaliza en '\n' el buffer para el número BCD
	mov	rbx,10	;divisor (Divide por 10 para expresarlo en decimal)
	mov	rax,rdi	;
again:
	xor	rdx,rdx
	div	rbx	;rbx:rax/rbx	==> rax=Cociente rdx=Resto
	mov	byte [rsi+int64_nDigits-2],dl
	dec	rsi
	cmp	rax,0	; Si el cociente es 0 terminamos
	jnz	again

	mov rax,rsi
	add rax,int64_nDigits-1	;Valor de retorno el rax. Puntero al dato
	
	pop	rbx
	pop	rdx
	ret

;/**
;* BCD2ascci
;* Recibe:
;* rdi: Puntero a una string BCD desempaquetado terminada en NULL (0)
;* Devuelve:
;* rax: Cantidad de bytes convertidos
;*/

BCD2ascii:
	xor	rax,rax		;Inicializa contador de bytes convertidos rax = 0.
next:
	cmp	byte [rdi+rax],10	;Si el contenido del byte apuntado por rdi es '\n' terminó
	je	listo
	add	byte [rdi+rax],0x30 	;Si no llegó el fin de string o '\n', convierte a ASCII
	inc	rax		;Siguiente elemento
	jmp	next
listo:
	inc	rax		;El '\n'cuenta como caracter de la cadena
	ret


;/**
;*memcopy
;*Recibe 
;*rdi: Puntero a la dirección destino
;*rsi: Puntero a la dirección origen
;*rdx: Cantidad de bytes a copiar
;* Copia los rdx bytes desde rsi a rdi
;* Devuelve cantidad de elementos copiados en rax
;*/
memcopy:
	push 	rcx
	pushfq
	cld
	mov 	rcx,rdx
	repnz	movsb 
	mov	rax,rdx
	popfq
	pop	rcx
	ret
