CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g
TARGET  = gerenciador
SRCS    = main.c fila.c pilha.c persistencia.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) tarefas.dat

.PHONY: all clean
