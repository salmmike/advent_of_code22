#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define STACKSIZE 9

int hasString(char* buff, char* str)
{
    return strstr(buff, str) != NULL;
}

char* findLastIndex(char* buff, char* str)
{
    return strstr(buff, str) + strlen(str);
}

void nullNextNonDigit(char* str)
{
    while(*str >= '0' && *str <= '9') {
        ++str;
    }
    *str = '\0';
}

void parseMoves(char* buff, int* a, int* b, int* c)
{
    char* moved = findLastIndex(buff, "move ");
    char* from = findLastIndex(buff, "from ");
    char* to = findLastIndex(buff, "to ");
    nullNextNonDigit(moved);
    nullNextNonDigit(to);
    nullNextNonDigit(from);
    
    *a = atoi(moved);
    *b = atoi(from);
    *c = atoi(to);
}

size_t getStackSize(char** stacks, size_t i)
{
    size_t size = (size_t) stacks[i][0];
    assert((long)size >= 0);
    return size;
}

char pop(char** stacks, size_t i)
{
    size_t size = getStackSize(stacks, i);
    char c = stacks[i][size];
    stacks[i][size] = 0;
    assert(stacks[i][0] > 0);
    stacks[i][0]--;
    return c;
}

void push(char** stacks, size_t i, char c)
{
    if (c == 0) {
        return;
    }
    size_t top = getStackSize(stacks, i);
    stacks[i][top+1] = c;
    stacks[i][0]++;
    //printf("Stack %ld size: %d\n", i, top+1);
    assert(stacks[i][0] > 0);
}

void parseStackStr(char** stacks, char* line)
{
    for (size_t i = 0; i < STACKSIZE; ++i) {
        char c = line[i*4+1];
        if (c != ' ' && c != 0) {
            push(stacks, i, c);
        }
    }
}

char** createStack(size_t n, size_t m)
{
    char** stack = (char**) calloc(n + 1, sizeof(char*));
    for (size_t i = 0; i <= n; ++i) {
        stack[i] = calloc(m, sizeof(char));
    }
    return stack;
}

int getMaxStack(char** stacks)
{
    int max = 0;
    for (size_t i = 0; i < STACKSIZE; ++i) {
        max = stacks[i][0] > max ? stacks[i][0] : max;
    }
    return max;
}

bool printbox(char c)
{
    if (c == 0) {
        printf("   ");
        return false;
    } else {
        printf("[%c]", c);
    }
    return true;
}

void printStack(char** stacks)
{
    printf("-----------------------------\n");
    int max = getMaxStack(stacks);
    int boxcount = 0;
    for (size_t i = max; i > 0; --i) {
        for (size_t k = 0; k < STACKSIZE; ++k) {
            if (printbox(stacks[k][i])) {
                boxcount++;
            }
        }
        printf("\n");
    }
    printf("-1--2--3--4--5--6--7--8--9---\n");
    printf("------| box count : %d |-----\n", boxcount);
}

int main()
{
    const char* filename = "data2.txt";

    char** stacks = createStack(STACKSIZE, 70);
    char lines[512][STACKSIZE*10];

    FILE* fp = fopen(filename, "r");
    char buff[255];
    size_t i = 0;

    while(hasString(fgets(buff, 255, fp), "[")) {
        strcpy(lines[i], buff);
        ++i;
    }

    for (int k = i; k >= 0; --k) {
        parseStackStr(stacks, lines[k]);
    }
    printStack(stacks);
    int tester = 0;

    while(fgets(buff, 255, fp) != NULL) {
        tester++;
        printf("%d:\n", tester);
        if (buff[0] == '\n') continue;
        if (buff[0] != 'm') continue;
        int a, b, c;
        printf("%s", buff);
        parseMoves(buff, &a, &b, &c);
        size_t beginSrcSize = getStackSize(stacks, b-1);
        size_t beginDstSize = getStackSize(stacks, c-1);
        char tempChars[a];
        for (int i = 0; i < a; ++i) {
            char moved = pop(stacks, b-1);
            tempChars[i] = moved;
        }
        for (int i = a - 1; i >= 0; --i) {
            push(stacks, c-1, tempChars[i]);
        }
        size_t endSrcSize = getStackSize(stacks, b-1);
        size_t endDstSize = getStackSize(stacks, c-1);
        assert(beginSrcSize - a == endSrcSize);
        assert(beginDstSize + a == endDstSize);

        printStack(stacks);
        printf("\n");
    }
    fclose(fp);
    return 0;
}
