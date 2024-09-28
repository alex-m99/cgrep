#include "../include/search.h"

void addPathNode(const char *pathName, struct path **headPath, struct path **tailPath) {
    struct path *newPath = (struct path *) malloc(sizeof(struct path));
        if (newPath == NULL) {
            printf("Failed to allocate memory");
            exit(1);
        }

        newPath->name = (char *) malloc(strlen(pathName)+1);
        if (newPath->name == NULL) {
            printf("Failed to allocate memory");
            free(newPath);
            return exit(1);
        }

        if (*tailPath != NULL)
            (*tailPath)->next = newPath;

        strcpy(newPath->name, pathName);
        newPath->next = NULL;
        *tailPath = newPath;

        if (*headPath == NULL)
            *headPath = newPath;   
}

void addLineNode(const char *lineText, struct line **headLine, struct line **tailLine) {
    struct line *newLine = (struct line *) malloc(sizeof(struct line));
        if (newLine == NULL) {
            printf("Failed to allocate memory");
            exit(1);
        }

        newLine->lineText = (char *) malloc(strlen(lineText)+1);
        if (newLine->lineText == NULL) {
            printf("Failed to allocate memory");
            free(newLine);
            return exit(1);
        }

        if (*tailLine != NULL)
            (*tailLine)->next = newLine;

        strcpy(newLine->lineText, lineText);
        newLine->next = NULL;
        *tailLine = newLine;

        if (*headLine == NULL)
            *headLine = newLine;  
}

void addFileNode(const char *fileName, struct file **headFile, struct file **tailFile) {
    struct file *newFile = (struct file *) malloc(sizeof(struct file));
        if (newFile == NULL) {
            printf("Failed to allocate memory");
            exit(1);
        }

        newFile->name = (char *) malloc(strlen(fileName)+1);
        if (newFile->name == NULL) {
            printf("Failed to allocate memory");
            free(newFile);
            exit(1);
        }

        if (*tailFile != NULL)
            (*tailFile)->next = newFile;

        strcpy(newFile->name, fileName);
        newFile->next = NULL;
        *tailFile = newFile;

        if (*headFile == NULL)
            *headFile = newFile;  
}

void  freePathsList(struct path **headPath) {
    struct path *curPath = *headPath;
    while (curPath != NULL) {
        struct path *nextPath = curPath->next;
        free(curPath->name);
        free(curPath);
        curPath = nextPath;
    }
    *headPath = NULL;
}

void freeLinesList(struct line **headLine) {
    struct line *curLine = *headLine;
    while (curLine != NULL) {
        struct line *nextLine = curLine->next;
        free(curLine->lineText);
        free(curLine);
        curLine = nextLine;
    }
    *headLine = NULL;
}

void freeFilesList(struct file **headFile) {
    struct file *curFile = *headFile;
    while (curFile != NULL) {
        struct file *nextFile = curFile->next;
        free(curFile->name);
         // free the lines list linked to the file as well
        freeLinesList(&curFile->headLine);
        free(curFile);
        curFile = nextFile;
    }
    *headFile = NULL;
}

