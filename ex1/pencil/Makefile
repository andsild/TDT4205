CFLAGS=-std=gnu99 -g
LDLIBS=-lm
pencil: pencil.c scanner.c pencil.h
test: pencil
	cat triangle.test | ./pencil | ps2pdf - > triangle.pdf
clean:
	-rm -f pencil triangle.pdf
