#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define WIDTH 20
#define HEIGHT 10
#define ROPESIZE 10

size_t visited = 1;

struct Point {
    size_t x;
    size_t y;
};

void distance(struct Point head, struct Point tail, int* x, int* y)
{
    *x = head.x - tail.x;
    *y = head.y - tail.y;
}

char** createGrid(size_t width, size_t height)
{
    char** grid = malloc(height*sizeof(char*));
    for (size_t i = 0; i < height; ++i) {
        grid[i] = malloc(width*sizeof(char));
        for (size_t k = 0; k < width; ++k) {
            grid[i][k] = '.';
        }
    }
    return grid;
}

void printPoint(struct Point* point)
{
    printf("x: %ld, y: %ld\n", point->x, point->y);
}

void updateTail(struct Point* head, struct Point* tail)
{
    int x = 0;
    int y = 0;
    distance(*head, *tail, &x, &y);
    //printf("Head: x: %ld, y: %ld\n", head->x, head->y);
    if (abs(x) >= 2) {
        tail->x += x/abs(x);
        if (head->y != tail->y) {
            tail->y += (head->y > tail->y) ? 1 : -1;
        }
    } else if (abs(y) >= 2) {
        tail->y += y/abs(y);
        if (head->x != tail->x) {
            tail->x += (head->x > tail->x) ? 1 : -1;
        }

    }
}

void printGrid(char** grid, size_t width, size_t height, struct Point* head, struct Point* tail)
{
    printf("\n======\n");
    for (size_t i = 0; i < height; ++i) {
        for (size_t k = 0; k < width; ++k) {
            if (head->x == k && head->y == i) {
                printf("H");
            } else if (tail->x == k && tail->y == i) {
                printf("T");
            } else {
                printf("%c", grid[i][k]);
            }
        }
        printf("\n");
    }
    printf("======\n");
}

void printRope(char** grid, size_t width, size_t height, struct Point** rope)
{
    for (size_t i = 0; i < height; ++i) {
        for (size_t k = 0; k < width; ++k) {
            int printed = 0;
            if (grid[i][k] == 's') {
                printf("s");
                continue;
            }
            for (size_t j = 0; j < ROPESIZE; ++j) {
                if (rope[j]->x == k && rope[j]->y == i) {
                    if (j == 0) {
                        printf("%c", 'H');
                    } else {
                        printf("%ld", j);
                    }
                    printed = 1;
                    break;
                }
            }
            if (!printed) {
                printf("%c", grid[i][k]);
            }
        }
        printf("\n");
    }
    printf("======\n");
}

void setStepped(char** grid, size_t x, size_t y)
{
    if (grid[y][x] == '.') {
        visited++;
    }
    if (grid[y][x] != 's') {
        grid[y][x] = '#';
    }
}

void update(struct Point* head, struct Point* tail, char dir, int n, char** grid)
{
    for (size_t i = 0; i < n; ++i) {
        switch (dir)
        {
            case 'L':
                head->x-=1;
                break;
            case 'R':
                head->x+=1;
                break;
            case 'U':
                head->y-=1;
                break;
            case 'D':
                head->y+=1;
                break;
            default:
                break;
        }
        updateTail(head, tail);
        assert(head->x < WIDTH && head->y < HEIGHT && tail->x < WIDTH && tail->y < HEIGHT);
        setStepped(grid, tail->x, tail->y);
        printGrid(grid, WIDTH, HEIGHT, head, tail);
    }
}

void updateLong(struct Point** nodes, char dir, int n, char** grid)
{
    struct Point* head = nodes[0];
    struct Point* tail = nodes[ROPESIZE-1];
    for (size_t i = 0; i < n; ++i) {
        switch (dir)
        {
            case 'L':
                head->x-=1;
                break;
            case 'R':
                head->x+=1;
                break;
            case 'U':
                head->y-=1;
                break;
            case 'D':
                head->y+=1;
                break;
            default:
                return;
        }
        for (size_t i = 0; i < ROPESIZE-1; ++i) {
            updateTail(nodes[i], nodes[i+1]);
        }

        setStepped(grid, tail->x, tail->y);
        printRope(grid, WIDTH, HEIGHT, nodes);
    }

}

void parseLine(char* buff, struct Point* head, struct Point* tail, char** grid)
{
   update(head, tail, buff[0], atoi(&buff[2]), grid); 
}

void parseRope(char* buff, struct Point** rope, char** grid)
{
    updateLong(rope, buff[0], atoi(&buff[2]), grid);
}

int main()
{
    const char* filename = "testcase1.txt";
    //const char* filename = "data.txt";
    FILE* fp = fopen(filename, "r");
    char** grid = createGrid(WIDTH, HEIGHT);
    struct Point** rope = malloc(sizeof(struct Point*)*ROPESIZE);
    for (size_t i = 0; i < ROPESIZE; ++i) {
        rope[i] = malloc(sizeof(struct Point));
        rope[i]->x=WIDTH/2;
        rope[i]->y=HEIGHT/2;

    }

    //printGrid(grid, WIDTH, HEIGHT, &head, &tail);
    grid[rope[0]->y][rope[0]->x] = 's';
    char buff[255];
    while(fgets(buff, 255, fp) != NULL) {
        parseRope(buff, rope, grid);
    }
    printf("Visited: %ld\n", visited);
    return 0;
}
