/*TO DO:
    - fix makefile error
    - implement searchPatternInFile
    - search the files for pattern and output accordingly  
    - make sure the struct members are sufficient to output the lines for -B, -A and -C options*/ 

#include "../include/search.h"
#include <stdio.h>
int main(int argc, char *argv[]) {
    int flags = 0;
    char *sPattern;

    // linked list for path traversal
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
        sPattern = *argv++; 
        --argc;
    }
    else {
        printf("Error: No search pattern provided\n");
        return 1;
    }
    
    printf("Bitmask: %x\n", flags);
    printf("Search: %s\n", sPattern);
   // printf("Argv: %s\n", *argv);

     // build the linked list with the paths/files to apply grep to
    if (argc > 0) {
        while (argc-- > 0) {
            addPathNode(*argv, &headPath, &tailPath);
            ++argv;
        }
    }
    else {
        printf("Error: No files or paths provided to search\n");
        return 1;
    }

    // struct path *curPath = headPath;
    // while (curPath != NULL) {
    //     printf("Name: %s\n", curPath->name);
    //     curPath = curPath->next;
    // }

    beginSearch(sPattern, flags, headPath);
    // free the paths linked list
    freePathsList(&headPath);

    // "D:\\C Projects\\CGrep"
    return 0;
}