#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxFileNameLen 100

char storeFileName[] = "bytes.txt";
char dataFileName[maxFileNameLen] = "";

int main(void)
{
    // Get file name input
    printf("File name: ");
    fgets(dataFileName, maxFileNameLen - 1, stdin);
    for (int i = 0; i < maxFileNameLen; i++) {
        if (dataFileName[i] == '\n') {
            dataFileName[i] = '\0';
            break;
        }
    }

    // Compare file names
    if (!strcmp(storeFileName, dataFileName)) {
        printf("The file name can't be \"%s\"\nPress ENTER to exit...", storeFileName);
        getchar();
        return -1;
    }

    // Open file
    FILE* dataFile = fopen(dataFileName, "rb");
    if (dataFile == NULL) {
        printf("File \"%s\" doesn't exist! \nPress ENTER to exit...", dataFileName);
        getchar();
        return -1;
    }

    // Open/Create "bytes.txt"
    FILE* storeFile = fopen(storeFileName, "w");
    if (storeFile == NULL) {
        fclose(dataFile);
        return -1;
    }

    // Save file name to text file
    fprintf(storeFile, "File name: %s\n", dataFileName);

    // Seek to the end of the file and get file length
    fseek(dataFile, 0, SEEK_END);
    int dataLen = ftell(dataFile);

    // Output length
    fprintf(storeFile, "Data length: %d\n", dataLen);

    // Allocate memory for the file data
    unsigned char* data = (unsigned char*)calloc(dataLen, sizeof(unsigned char));
    if (data == NULL) {
        fclose(dataFile);
        return -1;
    }

    // Seek to start and read data to buffer
    fseek(dataFile, 0, SEEK_SET);
    fread(data, sizeof(unsigned char), dataLen, dataFile);

    // Close file stream
    fclose(dataFile);

    // Print COLLUMN info
    fprintf(storeFile, "             00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F     0 1 2 3 4 5 6 7 8 9 A B C D E F");

    // Print ROW info, byte data and data as chars
    int rowCounter = 0;
    for (int i = 0; i < dataLen; i++) {
        if (i % 16 == 0) {
            if (i != 0) {
                fprintf(storeFile, "   ");
                for (int j = 16; j > 0; j--) {
                    if (*(data + i - j) > 31 && *(data + i - j) < 127) {
                        fprintf(storeFile, "%c ", *(data + i - j));
                    }
                    else {
                        fprintf(storeFile, ". ");
                    }
                }
            }
            fprintf(storeFile, "\n0x%08X: ", rowCounter);
            rowCounter++;
        }
        fprintf(storeFile, "0x%02X ", *(data + i));
    }

    // Close store file and free data
    fclose(storeFile);
    free(data);

    // Wait for input to close the console
    printf("Finished!");
    getchar();
    return 0;
}