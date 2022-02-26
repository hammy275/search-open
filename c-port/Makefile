C++ = g++
CFLAGS = -c -g

all: build

build:	main.o desktop_loading.o Desktop.o search.o
	$(C++) -o search-open main.o desktop_loading.o Desktop.o search.o

clean:
	rm -f *.o
	rm -f search-open

%.o: %.c
	$(C++) $(CFLAGS) $*.c

run: clean build
	./search-open
