#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>


typedef unsigned char byte;

const std::string bytesFileName = "bytes.txt";

int main() {
    // Getting the file name
    std::string fileName;
    printf("File name: ");
    std::getline(std::cin, fileName);

    // Comparing the file names
    if (fileName == bytesFileName) {
        printf("The file name can not be \"%s\"!", bytesFileName.c_str());
        std::cin.get();
        return 1;
    }

    // Opening the file
    FILE* inputFile = fopen(fileName.c_str(), "rb");
    if (!inputFile) {
        printf("File \"%s\" could not be opened!", fileName.c_str());
        std::cin.get();
        return 2;
    }

    // Creating the output file
    FILE* outputFile = fopen(bytesFileName.c_str(), "w");
    if (!outputFile) {
        printf("File \"%s\" could not be created!", bytesFileName.c_str());
        std::cin.get();
        return 3;
    }

    // Saving the file name
    fprintf(outputFile, "File name: %s\n", fileName.c_str());

    // Getting the file size
    fseek(inputFile, 0, SEEK_END);
    const int fileSize = ftell(inputFile);

    // Saving the file size
    fprintf(outputFile, "File size: %d\n", fileSize);

    // Allocating the byte buffer
    std::vector<byte> dataBuffer(fileSize);

    // Reading the file
    fseek(inputFile, 0, SEEK_SET);
    fread(&dataBuffer[0], sizeof(byte), fileSize, inputFile);

    // Closing the input file
    fclose(inputFile);

    // Saving the collumn info
    fprintf(outputFile, "             00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F     0 1 2 3 4 5 6 7 8 9 A B C D E F\n");

    // Calculating rows
    int rowCount = 0;
    if (fileSize % 16 == 0) {
        rowCount = fileSize / 16;
    }
    else {
        rowCount = fileSize / 16 + 1;
    }

    // Saving data
    for (int i = 0; i < rowCount; i++) {
        // Saving the row id
        fprintf(outputFile, "0x%08X: ", i);

        // Saving the bytes
        for (int j = 0; j < 16; j++) {
            const int ind = i * 16 + j;
            if (ind < fileSize) {
                fprintf(outputFile, "0x%02X ", dataBuffer[ind]);
            }
            else {
                fprintf(outputFile, "0x   ");
            }
        }
        
        // Saving the translated bytes
        fprintf(outputFile, "   ");
        for (int j = 0; j < 16; j++) {
            const int ind = i * 16 + j;
            if (ind < fileSize) {
                const byte tempByte = dataBuffer[ind];
                if (tempByte > 31 && tempByte < 127) {
                    fprintf(outputFile, "%c ", tempByte);
                }
                else {
                    fprintf(outputFile, ". ");
                }
            }
            else {
                break;
            }
        }
        fprintf(outputFile, "\n");
    }

    // Closing the output file
    fclose(outputFile);

    // Exit
    printf("Finished!");
    std::cin.get();
}