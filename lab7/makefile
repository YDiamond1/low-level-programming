SOURCES = main.c mem.c
OBJS = ${SOURCES:.c=.o}
EXEC = lab7

GCC = gcc
GCC_FLAGS = -o

all:$(EXEC)

$(EXEC) : $(OBJS)
	$(GCC) -pedantic -Wall -Werror  $(OBJS) -o  $@

%.o : %.c
	$(GCC) $< -c -o $@

main.c: mem.h
clean :
	rm -rf $(OBJS)
