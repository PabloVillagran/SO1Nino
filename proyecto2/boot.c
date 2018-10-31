void clear();
void draw();
void print(char *str);
void debugKeyPress();
int strLenght(char *str);
char * itoa(int value, char *result, int base);
void printc(char c, int pos);
char charByCode(int scancode);
void cursor();

#define OUTPUT_COLOR 0x0a
#define BLINK_OUTPUT 0X8a
#define DBCOLOR      0x17

#include <sys/io.h>

_Bool shiftPressed;
_Bool altPressed;
_Bool capsLock;
_Bool numLock;
_Bool debug;

char * vm;
char * input;
char * output;

unsigned int scancode;
unsigned char keyStatus;

unsigned int position;

void kmain(void)
{
 vm = (char*)0xb8000; // array de caracteres en pantalla
 clear();
 debug = 1;
 input = "";
 output = "Wooffer >";
 position = strLenght(output);
 draw();
 while(1){
  scancode = inb(0x60); //puerto de recepcion de datos en teclado
  keyStatus = inb(0x64); //puerto de estado de teclado
  
  if(keyStatus & 0x01){
   char cbCode = charByCode(scancode);
   if(cbCode!='\0')
   {
    input = input+cbCode;
    printc(cbCode, position);
    position++;
    cursor();
   }
  }
  
//  draw(); //dibuja resultado en pantalla;
  if(debug)debugKeyPress();
 }
 return;
}

void printc(char c, int pos){
 vm[pos*2] = c;
 vm[(pos*2)+1] = OUTPUT_COLOR;
}

void draw(){
 unsigned int j = 0;
 unsigned int i = 0;
 unsigned int n = 1;
 /* Escritura de mensaje */
 while(output[j] != '\0') {
  if(output[j] == '\n')
  {//si el ascii es 10 (nueva linea)
   position = (80*2*n)-2; //agrega el ancho de la fila al cursor
   ++n;
  }
  else
  {
   printc(output[j], i);
   i++;
  }
  ++j;
 }
 cursor(i);
}

void debugKeyPress(){
 vm[1997*2] = keyStatus;
 vm[1997*2+1] = DBCOLOR;
 char * valor = itoa(scancode, "", 16);
 int vLenght = strLenght(valor);
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

void cursor(int pos){
 vm[(position+1)*2] = 219;
 vm[(position+1)*2+1] = BLINK_OUTPUT; 
}

//Funcion que cuenta la cantidad de espacios en un string
int strLenght(char *str){
 int l = 0;
 while (str[l]){
  l++;
 }
 return l;
}

//Funcion que convierte de entero a string
char * itoa (int value, char *result, int base)
{
    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

//Limpia la pantalla
void clear(){
 unsigned int j = 0;
 /*Limpia la pantalla*/
 while(j < 80 * 25 * 2) {
  vm[j] = ' ';
  vm[j+1] = 0x07;
  j = j + 2;
 }
}

char charByCode(int scancode){
 if(scancode == 0x02)return '1';
 if(scancode == 0x03)return '2';
 if(scancode == 0x04)return '3';
 if(scancode == 0x05)return '4';
 if(scancode == 0x06)return '5';
 if(scancode == 0x07)return '6';
 if(scancode == 0x08)return '7';
 if(scancode == 0x09)return '8';
 if(scancode == 0x0a)return '9';
 if(scancode == 0x0b)return '0';

 if(scancode == 0x10)return 'q';
 if(scancode == 0x11)return 'w';
 if(scancode == 0x12)return 'e';
 if(scancode == 0x13)return 'r';
 if(scancode == 0x14)return 't';
 if(scancode == 0x15)return 'y';
 if(scancode == 0x16)return 'u';
 if(scancode == 0x17)return 'i';
 if(scancode == 0x18)return 'o';
 if(scancode == 0x19)return 'p';

 if(scancode == 0x1e)return 'a';
 if(scancode == 0x1f)return 's';
 if(scancode == 0x20)return 'd';
 if(scancode == 0x21)return 'f';
 if(scancode == 0x22)return 'g';
 if(scancode == 0x23)return 'h';
 if(scancode == 0x24)return 'j';
 if(scancode == 0x25)return 'k';
 if(scancode == 0x26)return 'l';

 if(scancode == 0x2c)return 'z';
 if(scancode == 0x2d)return 'x';
 if(scancode == 0x2e)return 'c';
 if(scancode == 0x2f)return 'v';
 if(scancode == 0x30)return 'b';
 if(scancode == 0x31)return 'n';
 if(scancode == 0x32)return 'm';
 
 return '\0';
}
