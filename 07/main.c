#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

struct File {
    char* name;
    size_t size;
};

struct Dir {
    char *name;
    struct Dir* parent;

    size_t dirCount;
    struct Dir** subdirs;

    struct File** files;
    size_t fileCount;
};

#define limit 100000
#define maxSpace 70000000
size_t currentSpace = 0;
size_t underLimit = 0;
size_t minOver = LONG_MAX;
size_t needToFree = 0;

void printDir(struct Dir* dir)
{
    printf("Dir %s, dirCount: %ld, fileCount: %ld\n", dir->name, dir->dirCount, dir->fileCount);
}


size_t dirSize(struct Dir* dir)
{
    size_t size = 0;
    for (size_t i = 0; i < dir->fileCount; ++i) {
        size += dir->files[i]->size;
    }
    for (size_t i = 0; i < dir->dirCount; ++i) {
        size += dirSize(dir->subdirs[i]);
    }
    return size;
}

void printTree(struct Dir* dir, size_t level)
{

    size_t totalSize = dirSize(dir);
    if (totalSize < limit) {
        underLimit += totalSize;
    }
    if (totalSize > needToFree && totalSize < minOver) {
        minOver = totalSize;
        printf("min over: %s - size: %ld\n", dir->name, minOver);
    }
    //for (size_t j = 0; j < level; ++j)
       //printf("  ");
    //printf("- %s (dir), total size: %ld\n", dir->name, totalSize);

    for (size_t i = 0; i < dir->dirCount; ++i) {
        printTree(dir->subdirs[i], level + 1);
    }
    for (size_t i = 0; i < dir->fileCount; ++i) {
        continue;
        for (size_t j = 0; j <= level; ++j)
            printf("  ");
        //printf("- %s (file, size=%ld)\n", dir->files[i]->name, dir->files[i]->size);
    }

}


struct Dir* parseDir(FILE* fp, struct Dir* parent, char* name)
{
    struct Dir* dir = calloc(1, sizeof(struct Dir));
    dir->parent = parent;
    dir->name = malloc(strlen(name) * sizeof(char) + 1);
    strcpy(dir->name, name);
    return dir;
}

void newlineToNull(char* str)
{
    while(*str != '\n' && *str != '\0') {
        ++str;
    }
    *str = '\0';
}

struct Dir* cd(char* filename, struct Dir* currentDir)
{
    newlineToNull(filename);
    if (strstr(filename, "..") != NULL) {
        if (currentDir->parent != NULL) {
            return currentDir->parent;
        }
        printf("%s no parent dir\n", currentDir->name);
        assert(0);
    }
    if (strstr(filename, "/") != NULL) {
        while(currentDir->parent != NULL) {
            currentDir = currentDir->parent;
        }
        return currentDir;
    }
    for (size_t i = 0; i < currentDir->dirCount; ++i) {
        if (strcmp(currentDir->subdirs[i]->name, filename) == 0) {
            printf("%s to %s\n", __func__, filename);
            return currentDir->subdirs[i];
        }
    }
    printf("No %s in %s\n", filename, currentDir->name);
    assert(0);
}

void freeFile(struct File* file)
{
    free(file->name);
    free(file);
}

void freeDir(struct Dir* dir)
{
    free(dir->name);
    free(dir->subdirs);
    free(dir->files);
    free(dir);
}

void freeTree(struct Dir* dir)
{
    for (size_t i = 0; i < dir->dirCount; ++i) {
        freeTree(dir->subdirs[i]);
    }
    for (size_t i = 0; i < dir->fileCount; ++i) {
        freeFile(dir->files[i]);
    }
    freeDir(dir);
}

void addFile(char* name, size_t size, struct Dir* dir)
{
    newlineToNull(name);
    printf("%s: %s with size %ld to %s\n", __func__, name, size, dir->name);
    dir->fileCount++;
    printf("Realloc %s files to %ld\n",dir->name, dir->fileCount * sizeof(struct File*));
    dir->files = realloc(dir->files, (dir->fileCount * sizeof(struct File*)));
    struct File* file = calloc(1, sizeof(struct File));
    file->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(file->name, name);
    file->size = size;
    dir->files[dir->fileCount-1] = file;
}

void addDir(char* name, struct Dir* dir)
{
    newlineToNull(name);
    printf("%s: add dir %s to %s\n", __func__, name, dir->name);
    dir->dirCount++;
    printf("Realloc %s dirs to %ld\n",dir->name, dir->dirCount * sizeof(struct Dir*));
    dir->subdirs = realloc(dir->subdirs, (dir->dirCount * sizeof(struct Dir*)));
    printf("Realloc done\n");
    struct Dir* newDir = calloc(1, sizeof(struct Dir));
    newDir->parent = dir;
    newDir->name = calloc(strlen(name) + 1, sizeof(char));
    newDir->subdirs = malloc(sizeof(struct Dir*));
    newDir->files = malloc(sizeof(struct File*));
    newDir->dirCount = 0;
    newDir->fileCount = 0;
    strcpy(newDir->name, name);
    dir->subdirs[dir->dirCount-1] = newDir;
}



void parseLs(FILE* fp, struct Dir* dir, char* outBuff)
{
    char buff[255];
    while (fgets(buff, 255, fp) != NULL) {
        printf("%s: %s",__func__, buff);
        if (strstr(buff, "dir") != NULL) {
            addDir(&buff[4], dir);
            continue;
        }
        int size = 0;
        if ((size = atoi(buff))) {
            char* str = buff;
            while(*str != ' ' && *str != '\0') str++;
            newlineToNull(str);
            str++;
            addFile(str, size, dir);
            continue;
        }
        strcpy(outBuff, buff);
        break;
    }
}

int main()
{
    //const char* filename = "testcase.txt";
    const char* filename = "mydata";
    FILE* fp = fopen(filename, "r");
    char buff[255];
    struct Dir* currentDir = calloc(1, sizeof(struct Dir));
    currentDir->parent = NULL;
    currentDir->name = malloc(strlen("/") * sizeof(char) + 1);
    currentDir->subdirs = malloc(sizeof(struct Dir*));
    currentDir->files = malloc(sizeof(struct File*));
    strcpy(currentDir->name, "/\0");

    while (fgets(buff, 255, fp) != NULL) {
        printf("%s", buff);
        if (strstr(buff, "$ ls")) {
            parseLs(fp, currentDir, buff);
        } 
        if (strstr(buff, "$ cd") != NULL) {
            currentDir = cd(&buff[5], currentDir);
        }
    }
    buff[0] = '/';
    buff[1] = '\0';
    currentDir = cd(buff, currentDir);

    currentSpace = maxSpace - dirSize(currentDir);
    needToFree = 30000000 - currentSpace;
    printTree(currentDir, 0);
    freeTree(currentDir);
    printf("Total size under limit: %ld\n", underLimit);
    return 0;
}
