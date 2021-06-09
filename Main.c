#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char fileName[50] = "";

int main(void)
{
    // Get console info
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hStdOut, &consoleInfo);
    WORD saved_attributes = consoleInfo.wAttributes;

    // Prepare to hide console cursor
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;

    // Get file name input
    printf("File name: ");
    fgets(fileName, 49, stdin);
    for (int i = 0; i < 49; i++)
    {
        if (fileName[i] == '\n')
        {
            fileName[i] = '\0';
            break;
        }
    }

    // Hide console cursor
    SetConsoleCursorInfo(hStdOut, &curInfo);

    // Open file
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL)
    {
        printf("File \"%s\" doesn't exist! \nPress ENTER to exit...", fileName);
        getchar();
        return -1;
    }

    // Seek to the end of the file and get file length
    fseek(fp, 0, SEEK_END);
    int dataLen = ftell(fp);

    // Output length
    printf("Data length: ");
    SetConsoleTextAttribute(hStdOut, 2);
    printf("%d\n", dataLen);

    // Allocate memory for the file data
    unsigned char *data = (unsigned char *)calloc(dataLen, sizeof(unsigned char));
    if (data == NULL)
    {
        fclose(fp);
        return -1;
    }

    // Seek to start and read data to buffer
    fseek(fp, 0, SEEK_SET);
    fread(data, sizeof(unsigned char), dataLen, fp);

    // Close file stream
    fclose(fp);

    // Print COLLUMN info
    SetConsoleTextAttribute(hStdOut, 6);
    printf("             00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F");

    // Print ROW info and byte data
    int rowCounter = 0;
    for (int i = 0; i < dataLen; i++)
    {
        if (i % 16 == 0)
        {
            rowCounter++;
            SetConsoleTextAttribute(hStdOut, 6);
            printf("\n0x%08x: ", rowCounter);
            SetConsoleTextAttribute(hStdOut, saved_attributes);
        }
        printf("0x%02x ", *(data + i));
    }

    // Free data
    free(data);

    // Wait for input to close the console
    getchar();
    return 0;
}