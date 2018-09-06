void clear();

void kmain(void)
{
 const char *str = "1 Hola Mundo... \n Grupo Debian \n Coordinador: Pablo Villagran ";
 char *vidptr = (char*)0xb8000; // array de caracteres en pantalla
 unsigned int i = 0;
 unsigned int j = 0;
 unsigned int n = 1;

 clear();

 /* Escritura de mensaje */
 while(str[j] != '\0') {
  if(str[j] == '\n')
  {//si el ascii es 10 (nueva linea)
   i = (80*2*n)-2; //agrega el ancho de la fila al cursor
   ++n;
  }
  else
  {
   vidptr[i] = str[j];//Asigna valor de caracter
   vidptr[i+1] = 0x0a;//Indica color de pantalla
   i = i + 2;
  }
  ++j;
 }
 return;
}

void clear(){
 char *vidptr = (char*)0xb8000; // array de caracteres en pantalla
 unsigned int j = 0;
 /*Limpia la pantalla*/
 while(j < 80 * 25 * 2) {
  vidptr[j] = ' ';
  vidptr[j+1] = 0x07;
  j = j + 2;
 }
}

