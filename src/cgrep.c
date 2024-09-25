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
    - search the files for pattern and output accordingly  */ 

bool searchFiles(const char *sDir, int flags)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];

    // string buffer for hFind. search for all files and directories
    sprintf(sPath, "%s\\*.*", sDir);
    printf("sPath outside: %s\n", sPath);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        printf("Path not found: [%s]\n", sDir);
        return false;
    }

    do
    {
        // first two directories will be . and ..
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
        {
            // build filepath with parameter path and found filename
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);
            printf("sPath inside: %s\n", sPath);


            // if directory
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
            {
               // printf("Directory: %s\n", sPath);
                // recursively search for the next files in the directory
                searchFiles(sPath, flags); 
            }
            //if file
            else{
               // printf("File: %s\n", sPath);
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); 

    // close search handle
    FindClose(hFind); 

    return true;
}

int main(int argc, char *argv[]) {

    int flags = 0;
    char *search;

    // get options
    while (-- argc > 0 && (*++argv)[0] == '-') {
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
    

    // get files to apply to


    printf("Bitmask: %x\n", flags);
    printf("Search: %s\n", search);
    printf("Argv: %s\n", *argv);

    if (argc > 0) {
        searchFiles(*argv, flags);
    }
    else {
        printf("Error: No files or path provided to search\n");
        return 1;
    }

    // "D:\\C Projects\\CGrep"
    return 0;
}