C++ = g++
CFLAGS = -c -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

all: build

build:	main.o desktop_loading.o Desktop.o search.o
	$(C++) -o search-open $^ $(LDFLAGS)

clean:
	rm -f *.o
	rm -f search-open

%.o: %.cpp
	$(C++) $(CFLAGS) $*.cpp

run: clean build
	./search-open