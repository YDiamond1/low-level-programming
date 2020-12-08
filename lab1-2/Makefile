
ASM = nasm
LD  = ld
ASMFLAGS = -g -felf64
SOURCES = main.asm dict.asm lib.asm
OBJS = ${SOURCES:.asm=.o}
EXEC = main



all:$(EXEC)

$(EXEC) : $(OBJS)
	$(LD) $(OBJS) -o $@

%.o : %.asm 
	$(ASM) $(ASMFLAGS) $^ -o $@

clean : 
	rm -rf $(OBJS) $(EXEC)
