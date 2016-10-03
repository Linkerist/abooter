objs := button.o interrupt.o mmu.o nand.o uart.o led.o

all : $(objs)
	arm-linux-ld -r -o dev.o $^
	
%.o : %.c
	arm-linux-gcc -g ${CFLAGS} -c $^
	
%.o : %.S
	arm-linux-gcc -g ${CFLAGS} -c $^

clean:
	rm -f *.o *.bak		
	
