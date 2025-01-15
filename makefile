CFLAGS = -Wall -g
LDFLAGS = -lm

CC = gcc

objects = beale.o testa_entrada.o gera_chaves.o codifica.o decodifica.o
     
all: beale

beale: $(objects)
	$(CC) $^ -o $@ $(LDFLAGS)

beale.o: beale.c testa_entrada.h gera_chaves.h codifica.h decodifica.h
	$(CC) -c $< $(CFLAGS)

testa_entrada.o: testa_entrada.c
	$(CC) -c $< $(CFLAGS)

gera_chaves.o: gera_chaves.c
	$(CC) -c $< $(CFLAGS)

codifica.o: codifica.c gera_chaves.h
	$(CC) -c $< $(CFLAGS)

decodifica.o: decodifica.c gera_chaves.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(objects)

purge:
	rm -f $(objects) beale