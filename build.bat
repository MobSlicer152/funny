nasm -f bin -o %~dp0boot.bin %~dp0boot.asm
nasm -f bin -o %~dp0startup.bin %~dp0startup.asm
del %~dp0*.o
for /f "usebackq tokens=*" %%x in (`dir /b %~dp0*.c`) do (
	clang --target=i386-unknown-elf -ffreestanding -c -o %%x.o %%x
)

ld.lld -o %~dp0main.elf -nostdlib %~dp0*.o
llvm-objcopy -O binary %~dp0main.elf %~dp0main.bin 
python %~dp0diskimg.py funny.img

