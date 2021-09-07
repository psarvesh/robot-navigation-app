all: build

build:
	mkdir -p bin
	gcc -std=c11 -Wall -Wextra -Werror -I./app/include -pthread -o bin/robot app/src/main.c app/src/ring_buffer.c

debug:
	mkdir -p bin
	gcc -std=c11 -Wall -Wextra -Werror -g -I./app/include -pthread -o bin/robot app/src/main.c app/src/ring_buffer.c

clean:
	rm -rf bin