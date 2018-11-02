
bits 32	        ;indica a nasm que se debe compilar en 32 bits
section .text	;esta seccion delimita banderas para el booteo en GRUB
      align 4   ;esta bandera indica un 'formato' de 4 espacios al archivo compilado
      dd 0x1BADB002 ;numero que indica a GRUB donde bootear
      dd 0x00   ;indica a GRUB que todos los modulos de booteo deben estar alineados
                ;en un limite de 4KB
      dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero
                ;realiza la operacion sobre los ultimos valores

global start
global reboot
global shutdown_success

extern kmain	;funcion kmain definida en el archivo c
      
start:
        ;funcion principal
	cli       ;cambia una bandera interna para el bloqueo de interrupciones
	mov esp, stack_space ;asigna puntero para pila de instrucciones
	call kmain ;llama al kernel
	je shutdown_success
	

reboot:
	mov al, 0xFE
	out 0x64, al

shutdown_success:
	mov ax, 0x1000
	mov ax, ss
	mov sp, 0xf000
	mov ax, 0x5307
	mov bx, 0x0001
	mov cx, 0x0003
	int 0x15


section .bss
      ;seccion de bits vacios a manera de reservar memoria.
stack_space: resb 8192 ;reserva 8KB para la pila de instrucciones

