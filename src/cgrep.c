#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define W 0x01
#define I 0x02
#define N 0x04

/*TO DO:
    - add relative path option to cmd input
    - add option to use multiple files as arguments (maybe with linked list)
    - add option for uppecrase cmd flags as well
    - modify searchDirectory to call readFile when file is found (think whether you wanna open any file or just text files)
    - search the files for pattern and output accordingly  
    - make sure the struct members are sufficient to output the lines for -B, -A and -C options*/ 


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


struct line *searchPatternInFile(char *file) {
    struct line *headLine = NULL;
    struct line *tailLine = NULL;

    return NULL;
}


void addFileNode(char *fileName, struct file *headFile, struct file *tailFile) {
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

        if (tailFile != NULL)
            tailFile->next = newFile;

        strcpy(newFile->name, fileName);
        newFile->next = NULL;
        tailFile = newFile;

        if (headFile == NULL)
            headFile = newFile;  
}

void addPathNode(char *pathName, struct path *headPath, struct path *tailPath) {
    struct path *newPath = (struct path *) malloc(sizeof(struct path));
        if (newPath == NULL) {
            printf("Failed to allocate memory");
            return 1;
        }

        newPath->name = (char *) malloc(strlen(pathName)+1);
        if (newPath->name == NULL) {
            printf("Failed to allocate memory");
            free(newPath);
            return 1;
        }

        if (tailPath != NULL)
            tailPath->next = newPath;

        strcpy(newPath->name, pathName);
        newPath->next = NULL;
        tailPath = newPath;

        if (headPath == NULL)
            headPath = newPath;   
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

int main(int argc, char *argv[]) {

    int flags = 0;
    char *search;
    char *name;

    // linked list for path traversal
    struct path *curPath = NULL;
    struct path *headPath = NULL;
    struct path *tailPath = NULL;

    // get options
    while (--argc > 0 && (*++argv)[0] == '-') {
        for (char *s = argv[0] + 1; *s != '\0'; ++s) 
            switch (*s) {
                case 'w':
                    flags |= W;
                    break;
                case 'i':
                    flags |= I;
                    break;
                case 'n':
                    flags |= N;
            }
    }

    // get search pattern
    if(argc > 0) {
        search = *argv++; 
        --argc;
    }
    else {
        printf("Error: No search pattern provided\n");
        return 1;
    }
    
    printf("Bitmask: %x\n", flags);
    printf("Search: %s\n", search);
   // printf("Argv: %s\n", *argv);

     // build the linked list with the paths/files to apply grep to
    if (argc > 0) {
        while (argc-- > 0) {
            addPathNode(*argv, headPath, tailPath);
            ++argv;
        }
    }
    else {
        printf("Error: No files or paths provided to search\n");
        return 1;
    }

    beginSearch(headPath, flags);

    // curPath = headPath;
    // while (curPath != NULL) {
    //     printf("Name: %s\n", curPath->name);
    //     curPath = curPath->next;
    // }

    // free the paths linked list
    freePathsList(&headPath);

    // "D:\\C Projects\\CGrep"
    return 0;
}