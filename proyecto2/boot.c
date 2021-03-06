void clear();
void draw();
void print(char *str);
void debugKeyPress();
long atoi(char * c);
char * itoa(long value, char *result, int base);
void printc(char c, int pos);
char charByCode(int scancode);
void cursor();
void backSpace();
void concat(char s1[], char s2[]);
int strlen(char * str);
char * funBegins(char * inp);
int equals(char * a, char * b);
char * sumar(char * s1, char * s2);
char * multi(char * s1, char * s2);
char * divin(char * s1, char * s2);
char * powe(char * s1, char * s2);
void  *memset(void *b, int c, int len);
void * clean(char * t);

void beep();
static void play_sound(int nFrecuence);
static void nosound();

extern int reboot();

#define OUTPUT_COLOR 0x0a
#define BLINK_OUTPUT 0X8a
#define DBCOLOR      0x17

#include <sys/io.h>
//include <string.h>

_Bool shiftPressed;
_Bool altPressed;
_Bool capsLock;
_Bool numLock;
_Bool debug;

char * vm;
char * output;
char * input;

unsigned int scancode;
unsigned char keyStatus;

unsigned int position;
unsigned int initPos;
unsigned int n;

// Metodo principal.
int kmain(void) {
	vm = (char*)0xb8000; // array de caracteres en pantalla.
	clear();
	debug = 0;
	n = 1;
	output = "Wooffer >";
	// position = strlen(output);
	draw();
	initPos = position;
	input = " ";
	while(1) { 
		if(inb(0x64) & 0x01) {
			scancode = inb(0x60); // Puerto de recepcion de datos en teclado.
			char cbCode = charByCode(scancode);
			if(cbCode!='\0') {
				if(cbCode==8) {
					backSpace();
					cursor();
				} else if(cbCode == '\n') {
					// Limpia cursor.
					vm[position*2] = ' ';
					vm[position*2+1] = 0x01;
					// Captura y logica.
					int len = position - initPos;
					position = initPos;
					char in[len];
					int k;
					for(k = 0; k < len; k++) {
						in[k] = vm[position*2];
						position++;
					}
					in[k] = '\0';
					input = in;
					if(n>24) {
						clear();
						n=1;
						position = 0;
					}
					position = (80*n);
					++n;
					print(input);
					position = (80*n);
					++n;
					char * inp = funBegins(input);
					print(inp);
					if(equals(inp, "REBOOT"))
					reboot();
					if(equals(inp, "APAGAR"))
					break;
					
					// Reset.
					vm[position*2] = ' ';
					vm[position*2+1] = 0x01;
					position = (80*n);
					++n;
					draw();
					initPos = position;
				}else{
					printc(cbCode, position);
					position++;
					cursor();
				}
			}
		}
	}
	
	position++;
	print("*DEAD*");
	clear();
	// INSERTAR LLAMADA PARA EL APAGADO DEL SISTEMA
	// reboot();
	return 0;
}

// Funcion que registra los valores que ingresamos (es como el menu).
char * funBegins(char * inp) {
    int tokenEnd[3] = {0, 0, 0};
    int p = 0;
    int q = 0;
    while(inp[p]!='\0') {
        if(inp[p]==' ') {
            q++;
        }
        tokenEnd[q] = p++;
    }
    
    char t1[tokenEnd[0]];
    char t2[tokenEnd[1]-tokenEnd[0]];
    char t3[tokenEnd[2]-tokenEnd[1]-1];
    clean(t1);
    clean(t2);
    clean(t3);
    for(p=0;p<=tokenEnd[0];p++) {
        t1[p] = inp[p]; 
    }
    position++;
    t1[p++] = '\0';
    for(q=0;q<tokenEnd[1]-tokenEnd[0]-1;q++) {
        t2[q] = inp[p++];
    }
    position++;
    t2[p++] = '\0';
    for(q=0;q<tokenEnd[2]-tokenEnd[1]-1;q++) {
        t3[q] = inp[p++];
    }
    position++;
    t3[p++] = '\0';
    
    if(equals(t1, "sum")) {
		return sumar(t2, t3); // Busca la funcion para hacer la suma o la resta.
    }else if(equals(t1, "prod")) {
		return multi(t2, t3); // Busca la funcion para hacer la multiplicacion.
    }else if(equals(t1, "div")) {
		return divin(t2, t3); // Busca la funcion para hacer la division.
    }else if(equals(t1, "pow")) {
		return powe(t2, t3); // Busca la funcion para hacer la potencia.
    }else if(equals(t1, "playdead")) {
		return "REBOOT"; 
    }else if(equals(t1, "die")) {
		return "APAGAR";
    }else if(equals(t1, "bark")) {
		beep();
		return "WOOF";// Reproducir sonidos??
    }else{
		return "No se reconoce la accion";
    }
}

