nasm -f bin -o start.bin start.asm
nasm -f bin -o main.bin main.asm
copy start.bin+main.bin funny.img
