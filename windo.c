#include <stdio.h>
#include <windows.h>

#define BUF_SIZE 256

int main(int argc, LPTSTR argv[]){

HANDLE FILE1, FILE2;
DWORD NIN, NOUT;


CHAR BUFFER[BUF_SIZE];

 if(argc != 3){
    printf("usage: cp ein file file2\n");
    return 1;
 }
FILE1 = CreateFile(argv[1], GENERIC_READ,FILE_SHARE_READ,
                         NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

     if (FILE1 <=0)
    {
        printf("cannot cpen input file, error %x \n", GetLastError());
        CloseHandle(FILE1);
        CloseHandle(FILE2);
        return 2;
    }
FILE2 = CreateFile(argv[2], GENERIC_WRITE,FILE_SHARE_WRITE,
                         NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

     if (FILE2 <=0)
    {
        printf("cannot cpen input file, error %x \n", GetLastError());
        CloseHandle(FILE1);
        CloseHandle(FILE2);
        return 3;
    }
    while(ReadFile(FILE1, BUFFER, BUF_SIZE,&NIN,NULL) && FILE1 > 0){
    WriteFile(FILE2,BUFFER,NIN,&NOUT,NULL);
    if(NIN != NOUT){
        printf("Fatal write, error %x \n", GetLastError);
        return 4;
    }

}
CloseHandle(FILE1);
CloseHandle(FILE2);
return 0;
}