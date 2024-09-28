#include "../include/search.h"

bool isDirectory(const char *path) {
    DWORD attributes = GetFileAttributes(path);
    
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        // Error handling if the path is invalid
        printf("Invalid path: [%s]\n", path);
        return false;
    }
    
    // Check if it's a directory
    return (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool searchDirectory(const char *sPattern, const int flags, const char *sDir, struct file **headFile, struct file **tailFile) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];
    
    // string buffer for hFind. search for all files and directories
    sprintf(sPath, "%s\\*.*", sDir);
    //printf("sPath outside: %s\n", sPath);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path not found: [%s]\n", sDir);
        return false;
    }

    do {
        // first two directories will be . and ..
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0) {
            // build filepath with parameter path and found filename
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);
           // printf("sPath inside: %s\n", sPath);


            // if found directory, recursively call function to search for files in it
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
               //printf("Directory: %s\n", sPath);
                // recursively search for the next files in the directory
                searchDirectory(sPattern, flags, sPath, headFile, tailFile); 
            }
            //if found file, search for pattern, add the file to linked list if pattern found and link
            // the file to the line list
            else {
                searchFile(sPattern, sPath, headFile, tailFile);
                
              //  if (headLine != NULL){
                    //addFileNode(sPath, headFile, tailFile);
               //     (*tailFile)->headLine = headLine; 
             //   }
                
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); 

    // close search handle
    FindClose(hFind); 

    return true;
}

void searchFile(const char *sPattern, const char *sPath, struct file **headFile, struct file **tailFile) {
    int linePatternCount = 0;
    struct line *headLine = searchPatternInFile(sPattern, sPath, &linePatternCount);
    if (headLine != NULL) {
        addFileNode(sPath, headFile, tailFile);
        (*tailFile)->headLine = headLine;
        (*tailFile)->linePatternCount = linePatternCount;
    }
}

char *readLine(FILE *file) {
    size_t buffer_size = 128;
    size_t pos = 0;
    int c;
    char *buffer = (char *) malloc(buffer_size);
    
    if (buffer == NULL) {
        printf("Failed to allocate memory for line reading");
        return NULL;
    }

    while ((c = fgetc(file)) != EOF) {
        // Ensure space for the null terminator
        if (pos + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                printf("Failed to allocate memory for line reading");
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }

        buffer[pos++] = c;

        // Stop at newline
        if (c == '\n') {
            break;
        }
    }

    // Null-terminate
    buffer[pos] = '\0';

    if (pos == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

// replace with struct line *searchPatternInFile
struct line *searchPatternInFile(const char *sPattern, const char *filePath, int *linePatternCount) {
    struct line *headLine = NULL;
    struct line *tailLine = NULL;
    FILE *file = fopen(filePath, "r");
    char *line;
    int lineNumber = 0;

    if (file == NULL) {
        printf("Error opening file %s\n", filePath);
        exit(1);
    }

    while((line = readLine(file)) != NULL) {
        ++lineNumber;
        // if pattern is found in the line, add it to linked list
        if(strstr(line, sPattern) != NULL) {
            addLineNode(line, &headLine, &tailLine);
            tailLine->lineNumber = lineNumber;
            ++(*linePatternCount);
           // printf("Pattern found in file %s at line %d: %s", filePath, lineNumber, line);
        }
        free(line);
    }

    fclose(file);

    return headLine;
}

void printResult(struct file *headFile, const int flags) {
    struct file *curFile = headFile;
    while (curFile != NULL) {
        struct line *curLine = curFile->headLine;
        while(curLine != NULL) {
            printf("%s : %d : %s", curFile->name, curLine->lineNumber, curLine->lineText);
            curLine = curLine->next;
        }
        curFile = curFile->next;
    }

}

void beginSearch(const char *sPattern, const int flags, struct path *headPath) {
    // traverse the path linked list and do searchDirectory routine for each path. 
    // if the provided path is directory, begin searchDirectory routine
    // if the provided path is file, begin searchPatternInFile routine
    struct file *headFile = NULL;
    struct file *tailFile = NULL;

    struct path *curPath = headPath;
    while (curPath != NULL) {
        if(isDirectory(curPath->name)) 
            searchDirectory(sPattern, flags, curPath->name, &headFile, &tailFile);
        else 
            searchFile(sPattern, curPath->name, &headFile, &tailFile);
        
        curPath = curPath->next;
    }

    printResult(headFile, flags);

   // struct file *curFile = headFile;
    // while (curFile != NULL) {
    //     printf("File name: %s\n", curFile->name);
    //     curFile = curFile->next;
    // }

    // free the files linked list
    freeFilesList(&headFile);
}
