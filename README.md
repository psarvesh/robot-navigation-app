# Text Based Robot Navigation
Producer consumer application for robot navigation using text commands

This README explains the code structure, build/test setup, how to build, how to run, and design decisions and assumptions.

## Directory structure

```
.
├── app
│   ├── include
│   ├── src
│   └── tests
├── bin
│   └── robot
├── LICENSE
├── Makefile
└── README.md
```

- app/ - Contains the application code
  - app/include/ - Header files
  - app/src - Source files
  - app/tests - Test files
- bin/ - Contains the executable
- LICENSE - MIT License file
- Makefile - Used for building
- README.md - This file

## Build and Test System

The project uses gcc, make, git, Linux.

- Programming Language - C11
  - Optional feature used  - `atomic_int` keyword
  - C Compiler - gcc 9.3.0
  - ldd GLIBC version - 2.31
- make version - GNU Make 4.2.1
- git version - 2.25.1
- Linux version - 5.4.0-81-generic
  - Release - Ubuntu 20.04.3 LTS

## Build Instructions

### Build basic executable
```bash
$ make 
```
### Build debug executable
```bash
$ make debug
```
### Clean the builds
```bash
$ make clean
```

## Run Instructions

### Run application

```bash
$ ./bin/robot <max size> <time>
```
- max size - Max size of the buffer in characters
- time - Time interval in seconds at which consumer wakes up to read

### Run tests

```bash
$ make tests
```

This will run the tests and then clean the test executable if all successful.

## Design Decisions and Assumptions


### Design Decisions

1. **Threads** - Decided to use main thread as the consumer and create a new pthread for producer.
2. **Shared Buffer** - Data structure used was a lock-free ring buffer. since we are targeting a single producer-single consumer system. 
3. **Minimal Resource Sharing** -
   1. Producer uses(updates) head pointer of the ringbuffer and reads from stdin.
   2. Consumer uses(updates) tail pointer of the ringbuffer and writes to stdout.
4. **Parsing Command** - Kept separate in a function in case the command structure changes in the future.

### Assumptions

1. **Non-canonical input** - Since the input is not delimited by whitespace characters, input is read byte-by-byte. 
2. **Ignored whitespaces** - All whitespaces are ignored. So, `+++MS40;` is same as `++ +MS4<\n>0;` (Here <\n> represents the newline character ascii value 0x0A).
3. **Lock-less buffer** - The buffer didn't need any locks since it is a single writer/single reader system and the buffer capacity is 1 more than the max size (passed in through command line).

### Notes

1. **Unit Tests** - Functionality for command parsing and single threaded ring buffer is tested through the tests.
2. **Functional Test** - Remaining functionality was tested by running the code multiple times for multiple scenarios.
3. **Memcheck** - Valgrind was used to check for leaks.


