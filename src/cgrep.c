#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define W 0x01
#define I 0x02
#define N 0x04

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
                ListDirectoryContents(sPath); 
            }
            //if file
            else{
               // printf("File: %s\n", sPath);
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); 

    // closesearch handle
    FindClose(hFind); 

    return true;
}

int main(int argc, char *argv[]) {

    int flags = 0;

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

    printf("Bitmask: %x\n", flags);

    searchFiles("D:\\C Projects\\CGrep", flags);

    return 0;
}