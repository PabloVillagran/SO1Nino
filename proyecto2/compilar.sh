nasm -f elf32 boot.asm -o boot.o
echo "Se ha compilado el bootloader en nasm"
gcc -m32 -c boot.c -o bootc.o
echo "Se ha compilado el kernel en C"
ld -m elf_i386 -T link.ld -o ./kernel/boot/kernel boot.o bootc.o
echo "Se han vinculado los archivos"
grub-mkrescue -o boot.iso kernel
echo "Se ha terminado la compilación del kernel"