// Funcion que realiza la suma o la resta.
char * sumar(char * s1, char * s2){
	return itoa(atoi(s1) + atoi(s2),"", 10);
}

// Funcion que realiza la multiplicacion.
char * multi(char * s1, char * s2){
	return itoa(atoi(s1) * atoi(s2), "", 10);
}

// Funcion que realiza la division.
char * divin(char * s1, char * s2) {
	int i2 =atoi(s2);
	if(i2 == 0) { 
		return "DIV/0";
	} else {
		int i1 = atoi(s1); 
		if((i1%i2)==0) {
			return itoa(i1 / i2, "", 10);
		} else{
		print("mod ");
		return itoa(i1 % i2, "", 10);}
	}
}

// Funcion que realiza la potencia
char * powe(char * s1, char * s2) {
	long base = atoi(s1);
	long expo = atoi(s2);
	long result = 1;
	while(expo!= 0) {
		result *= base;
		expo--;
	}
	return itoa(result, "", 10);
}

// Funcion que convierte una cadena a entero. 
long atoi(char * c) {
    unsigned long l = strlen(c);
    long result = 0;
    long i = 0;
    int boo = 0;
    //for(int i=0; i<l; i++){
    while(c[i]!='\0') {
        if(c[i]=='-')
			boo=1;
        else 
			result = result * 10 + ( c[i] - '0' );
        i++;
    }
    if(boo) {
		result = result * -1;
    }
    return result;
}

// Funcion que valida que dos cadenas sean iguales.
int equals(char * a, char * b){
	unsigned int la = strlen(a);
	unsigned int lb = strlen(b);
	if(la != lb){ 
		return 0;
	}else{
		la = 0;
		while(a[la]!='\0'){
			if(a[la]!=b[la]) 
				return 0;
			la++;
		}
	}
	return 1;
}

// Funcion que procesa los valores que va a imprimir en pantalla.
void print(char *str){
	unsigned int j = 0;
	while(str[j] != '\0') {
		printc(str[j], position);
		position++;
		++j;
	}
}

// Funcion que imprime valor en pantalla.
void printc(char c, int pos){
	vm[pos*2] = c;
	vm[(pos*2)+1] = OUTPUT_COLOR;
}

// Regresa un espacio.
void backSpace(){
	if(position > initPos){
		printc(' ', position);
		position--;
		printc(' ', position);
	}
}

// Dibuja en la pantalla "Wooffer >"
void draw(){
	unsigned int j = 0;
	unsigned int i = 0;
	/* Escritura de mensaje */
	while(output[j] != '\0') {
		printc(output[j], position);
		position++;
		++j;
	}
	cursor(i);
}

// Dibujo o imprime el codigo hexadecimal de la tecla presionada.
void debugKeyPress(){
	vm[1997*2] = keyStatus;
	vm[1997*2+1] = DBCOLOR;
	char * valor = itoa(scancode, "", 16);
	int vLenght = strlen(valor);
	if(vLenght<2){
		vm[1998*2] = '0';
		vm[1998*2+1] = DBCOLOR;
		vm[1999*2] = valor[0];
		vm[1999*2+1] = DBCOLOR;
	}else{
		vm[1998*2] = valor[0];
		vm[1998*2+1] = DBCOLOR;
		vm[1999*2] = valor[1];
		vm[1999*2+1] = DBCOLOR;
	}
}

