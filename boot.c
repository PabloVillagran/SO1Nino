/*
* kernel.c
*/
void kmain(void)
{
	const char *str = "Bienvenido al sistema operativo: hola";
	char *vidptr = (char*)0xb8000; //Nuevo video comienza aquí.
	unsigned int i = 0;
	unsigned int j = 0;

	/* Este ciclo limpia la pantalla
	* Hay 25 líneas cada una de 80 columnas; cada elemento toma 2 bytes
	while (j < 80 * 25 * 2){
		/* Caracter en blanco*/
		vidptr[j]=' ';
		/* Atributo-byte: Gris claro en la pantalla negra */
		vidptr[j+1] = 0x07;
		j = j + 2;
	}

	j = 0;

	/* Este ciclo escribe la cadena en la memoria de video */
	while (str[i] != '\0'){
		/* El ASCII del carácter */
		vidptr[i] = str[j];
		/* Atributo-byte: dar carácter negro gb y gris claro fg */
		vidptr[i+1] = 0x07;
		++j;
		i = i +2;
	}
	return;
}
