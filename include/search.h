#ifndef SEARCH_H
#define SEARCH_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
bool isDirectory(const char *path);
struct line *searchPatternInFile(char *file);
void addFileNode(char *fileName, struct file *headFile, struct file *tailFile);
void addPathNode(char *pathName, struct path *headPath, struct path *tailPath);
bool searchDirectory(const char *sDir, int flags, struct file *headFile, struct file *tailFile);
void freeLinesList(struct line **headLine);
void freeFilesList(struct file **headFile);
void freePathsList(struct path **headPath);
void beginSearch(struct path *headPath, int flags);

#endif
