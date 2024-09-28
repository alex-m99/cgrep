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
    int linePatternCount;
    struct file *next;
};

struct line {
    char *lineText;
    int lineNumber;
    struct line *next;
};

// Function declarations
void addPathNode(const char *pathName, struct path **headPath, struct path **tailPath);
void addLineNode(const char *lineText, struct line **headLine, struct line **tailLine);
void addFileNode(const char *fileName, struct file **headFile, struct file **tailFile);
void freePathsList(struct path **headPath);
void freeLinesList(struct line **headLine);
void freeFilesList(struct file **headFile);

bool isDirectory(const char *path);
bool searchDirectory(const char *sPattern, const int flags, const char *sDir, struct file **headFile, struct file **tailFile);
void searchFile(const char *sPattern, const char *sPath, struct file **headFile, struct file **tailFile);
char *readLine(FILE *file);
struct line *searchPatternInFile(const char *sPattern, const char *filePath, int *linePatternCount);
void printResult(struct file *headFile, const int flags);
void beginSearch(const char *searchPattern, const int flags, struct path *headPath);

#endif
