#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
        FILE *file1,*file2;
        char rec[BUF_SIZE];
        size_t bytesIn, byteOut;
        if (argc != 3)
        {
            printf("Usage: cp file1 file2\n");
            return 1;
        }
        file1 = fopen(argv[1], "rb");
        if (file1 == NULL)
        {
            perror(argv[1]);
            return 2;
        }
        file2 = fopen(argv[2], "wb");
        if (file2 == NULL)
        {
            perror(argv[2]);
            return 3;
        }
        
        while ((bytesIn = fread(rec, BUF_SIZE, 1, file1)) != 0)
        {
            byteOut = fwrite(rec, BUF_SIZE, 1, file2);
            if (bytesIn != byteOut)
            {
                printf("fatel write error");
                return 4;
            }
            
        }

        fclose(file1);
        fclose(file2);
    return 0;
}
