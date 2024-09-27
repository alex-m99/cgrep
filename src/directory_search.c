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

bool searchDirectory(const char *sDir, int flags, struct file *headFile, struct file *tailFile) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];
    
    // string buffer for hFind. search for all files and directories
    sprintf(sPath, "%s\\*.*", sDir);
    printf("sPath outside: %s\n", sPath);

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
            printf("sPath inside: %s\n", sPath);


            // if found directory, recursively call function to search for files in it
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
               // printf("Directory: %s\n", sPath);
                // recursively search for the next files in the directory
                searchDirectory(sPath, flags, headFile, tailFile); 
            }
            //if found file, search for pattern, add the file to linked list if pattern found and link
            // the file to the line list
            else {
                struct line *headLine = searchPatternInFile(sPath);
                if (headLine != NULL){
                    addFileNode(sPath, headFile, tailFile);
                    tailFile->headLine = headLine; 
                }
                
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); 

    // close search handle
    FindClose(hFind); 

    return true;
}

struct line *searchPatternInFile(char *file) {
   // struct line *headLine = NULL;
   // struct line *tailLine = NULL;

    return NULL;
}

void beginSearch(struct path *headPath, int flags) {
    // traverse the path linked list and do searchDirectory routine for each path. 
    // if the provided path is directory, begin searchDirectory routine
    // if the provided path is file, begin searchPatternInFile routine
    struct file *headFile = NULL;
    struct file *tailFile = NULL;

    struct path *curPath = headPath;
    while (curPath != NULL) {
        if(isDirectory(curPath->name)) 
            searchDirectory(curPath->name, flags, headFile, tailFile);
        else {
            searchPatternInFile(curPath->name);
        }
        curPath = curPath->next;
    }

    // free the files linked list
    freeFilesList(&headFile);
}
