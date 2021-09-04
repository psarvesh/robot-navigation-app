all: build

build:
	mkdir bin
	gcc -I./app/include -pthread -o bin/robot src/main.c src/threads.c src/ring_buffer.c

debug:
	gcc -g -I./app/include -pthread -o bin/robot src/main.c src/threads.c src/ring_buffer.c

clean:
	rm -rf bin