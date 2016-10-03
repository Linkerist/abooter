
ARCH	=	arm

CROSS_COMPILE	= arm-linux-
AS	=	$(CROSS_COMPILE)as
LD	=	$(CROSS_COMPILE)ld
CC	=	$(CROSS_COMPILE)gcc
CPP	=	$(CC) -E
AR	=	$(CROSS_COMPILE)ar
NM	=	$(CROSS_COMPILE)nm

STRIP	=	$(CROSS_COMPILE)strip
OBJCOPY	=	$(CROSS_COMPILE)objcopy
OBJDUMP	=	$(CROSS_COMPILE)objdump
export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS	:=	-Wall -O2 -g -fno-builtin
CFLAGS	+=	-I $(shell pwd)/include -I $(SRCTREE)/arch/$(ARCH)/include

SRCTREE := /home/win/Proj/Vay_BootLoader
export SRCTREE

LDS	:=	$(SRCTREE)/mainboard/bonzi/2440/Vay_BootLoader.lds
LDFLAGS	:=	-T $(LDS)
export CFLAGS LDFLAGS

TOPDIR	:=	$(shell pwd)
export TOPDIR

TARGET := Vay_BootLoader

obj-y += main.o
obj-y += arch/
obj-y += protocols/
obj-y += drivers/
obj-y += mainboard/
#obj-y += fs/
obj-y += lib/
#obj-y += tools/
#obj-y += post/

$(TARGET)	:
	make -C ./ -f $(TOPDIR)/scripts/Makefile.build
	$(LD) $(LDFLAGS) -o $(TARGET) built-in.o


$(TARGET).bin : $(TARGET)
	$(OBJCOPY) -O binary $^ $@

#$(TARGET).elf : Vay_BootLoader
#	$(LD) $(LDFLAGS) -o $@ $^


.PHONY	:	clean
clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	make clean
	rm -f $(shell find -name "*.o.d")
	rm -f ./.config

