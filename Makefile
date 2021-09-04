all: build

build:
	mkdir bin
	gcc -I./app/include -pthread -o bin/robot src/main.c src/threads.c src/ring_buffer.c

debug:
	mkdir bin
	gcc -g -I./app/include -pthread -o bin/robot src/main.c src/threads.c src/ring_buffer.c

clean:
	rm -rf bin