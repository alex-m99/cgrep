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
    - modify searchFiles to call readFile when file is found (think whether you wanna open any file or just text files)
    - search the files for pattern and output accordingly  
    - make sure the struct members are sufficient to output the lines for -B, -A and -C options*/ 


struct path {
    char *name;
    struct path *next;
};

struct file {
	char *name;
	struct line *curLine;
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


bool searchFiles(const char *sDir, int flags) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];
    
    // check if the passed path is already a file
    if(!isDirectory(sDir)) {
        printf("File: %s\n", sDir);
        return true;
    }

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


            // if directory
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
               // printf("Directory: %s\n", sPath);
                // recursively search for the next files in the directory
                searchFiles(sPath, flags); 
            }
            //if file
            else {
               // printf("File: %s\n", sPath);
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); 

    // close search handle
    FindClose(hFind); 

    return true;
}

void  freePathList(struct path **headPath) {
    struct path *curPath = *headPath;
    while (curPath != NULL) {
        struct path *nextPath = curPath->next;
        free(curPath->name);
        free(curPath);
        curPath = nextPath;
    }
    *headPath = NULL;
}

int main(int argc, char *argv[]) {

    int flags = 0;
    char *search;
    char *name;
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
    

    // get paths/files to apply to



    printf("Bitmask: %x\n", flags);
    printf("Search: %s\n", search);
   // printf("Argv: %s\n", *argv);

    if (argc > 0) {
        while (argc-- > 0) {
            struct path *new = (struct path *) malloc(sizeof(struct path));
            if (new == NULL) {
                printf("Failed to allocate memory for new path");
                return 1;
            }

            new->name = (char *) malloc(strlen(*argv)+1);
            if (new->name == NULL) {
                printf("Failed to allocate memory for name");
                free(new);
                return 1;
            }

            if (tailPath != NULL)
                tailPath->next = new;

            strcpy(new->name, *argv);
            new->next = NULL;
            tailPath = new;

            if (headPath == NULL)
                headPath = new;   
            ++argv;
        }
        
        //searchFiles(*argv, flags);
    }
    else {
        printf("Error: No files or paths provided to search\n");
        return 1;
    }

    curPath = headPath;
    while (curPath != NULL) {
        printf("Name: %s\n", curPath->name);
        curPath = curPath->next;
    }

    // free the linked list
    freePathList(&headPath);

    // "D:\\C Projects\\CGrep"
    return 0;
}