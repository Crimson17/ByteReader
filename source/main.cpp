#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


static std::string read_file_name(const int argc, const char** argv)
{
    std::string file_name;
    if (argc > 1) {
        file_name = argv[1];
    }
    else {
        std::cout << "File name: ";
        std::getline(std::cin, file_name);
    }
    return file_name;
}

static void read_file_bytes(const std::string& filepath, std::vector<uint8_t>& out_buffer)
{
    if (FILE* file = nullptr; fopen_s(&file, filepath.c_str(), "rb")) {
        std::cout << "File \"" << filepath << "\" could not be opened!" << std::endl;
        exit(1);
    }
    else {
        fseek(file, 0, SEEK_END);
        out_buffer.resize(ftell(file));
        rewind(file);
        fread(out_buffer.data(), 1, out_buffer.size(), file);
        fclose(file);
    }
}

static void save_file_bytes(const std::string& filepath, const std::vector<uint8_t>& byte_data)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cout << "File \"" << filepath << "\" could not be opened!" << std::endl;
        exit(1);
    }

    file << "File name: " << filepath << std::endl;
    file << "File size: " << byte_data.size() << " bytes\n" << std::endl;
    file << "             00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F     0 1 2 3 4 5 6 7 8 9 A B C D E F" << std::endl;

    uint64_t row_count;
    if (byte_data.size() % 16 == 0) {
        row_count = byte_data.size() / 16;
    }
    else {
        row_count = byte_data.size() / 16 + 1;
    }

    for (uint64_t i = 0; i < row_count; i++) {
        file << "0x" << std::setfill('0') << std::setw(8) << std::uppercase << std::hex << i << ": ";

        for (int j = 0; j < 16; j++) {
            if (const uint64_t index = i * 16 + j; index < byte_data.size()) {
                const auto value = static_cast<uint64_t>(byte_data[index]);
                file << "0x" << (value < 16 ? "0" : "") << std::uppercase << std::hex << value << " ";
            }
            else {
                file << "     ";
            }
        }

        file << "   ";
        
        for (int j = 0; j < 16; j++) {
            if (const uint64_t index = i * 16 + j; index < byte_data.size()) {
                if (const uint8_t temp_byte = byte_data[index]; temp_byte > 31 && temp_byte < 127) {
                    file << temp_byte << " ";
                }
                else {
                    file << ". ";
                }
            }
            else {
                break;
            }
        }
        
        file << std::endl;
    }

    file.close();
}

int main(const int argc, const char** argv)
{
    const std::string file_name = read_file_name(argc, argv);
    
    std::vector<uint8_t> byte_data = {};
    read_file_bytes(file_name, byte_data);

    const std::string pure_name = std::filesystem::path(file_name).filename().stem().string();
    save_file_bytes(pure_name + "_bytes.txt", byte_data);
}
