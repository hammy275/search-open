C++ = g++
CFLAGS = -c -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0` -l pthread

all: build

build:	main.o desktop_loading.o Desktop.o search.o ui.o
	$(C++) -g -o search-open $^ $(LDFLAGS)

clean:
	rm -f *.o
	rm -f search-open

%.o: %.cpp
	$(C++) $(CFLAGS) $*.cpp

run: clean build
	./search-open

valgrind: clean build
	valgrind --leak-check=full ./search-open