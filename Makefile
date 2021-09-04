all: build

build:
	mkdir -p bin
	gcc -Wall -Werror -I./app/include -pthread -o bin/robot app/src/main.c app/src/ring_buffer.c

debug:
	mkdir -p bin
	gcc -Wall -Werror -g -I./app/include -pthread -o bin/robot app/src/main.c app/src/ring_buffer.c

clean:
	rm -rf bin