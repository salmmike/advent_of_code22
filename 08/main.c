#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

void printGrid(uint8_t** grid, size_t width, size_t height)
{
    for (size_t i = 0; i < height; ++i) {
        for (size_t k = 0; k < width; ++k) {
            printf("%d", grid[i][k]);
        }
        printf("\n");
    }
}



uint8_t** parseGrid(FILE* fp, size_t* width, size_t* height)
{
    char *buff = malloc(255*sizeof(char));
    size_t maxSize = 255;
    uint8_t** trees = malloc(maxSize * sizeof(uint8_t*));

    size_t i = 0;

    while (fgets(buff, 255, fp) != NULL) {
        size_t k = 0;
        if (strlen(buff) > 2)
            *width = strlen(buff) - 1;
        trees[i] = malloc(*width * sizeof(uint8_t));

        while(buff[k] != '\n' &&  buff[k] != '\0') {
            trees[i][k] = (uint8_t) buff[k] - '0';
            ++k;
        }
        ++i;
    }
    *height = i - 1;
    return trees;
}

bool visibleLeft(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    for (size_t i = 0; i < x; ++i) {
        if (grid[y][i] >= grid[y][x]) {
            return false;
        }
    }
    return true;
}

bool visibleRight(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    for (size_t i = width - 1; i > x; --i) {
        if (grid[y][i] >= grid[y][x]) {
            return false;
        }
    }
    return true;
}

bool visibleBottom(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    for (size_t i = height - 1; i > y; --i) {
        if (grid[i][x] >= grid[y][x]) {
            return false;
        }
    }
    return true;
}

bool visibleTop(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    for (size_t i = 0; i < y; ++i) {
        if (grid[i][x] >= grid[y][x]) {
            return false;
        }
    }
    return true;
}

bool checkVisible(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    return visibleRight(grid, width, height, x, y) ||
           visibleLeft(grid, width, height, x, y) ||
           visibleBottom(grid, width, height, x, y) ||
           visibleTop(grid, width, height, x, y);
}

void printVisible(uint8_t** grid, size_t width, size_t height)
{
    for (size_t i = 0; i < height; ++i) {
        for (size_t k = 0; k < width; ++k) {
            if (checkVisible(grid, width, height, k, i)) {
                printf("%d", grid[i][k]);
            } else {
                printf("x");
            }
        }
        printf("\n");
    }
}

size_t sR(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{

    size_t view = 0;
    for (size_t i = x+1; i < width; ++i) {
        ++view;
        if (grid[y][i] >= grid[y][x]) {
            break;
        }
    }
    printf("%ld, %ld right: %ld\n", x, y, view);
    return view;
}

size_t sL(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    size_t view = 0;

    for (int i = x - 1; i >= 0; --i) {
        ++view;
        if (grid[y][i] >= grid[y][x]) {
            break;
        }
    }
    printf("%ld, %ld left: %ld\n", x, y, view);
    return view;
}

size_t sB(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    size_t view = 0;

    for (size_t i = y+1; i < height; ++i) {
        ++view;
        if (grid[i][x] >= grid[y][x]) {
            break;
        }
    }
    printf("%ld, %ld bottom: %ld\n", x, y, view);
    return view;
}

size_t sT(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    size_t view = 0;

    for (int i = y-1; i >= 0; --i) {
        ++view;
        if (grid[i][x] >= grid[y][x]) {
            break;
        }
    }
    printf("%ld, %ld top: %ld\n", x, y, view);
    return view;
}

size_t checkScenery(uint8_t** grid, size_t width, size_t height, size_t x, size_t y)
{
    return sR(grid, width, height, x, y) *
           sL(grid, width, height, x, y) *
           sB(grid, width, height, x, y) *
           sT(grid, width, height, x, y);
}

size_t maxScenery(uint8_t** grid, size_t width, size_t height)
{
    size_t max = 0;
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            size_t curr = checkScenery(grid, width, height, j, i);
            if (curr > max) {
                max = curr;
            }
        }
    }
    printf("Max: %ld\n", max);
    return max;
}

int main()
{
    //const char* filename = "testcase.txt";
    const char* filename = "test.txt";
    FILE* fp = fopen(filename, "r");
    size_t width = 0;
    size_t height = 0;
    uint8_t** grid = parseGrid(fp, &width, &height);
    printf("Height %ld, Width %ld\n", height, width);
    printVisible(grid, width, height);
    size_t visible = 0;
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            if (checkVisible(grid, width, height, x, y)) {
                visible++;
            }
        }
    }
    printf("Visible: %ld\n", visible);
    maxScenery(grid, width, height);
    return 0;
}