// Muestra la linea que parpadea para indicar donde se va escribir.
void cursor(int pos){
	vm[(position)*2] = 219;
	vm[(position)*2+1] = BLINK_OUTPUT;
}

// Funcion que cuenta la cantidad de espacios en un string.
int strlen(char *str){
	int l = 0;
	while (str[l]){
		l++;
	}
	return l;
}

// Concatena dos cadenas.
void concat(char s1[], char s2[]) {
	int i, j;
	i = strlen(s1);
	for (j = 0; s2[j] != '\0'; i++, j++) {
		s1[i] = s2[j];
	}
	s1[i] = '\0';
}

// Funcion que convierte de entero a string.
char * itoa (long value, char *result, int base) {
    char* ptr = result, *ptr1 = result, tmp_char;
    long tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Aplica Signo Negativo
    if (tmp_value < 0) *ptr++ = '-';
		*ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

// Limpia la pantalla.
void clear(){
	unsigned int j = 0;
	/*Limpia la pantalla*/
	while(j < 80 * 25 * 2) {
		vm[j] = ' ';
		vm[j+1] = 0x07;
		j = j + 2;
	}
}

// Funcion que Hace la conversión de Hexadecimal a un caracter en especifico.
char charByCode(int scancode){
	if(scancode == 0x02)
		return '1';
	if(scancode == 0x03)
		return '2';
	if(scancode == 0x04)
		return '3';
	if(scancode == 0x05)
		return '4';
	if(scancode == 0x06)
		return '5';
	if(scancode == 0x07)
		return '6';
	if(scancode == 0x08)
		return '7';
	if(scancode == 0x09)
		return '8';
	if(scancode == 0x0a)
		return '9';
	if(scancode == 0x0b)
		return '0';
	
	if(scancode == 0x10)
		return 'q';
	if(scancode == 0x11)
		return 'w';
	if(scancode == 0x12)
		return 'e';
	if(scancode == 0x13)
		return 'r';
	if(scancode == 0x14)
		return 't';
	if(scancode == 0x15)
		return 'y';
	if(scancode == 0x16)
		return 'u';
	if(scancode == 0x17)
		return 'i';
	if(scancode == 0x18)
		return 'o';
	if(scancode == 0x19)
		return 'p';
	
	if(scancode == 0x1e)
		return 'a';
	if(scancode == 0x1f)
		return 's';
	if(scancode == 0x20)
		return 'd';
	if(scancode == 0x21)
		return 'f';
	if(scancode == 0x22)
		return 'g';
	if(scancode == 0x23)
		return 'h';
	if(scancode == 0x24)
		return 'j';
	if(scancode == 0x25)
		return 'k';
	if(scancode == 0x26)
		return 'l';
	
	if(scancode == 0x2c)
		return 'z';
	if(scancode == 0x2d)
		return 'x';
	if(scancode == 0x2e)
		return 'c';
	if(scancode == 0x2f)
		return 'v';
	if(scancode == 0x30)
		return 'b';
	if(scancode == 0x31)
		return 'n';
	if(scancode == 0x32)
		return 'm';
	
	if(scancode == 0x35)
		return '-';
	if(scancode == 0x1c)
		return '\n';
	if(scancode == 0x0e)
		return 8;
	if(scancode == 0x39)
		return ' ';
	
	return '\0';
}

// Limpia el valor de las variables.
void *clean(char * t){
    int i = 0;
    while(t[i]!=0){
        t[i] = 0;
        t++;
    }
}

// Reproducir sonido utilizando altavoz integrado
static void play_sound(int nFrequence) {
 	int Div;
 	short tmp;
 
    // Ajuste el PIT a la frecuencia deseada.
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (short) (Div) );
 	outb(0x42, (short) (Div >> 8));
 
    // Y reproduce el sonido utilizando el altavoz de la PC.
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
}
 
 // Hace el shutup.
static void nosound() {
 	short tmp = inb(0x61) & 0xFC;
	outb(0x61, tmp);
}
 
 // Hace un beep.
void beep() {
 	play_sound(1000);
 	for (int c = 1; c <= 32767; c++)
		for (int d = 1; d <= 32767; d++){
			
		}
 	nosound();
    //set_PIT_2(old_frequency);
}

 