all: build

build:
	mkdir -p bin
	gcc -std=c11 -Wall -Wextra -Werror -I./app/include -pthread -o bin/robot app/src/main.c app/src/ring_buffer.c

debug:
	mkdir -p bin
	gcc -std=c11 -Wall -Wextra -Werror -g -I./app/include -pthread -o bin/robot app/src/main.c app/src/ring_buffer.c

tests:
	mkdir -p bin
	gcc -std=c11 -Wall -Wextra -Werror -g -I./app/include -pthread -o bin/tests app/tests/test.c app/src/ring_buffer.c
	./bin/tests
	rm -rf bin/tests

clean:
	rm -rf bin