#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int countPriority(char c)
{
    if (c >= 'a') {
        return c - 'a' + 1;
    }
    return c - 'A' + 27;
}

void challengeOne(FILE* fp)
{
    char content[255];
    int total = 0;
    assert(countPriority('A') == 27);
    assert(countPriority('a') == 1);
    assert(countPriority('e') == 5);
    assert(countPriority('E') == 31);

    while(fgets(content, 255, fp) != NULL) {
        if (content[0] == '\n') continue;

        size_t len = strlen(content) - 1;
        char beginning[len/2 + 1];
        beginning[len/2] = '\0';
        char end[len/2 + 1];
        end[len/2] = '\0';
        memcpy(beginning, content, len/2);
        memcpy(end, content + len/2, len/2);

        int sum = 0;
        for (size_t i = 0; i < len/2; ++i) {
            for (size_t j = 0; j < len/2; ++j) {
                if (beginning[i] == end[j]) {
                    sum += countPriority(beginning[i]);
                    break;
                }
            }
            if (sum) break;
        }
        total += sum;

    } 

    printf("Total: %d\n", total);
 
}

char findBadge(char* group1, char* group2, char* group3)
{
    size_t len1 = strlen(group1);
    size_t len2 = strlen(group2);
    size_t len3 = strlen(group3);

    for (size_t i = 0; i < len1; ++i) {
        for (size_t j = 0; j < len2; ++j) {
            if (group1[i] == group2[j]) {
                for (size_t k = 0; k < len3; ++k) {
                    if (group3[k] == group2[j]) {
                        return group3[k];
                    }
                }
            }
        }
    }
    return 0;
}

void challengeTwo(FILE* fp)
{
    char group1[255];
    char group2[255];
    char group3[255];
    int sum = 0;

    while(1) {
        if (fgets(group1, 255, fp) == NULL) break;
        if (fgets(group2, 255, fp) == NULL) break;
        if (fgets(group3, 255, fp) == NULL) break;
        char c = findBadge(group1, group2, group3);
        sum += countPriority(c);
    }
    printf("Total: %d\n", sum);
}

int main()
{
    //const char* testfile = "testcase.txt";
    const char* testfile = "testdata.txt";

    FILE* fp = fopen(testfile, "r");
    challengeOne(fp);
    fclose(fp);

    fp = fopen(testfile, "r");
    challengeTwo(fp);
    fclose(fp);

    return 0;
}
