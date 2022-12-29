#include <stdio.h>
#include <stdlib.h>
#include <string.h>


size_t getMarker(char* str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len - 14; ++i) {
        char buf[255] = {0};
        int found = 1;
        for (size_t k = 0; k < 14; ++k) {
            buf[str[i+k]]++;
            if (buf[str[i+k]] > 1) {
                found = 0;
                break;
            }
        }
        if (found) {
            return i + 14;
        }
    }
    return 0;
}

int main()
{
    FILE* fp = fopen("testcase.txt", "r");
    char buff[5012];
    while(fgets(buff, 5012, fp) != NULL) {
        size_t res = getMarker(buff);
        printf("Marker: %ld\n", res);
    }
    return 0;
}
