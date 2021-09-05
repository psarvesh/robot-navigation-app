# Text Based Robot Navigation
Producer consumer application for robot navigation using text commands

This README explains the code structure, how to build, how to run, and design decisions and assumptions.

## Directory structure

```
.
├── app
│   ├── include
│   └── src
├── bin
│   └── robot
├── LICENSE
├── Makefile
├── Question.txt
└── README.md
```

- app/ - Contains the application code
  - app/include/ - Header files
  - app/src - Source files
- bin/ - Contains the executable
- LICENSE - MIT License file
- Makefile - Used for building
- Question.txt - Problem statement copied as-is
- README.md - This file

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

```bash
$ ./bin/robot <max size> <time>
```
- max size - Max size of the buffer in characters
- time - Time interval in seconds at which consumer wakes up to read

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






