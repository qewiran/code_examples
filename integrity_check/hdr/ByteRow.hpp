#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <sstream>

// я решил создать надстройку над классом std::array, чтобы перегрузить некоторые операторы, такие как operator+
template <size_t rowLength, typename ExactByteType = uint8_t>
class ByteRow
{
    std::array<ExactByteType, rowLength> values;

public:
    using Iterator = typename std::array<ExactByteType, rowLength>::iterator;
    using ConstIterator = typename std::array<ExactByteType, rowLength>::const_iterator;

    // вот тут я не уверен, нужно ли мне было вводить этот конструктор, вроде по правилу нуля можно было и отказаться
    ByteRow(const std::array<ExactByteType, rowLength>& byteSequence) : values(byteSequence) {}
    ByteRow() = default;

    ByteRow<rowLength> operator+(const ByteRow<rowLength>& otherRow) const
    {
        std::array<ExactByteType, rowLength> sum;

        ConstIterator it = this->cbegin();
        ConstIterator itOther = otherRow.cbegin();
        std::for_each(sum.begin(), sum.end(), [&it, &itOther](ExactByteType sumElement)
            {
                sumElement = (*it + *itOther) % UINT8_MAX;
                ++itOther; ++it;
            });
        return ByteRow<rowLength>(sum);
    }

    ByteRow<rowLength> operator*(const ByteRow<rowLength>& otherRow) const
    {
        std::array<ExactByteType, rowLength> conv;

        ConstIterator it = this->cbegin();
        ConstIterator itOther = otherRow.cbegin();

        std::for_each(conv.begin(), conv.end(), [&it, &itOther](ExactByteType convElement)
            {
                convElement = (*it * *itOther) % UINT8_MAX;
                ++itOther; ++it;
            });
        return ByteRow<rowLength>(conv);
    }

    ByteRow<rowLength> operator^(const ByteRow<rowLength>& otherRow) const
    {
        std::array<ExactByteType, rowLength> xor_;

        ConstIterator it = this->cbegin();
        ConstIterator itOther = otherRow.cbegin();

        std::for_each(xor_.begin(), xor_.end(), [&it, &itOther](ExactByteType xor_Element)
            {
                xor_Element = (*it ^ *itOther) % UINT8_MAX;
                ++itOther; ++it;
            });
        return ByteRow<rowLength>(xor_);
    }
    void fill(ExactByteType valueInit)
    {
        values.fill(valueInit);
        Iterator it = values.begin();
        Iterator it_end = values.end();
        if (it == it_end)
        {
            std::cout << "IT = END in BYTEROW::FILL\n";
        }
    }

    Iterator begin()
    {
        return this->values.begin();
    }

    Iterator end()
    {
        return this->values.end();
    }

    ConstIterator cbegin() const
    {
        return this->values.cbegin();
    }

    ConstIterator cend() const
    {
        return this->values.cend();
    }

    // это чтобы сравнить итоговые хэш-суммы в итоге 
    operator std::string()
    {
        std::ostringstream convert;
        std::for_each(this->begin(), this->end(), [&convert](ExactByteType rowElement)
            {
                convert << std::hex << static_cast<int>(rowElement);
            });
        return convert.str();
    }
};

// я выбрал такие функции для алгоритма, потому что в любом современном десктопном процессоре есть похожие векторные инструкции
// насчет присутствия fxa не уверен, но в общем, легко векторизовать программу для обработки чудовищно больших файлов 

template <typename ExactByteType, size_t rowLength>
ByteRow<rowLength, ExactByteType> fxa(const ByteRow<rowLength, ExactByteType>& xor1, const ByteRow<rowLength, ExactByteType>& xor2,
    const ByteRow<rowLength, ExactByteType>& add)
{
    return (xor1 ^ xor2) + add;
}

template <typename ExactByteType, size_t rowLength>
ByteRow<rowLength, ExactByteType> fma(const ByteRow<rowLength, ExactByteType>& mul1, const ByteRow<rowLength, ExactByteType>& mul2,
    const ByteRow<rowLength, ExactByteType>& add)
{
    return (mul1 * mul2) + add;
}
