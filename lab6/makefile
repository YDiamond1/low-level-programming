SOURCES = main.c image_utils.c image_utils.h
OBJS = ${SOURCES:.c=.o}
EXEC = lab6

GCC = gcc
GCC_FLAGS = -o

all:$(EXEC)

$(EXEC) : $(OBJS)
	$(GCC) $(OBJS) -lm -o $@

%.o : %.c
	$(GCC) $< -c -o $@

main.c: image_utils.h
clean :
	rm -rf $(OBJS)