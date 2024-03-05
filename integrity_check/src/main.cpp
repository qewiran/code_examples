#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../hdr/ByteRow.hpp"

constexpr size_t ROW_LENGTH = 32;
const std::array<uint8_t, ROW_LENGTH> CONST_MUL{
    0xc8, 0xe4, 0x1d, 0xab, 0x5e, 0xb6, 0x9e, 0x30,
     0x72, 0x4f, 0xff, 0xd0, 0x87, 0xed, 0xa5, 0xdb,
      0x1c, 0x4b, 0x64, 0xdc, 0xe5, 0xec, 0xff, 0x96,
       0xcc, 0x78, 0xc9, 0x21, 0xe6, 0x21, 0xa6, 0xae };

const std::array<uint8_t, ROW_LENGTH> CONST_ADD{
    0x1D, 0x58, 0x1E, 0x09, 0x3D, 0x6A, 0x74, 0x2C,
     0x4A, 0x11, 0x48, 0x97, 0xC7, 0x85, 0xF0, 0x4E,
      0xB3, 0x0B, 0x92, 0xC4, 0x6C, 0x91, 0x60, 0x53,
       0x14, 0xB7, 0xE9, 0x33, 0x4D, 0x5E, 0xFC, 0x8E };


// функция, которая обрабатывает байтовую строку: свертка входящей строки row
// со строкой CONST_MUL и сложение с CONST_ADD (конструктор через variadic templates не сделал,
// поэтому выше объявлены константные массивы и уже через конструктор ByteRow(const std::array<>&)
//  временные объекты передаются в fma)
ByteRow<ROW_LENGTH> processRow(const ByteRow<ROW_LENGTH>& row)
{
    return fxa(row, ByteRow<ROW_LENGTH>(CONST_MUL), ByteRow<ROW_LENGTH>(CONST_ADD));
}

// функция, которая переводит строку в вектор байтов (я решил здесь использовать только инстанс
// вектора с типом uint8_t, но по-хорошему надо было сделать шаблон, а лучше еще и sizeof(параметра шаблона) == 1)
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

    std::string inputPath; // объект строки пути к проверяемому файлу
    std::string hashSumExpected; // объект строки ожидаемой хэш-суммы

    ByteRow<ROW_LENGTH> hashSumResult; //объект байтовой строки, которая будет результатом хэширования
    hashSumResult.fill(0); //заполнение нулями, чтобы получить далее кумулятивную поэлементную сумму

    if (argc == 1) // не указано аргументов 
    {
        std::cerr << "usage: checkSum [EXPECTED HASH SUM (32 char)] [FILE_PATH]\nor checkSum [FILE_PATH]\n";
        return 1;
    }
    else if (argc == 2) //указан только файл (далее в коде будет момент (*), где проверяется, открылся ли файл)
    {
        std::cerr << "provided only one argument, finding file...\n";
        inputPath = argv[1];
    }
    else if (argc == 3) // указана и ожидаемая хэш-сумма, и путь к проверяемому файлу, ОК
    {
        inputPath = argv[2];
        hashSumExpected = argv[1];
    }
    else
    {
        std::cerr << "too much arguments\nusage: checkSum [EXPECTED HASH SUM (32 char)] [FILE_PATH]\nor checkSum [FILE_PATH]\n";
        return 1;
    }

    std::ifstream input(inputPath, std::ios::binary | std::ios::in); //объект ifstream для считывания с файла
    if (!input.is_open())
    {
        std::cerr << "file is not opened (maybe not present)" << std::endl; // (*)
    }
    else
    {
        auto byteFileContent = toBytes(std::string((std::istreambuf_iterator<char>(input)),
            std::istreambuf_iterator<char>())); // перевод исходных символов строки в тип uint8_t

        //добавить в конец нули до кратности ROW_LENGTH, если size() получившегося вектора uint8_t не кратен ROW_LENGTH 
        if (byteFileContent.size() % ROW_LENGTH != 0)
        {
            std::vector<uint8_t> zeros(ROW_LENGTH - (byteFileContent.size() % ROW_LENGTH));
            byteFileContent.insert(byteFileContent.end(), zeros.begin(), zeros.end());
        }

        auto it = byteFileContent.begin();
        auto itEnd = byteFileContent.end();
        std::advance(it, ROW_LENGTH); // я решил обрабатывать строку, так чтобы обрабатываемую строку
        // получать смещением итератора назад, а не вперед, поэтому но 0-м шаге отодвинул позицию итератора

        for (; it != itEnd; std::advance(it, ROW_LENGTH))
        {
            std::array<uint8_t, ROW_LENGTH> rowData;
            std::copy_n(it - ROW_LENGTH, ROW_LENGTH, rowData.begin()); //вот тут в rowData копируется с места it - ROW_LENGTH до it
            hashSumResult = hashSumResult + processRow(ByteRow<ROW_LENGTH>(rowData));
        }

        auto itHash = hashSumResult.begin();
        auto itHashEnd = hashSumResult.end();
        for (; itHash != itHashEnd; ++itHash)
        {
            std::cout << std::hex << static_cast<int>(*itHash); //посимвольный вывод хэш-суммы
        }
        std::cout << "\n"; 
        input.close();
    }

    // вывод о проделанной работе
    if (argc == 3)
    {
        if (hashSumExpected.compare(static_cast<std::string>(hashSumResult)) == 0)
            std::cout << "OK! hash sums are identical\n";
        else std::cout << "not OK? hash sums are different\n";
    }



}

