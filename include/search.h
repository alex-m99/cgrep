#ifndef SEARCH_H
#define SEARCH_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// Macros for flags
#define W 0x01
#define I 0x02
#define N 0x04

// Struct definitions
struct path {
    char *name;
    struct path *next;
};

struct file {
    char *name;
    struct line *headLine;
    int lineCount;
    struct file *next;
};

struct line {
    int lineNr;
    char *lineText;
    struct line *next;
};

// Function declarations
void addPathNode(char *pathName, struct path **headPath, struct path **tailPath);
void addFileNode(char *fileName, struct file **headFile, struct file **tailFile);
void freePathsList(struct path **headPath);
void freeLinesList(struct line **headLine);
void freeFilesList(struct file **headFile);

bool isDirectory(const char *path);
bool searchDirectory(const char *sPattern, const int flags, const char *sDir, struct file **headFile, struct file **tailFile);
char *readLine(FILE *file);
// replace with struct line *searchPatternInFile
void searchPatternInFile(const char *sPattern, const char *filePath);
void beginSearch(const char *searchPattern, const int flags, struct path *headPath);

#endif
