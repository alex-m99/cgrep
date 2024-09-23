#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool ListDirectoryContents(const char *sDir)
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

ListDirectoryContents("D:\\C Projects\\CGrep");

return 0;
}