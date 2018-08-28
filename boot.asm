;;boot.asn

bits 32			;directiva nasn -32 bits
section	.text
	;especificación de arranque múltiple
	align 4
	dd 0x1BADB002		 ;magico
	dd 0x00			 ;banderas
	dd - (0x1BADB002 + 0x00) ;suma de comprobación. m + f + c debe ser cero

global start

extern kmain		;kmain se define en el archivo c

start:
  cli			;interrupciones de bloque
  mov esp, stack_space	;establecer puntero de pila
  call kmain
  hlt			;detener la CPU

section .bss
stack_space resb 8192	;8KB para la pila
