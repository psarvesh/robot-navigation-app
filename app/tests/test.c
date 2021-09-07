#include <stdio.h>
#include <assert.h>

#include "command.h"
#include "ring_buffer.h"


void TestCommandValidationAndParsing() {
    char NSEW_chars[] = "NSEW";
    char cmd[] = "+++MN00;";
    CommandType type, expectedType;
    int n;
    
    // Check all possible valid values for directions
    printf("Checking all possible values for directions - ");
    for(int dir = 0; dir < 4; dir++) {
        expectedType = kNorth + dir;
        cmd[4] = NSEW_chars[dir];
        for(int i = 0; i < 100; i++) {
            cmd[5] = (i/10) + '0';
            cmd[6] = (i % 10) + '0';
            ParseCommand(cmd, &type, &n);
            assert(("Failed in asserting type" && type == expectedType));
            assert(("Failed in asserting number" && n == i));
        }
    }

    printf(" - PASSED\n Checking EXIT - ");
    // Check exit
    char EXIT_cmd[] = "+++EXIT;";
    ParseCommand(EXIT_cmd, &type, &n);
    assert(("EX Failed in asserting type" && type == kExit));

    printf(" - PASSED\n Checking Invalid - ");
    // Check invalid
    cmd[0] = '\x00';
    cmd[1] = '\x7F';
    cmd[2] = '\x08';
    cmd[3] = '\x40';
    ParseCommand(cmd, &type, &n);
    assert(("INV 0 Failed in asserting type" && type == kInvalid));
    
    ParseCommand("MN40;124", &type, &n);
    assert(("INV 1 Failed in asserting type" && type == kInvalid));

    ParseCommand("abcd+++M", &type, &n);
    assert(("INV 2 Failed in asserting type" && type == kInvalid));

    ParseCommand("abcdefgh", &type, &n);
    assert(("INV 3 Failed in asserting type" && type == kInvalid));

    printf(" - PASSED\n Checking string size restrictions - ");
    ParseCommand("123456789", &type, &n);
    assert(("String size assetion failed" && type == kInvalid));
    ParseCommand("", &type, &n);
    assert(("String size assetion failed" && type == kInvalid));

    printf(" - PASSED\n");
}


void TestRingBuffer() {
    RingBuffer rb;
    const int test_size = 4;
    int x, y;
    printf("Checking Initialiazation - ");
    assert(("InitBuffer fails" && InitBuffer(&rb, test_size) == 0));
    assert(("Buffer is null" && rb.buffer != NULL));
    assert(("Capacity is incorrect" && rb.capacity == test_size + 1));
    x = rb.head;
    assert("head pointer is incorrect" && x == 0);
    x = rb.tail;
    assert("tail pointer is incorrect" && x == 0);

    printf("- PASSED\n Checking Filling and emptying buffer - ");
    const char values[] = {'T', 'e', 's', 't', 's'};
    x = rb.head;
    y = rb.tail;
    assert("head pointer is incorrect" && x == 0);
    for(int i = 0; i < test_size; i++) {
        assert(("Push fails" && Push(&rb, values[i]) == 0));
        x++;
        assert(("Head pointer is not incremented" && rb.head == x));
        assert(("Tail pointer is changed" && rb.tail == y));
    }
    assert(("Push succeeded beyond buffer limit" && Push(&rb, values[4]) == -1));
    char ch;
    for(int i = 0; i < test_size; i++) {
        assert(("Pop fails" && Pop(&rb, &ch) == 0));
        assert("Popped value is incorrect" && ch == values[i]);
        y++;
        assert(("Tail pointer is not incremented" && rb.tail == y));
        assert(("Head pointer is changed" && rb.head == x));
    }
    assert(("Pop succeeded even if buffer empty" && Pop(&rb, &ch) == -1));

    printf("- PASSED\n Checking fill and remove elements - ");
    x = rb.head;
    y = rb.tail;

    assert(("Push fails" && Push(&rb, values[0]) == 0));
    x = (x + 1) % (test_size + 1);
    assert(("Head pointer is not overflown" && rb.head == x));
    assert(("Tail pointer is changed" && rb.tail == y));
    assert(("Push fails" && Push(&rb, values[1]) == 0));
    x = (x + 1) % (test_size + 1);
    assert(("Head pointer is not incremented" && rb.head == x));
    assert(("Tail pointer is changed" && rb.tail == y));
    assert(("Push fails" && Push(&rb, values[2]) == 0));
    x = (x + 1) % (test_size + 1);
    assert(("Head pointer is not incremented" && rb.head == x));
    assert(("Tail pointer is changed" && rb.tail == y));
    assert(("Pop fails" && Pop(&rb, &ch) == 0));
    assert("Popped value is incorrect" && ch == values[0]);
    y = (y + 1) % (test_size + 1);
    assert(("Tail pointer is not incremented" && rb.tail == y));
    assert(("Head pointer is changed" && rb.head == x));
    assert(("Pop fails" && Pop(&rb, &ch) == 0));
    assert("Popped value is incorrect" && ch == values[1]);
    y = (y + 1) % (test_size + 1);
    assert(("Tail pointer is not incremented" && rb.tail == y));
    assert(("Head pointer is changed" && rb.head == x));
    assert(("Push fails" && Push(&rb, values[3]) == 0));
    x = (x + 1) % (test_size + 1);
    assert(("Head pointer is not incremented" && rb.head == x));
    assert(("Tail pointer is changed" && rb.tail == y));
    assert(("Pop fails" && Pop(&rb, &ch) == 0));
    assert("Popped value is incorrect" && ch == values[2]);
    y = (y + 1) % (test_size + 1);
    assert(("Tail pointer is not incremented" && rb.tail == y));

    printf("- PASSED\n Checking Resetting buffer - ");
    ResetBuffer(&rb);
    assert(("Buffer is null" && rb.buffer != NULL));
    assert(("Capacity changed" && rb.capacity == test_size + 1));
    x = rb.head;
    assert("head pointer is not reset" && x == 0);
    x = rb.tail;
    assert("tail pointer is not reset" && x == 0);

    printf("- PASSED\n Checking Freeing buffer - ");
    rb.head = 1;
    rb.tail = 3;
    FreeBuffer(&rb);
    assert(("Buffer is not null" && rb.buffer == NULL));
    assert(("Capacity changed" && rb.capacity == test_size + 1));
    x = rb.head;
    assert("head pointer changed" && x == 1);
    x = rb.tail;
    assert("tail pointer changed" && x == 3);
    printf(" - PASSED\n");
}

int main(void) {
    printf("\nRunning tests \n");
    printf("\n==================================================================\n");
    printf("Test command validation and parsing\n");
    printf("\n==================================================================\n");
    TestCommandValidationAndParsing();
    printf("\n==================================================================\n");
    printf("Testing single threaded ringbuffer functionality\n");
    printf("\n==================================================================\n");
    TestRingBuffer();
    printf("\nTests completed successfully.\n");
    return 0;
}