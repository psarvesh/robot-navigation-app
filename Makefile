all: build

build:
	mkdir -p bin
	gcc -I./app/include -pthread -o bin/robot app/src/main.c app/src/ring_buffer.c

debug:
	mkdir -p bin
	gcc -g -I./app/include -pthread -o bin/robot app/src/main.c app/src/ring_buffer.c

clean:
	rm -rf bin