include ../Makefile.inc

MODULE=0000-sampleCodeModule
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES=$(wildcard [^_]*.c)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS=$(SOURCES:.c=.o)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c _loader.c $(SOURCES) 

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@


all: $(MODULE).bin 

$(MODULE).bin: $(MODULE).elf
	$(OBJCOPY) -O binary ../$< ../$@

$(MODULE).elf: $(OBJECTS_ASM) $(OBJECTS)
	$(LD) -T sampleCodeModule.ld _loader.o $(OBJECTS) $(OBJECTS_ASM) -o ../$@

clean:
	rm -rf *.o

.PHONY: all clean print

