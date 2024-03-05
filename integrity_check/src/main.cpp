#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../hdr/ByteRow.hpp"

constexpr size_t ROW_LENGTH = 32;
std::array<uint8_t, ROW_LENGTH> CONST_XOR{
    0xc8, 0xe4, 0x1d, 0xab, 0x5e, 0xb6, 0x9e, 0x30,
     0x72, 0x4f, 0xff, 0xd0, 0x87, 0xed, 0xa5, 0xdb,
      0x1c, 0x4b, 0x64, 0xdc, 0xe5, 0xec, 0xff, 0x96,
       0xcc, 0x78, 0xc9, 0x21, 0xe6, 0x21, 0xa6, 0xae };

std::array<uint8_t, ROW_LENGTH> CONST_ADD{
    0x1D, 0x58, 0x1E, 0x09, 0x3D, 0x6A, 0x74, 0x2C,
     0x4A, 0x11, 0x48, 0x97, 0xC7, 0x85, 0xF0, 0x4E,
      0xB3, 0x0B, 0x92, 0xC4, 0x6C, 0x91, 0x60, 0x53,
       0x14, 0xB7, 0xE9, 0x33, 0x4D, 0x5E, 0xFC, 0x8E
};


ByteRow<ROW_LENGTH> processBlock(const ByteRow<ROW_LENGTH>& block)
{
    return fma(ByteRow<ROW_LENGTH>(CONST_ADD), block, ByteRow<ROW_LENGTH>(CONST_XOR));
}

std::vector<uint8_t> toBytes(const std::string& s)
{
    std::vector<uint8_t> bytes;
    bytes.reserve(s.size());

    std::transform(s.begin(), s.end(), std::back_inserter(bytes), [](char c)
        {
            return static_cast<uint8_t>(c);
        });

    return bytes;
}

int main(int argc, char* argv[])
{
    std::string inputPath;
    std::string hashSumExpected;

    ByteRow<ROW_LENGTH> hashSumResult;
    hashSumResult.fill(0);

    if (argc == 1)
    {
        std::cerr << "usage: checkSum [EXPECTED HASH SUM (32 char)] [FILE_PATH]\nor checkSum [FILE_PATH]\n";
        return 1;
    }
    else if (argc == 2)
    {
        std::cerr << "provided only one argument, finding file...\n";
        inputPath = argv[1];
    }
    else if (argc == 3)
    {
        inputPath = argv[2];
        hashSumExpected = argv[1];
    }
    else
    {
        std::cerr << "too much arguments\nusage: checkSum [EXPECTED HASH SUM (32 char)] [FILE_PATH]\nor checkSum [FILE_PATH]\n";
        return 1;
    }

    std::ifstream input(inputPath, std::ios::binary | std::ios::in);
    if (!input.is_open())
    {
        std::cerr << "file is not opened (maybe not present)" << std::endl;
    }
    else
    {
        auto byteFileContent = toBytes(std::string((std::istreambuf_iterator<char>(input)),
            std::istreambuf_iterator<char>()));

        if (byteFileContent.size() % ROW_LENGTH != 0)
        {
            std::vector<uint8_t> zeros(ROW_LENGTH - (byteFileContent.size() % ROW_LENGTH));
            byteFileContent.insert(byteFileContent.end(), zeros.begin(), zeros.end());
        }

        auto it = byteFileContent.begin();
        auto itEnd = byteFileContent.end();
        std::advance(it, ROW_LENGTH);

        for (; it != itEnd; std::advance(it, ROW_LENGTH))
        {
            std::array<uint8_t, ROW_LENGTH> rowData;
            std::copy_n(it - ROW_LENGTH, ROW_LENGTH, rowData.begin());
            hashSumResult = hashSumResult + processBlock(ByteRow<ROW_LENGTH>(rowData));
        }

        auto itHash = hashSumResult.begin();
        auto itHashEnd = hashSumResult.end();
        for (; itHash != itHashEnd; ++itHash)
        {
            std::cout << std::hex << static_cast<int>(*itHash);
        }
        std::cout << "\n";
        input.close();
    }

    if (argc == 3)
    {
        if (hashSumExpected.compare(static_cast<std::string>(hashSumResult)) == 0)
            std::cout << "OK! hash sums are identical\n";
        else std::cout << "not OK? hash sums are different\n";
    }
    


}

