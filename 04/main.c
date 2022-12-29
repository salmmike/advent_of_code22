#include <stdio.h>
#include <stdlib.h>


size_t getDashIndex(char* str)
{
    size_t i = 0;
    while (str[i] != '-') {
        ++i;
    }
    return i;
}

size_t getCommaIndex(char* str)
{
    size_t i = 0;
    while (str[i] != ',') {
        ++i;
    }
    return i;
}

void parseNumbers(int* a, int* b, int* c, int* d, char* buff)
{
    size_t i = getDashIndex(buff);
    size_t j = getCommaIndex(buff);
    size_t k = getDashIndex(&buff[j+1]) + j + 1;
    buff[i] = '\0';
    buff[k] = '\0';

    *a = atoi(buff);
    *b = atoi(&buff[i+1]);

    *c = atoi(&buff[j+1]);
    *d = atoi(&buff[k+1]);
}


int checkContains(int a, int b, int c, int d)
{
    if (a <= c) {
        return (b >= d || a == c);
    }
    return (d >= b);
}

int checkOverlap(int a, int b, int c, int d)
{
    return ((a <= c && b >= c) || (a <= d && b >=d) || (c <= a && d >= a) || (c <= b && d >= b));
}

int main()
{
    //const char* filename = "testcase.txt";
    const char* filename = "data1.txt";
    FILE* fp = fopen(filename, "r");
    char buff[255];
    int startF = 0;
    int endF = 0;
    int startS = 0;
    int endS = 0;
    int count = 0;
    int overlap = 0;

    while (fgets(buff, 255, fp) != NULL) {
        if (buff[0] == '\n') continue;
        parseNumbers(&startF, &endF, &startS, &endS, buff);
        printf("%d-%d,%d-%d\n", startF, endF, startS, endS);
        if (checkContains(startF, endF, startS, endS)) {
            printf("Is fully contained!\n");
            count++;
        }
        if (checkOverlap(startF, endF, startS, endS)) {
            printf("Overlaps\n");
            overlap++;
        }

    }
    printf("Count: %d\nOverlap: %d\n", count, overlap);
    return 0;
}
