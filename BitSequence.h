#pragma once
#include "Sequence.h"
#include "ArraySequence.h"
#include <stdexcept>
#include <string>

class Bit
{
private:
    bool value;

public:
    Bit() : value(false) {}
    explicit Bit(bool v) : value(v) {}
    Bit(int v) : value(v != 0) {}

    operator bool() const { return value; }
    bool operator==(const Bit &other) const { return value == other.value; }
    bool operator!=(const Bit &other) const { return value != other.value; }

    Bit operator&(const Bit &other) const { return Bit(value && other.value); }
    Bit operator|(const Bit &other) const { return Bit(value || other.value); }
    Bit operator^(const Bit &other) const { return Bit(value != other.value); }
    Bit operator~() const { return Bit(!value); }

    std::string ToString() const { return value ? "1" : "0"; }
};

class BitSequence : public Sequence<Bit>
{
private:
    DynamicArray<Bit> *bits;

public:
    BitSequence() : bits(new DynamicArray<Bit>()) {}
    BitSequence(const bool *arr, size_t count) : bits(new DynamicArray<Bit>(count))
    {
        for (size_t i = 0; i < count; ++i)
            bits->Set(i, Bit(arr[i]));
    }
    BitSequence(const BitSequence &other) : bits(new DynamicArray<Bit>(*other.bits)) {}

    ~BitSequence() override { delete bits; }

    Bit GetFirst() const override
    {
        if (bits->GetSize() == 0)
            throw IndexOutOfRange(0, 0, "GetFirst on empty BitSequence");
        return bits->Get(0);
    }

    Bit GetLast() const override
    {
        if (bits->GetSize() == 0)
            throw IndexOutOfRange(0, 0, "GetLast on empty BitSequence");
        return bits->Get(bits->GetSize() - 1);
    }

    Bit Get(size_t index) const override { return bits->Get(index); }
    size_t GetLength() const override { return bits->GetSize(); }

    Sequence<Bit> *GetSubsequence(size_t startIndex, size_t endIndex) const override
    {
        if (startIndex > endIndex || endIndex >= bits->GetSize())
            throw IndexOutOfRange(startIndex, bits->GetSize(), "BitSequence::GetSubsequence");

        size_t subSize = endIndex - startIndex + 1;
        bool *subArr = new bool[subSize];
        for (size_t i = 0; i < subSize; ++i)
            subArr[i] = bits->Get(startIndex + i);
        Sequence<Bit> *result = new BitSequence(subArr, subSize);
        delete[] subArr;
        return result;
    }

    Sequence<Bit> *Append(const Bit &item) const override
    {
        BitSequence *copy = new BitSequence(*this);
        copy->bits->Resize(copy->bits->GetSize() + 1);
        copy->bits->Set(copy->bits->GetSize() - 1, item);
        return copy;
    }

    Sequence<Bit> *Prepend(const Bit &item) const override
    {
        BitSequence *copy = new BitSequence(*this);
        size_t oldSize = copy->bits->GetSize();
        copy->bits->Resize(oldSize + 1);
        for (size_t i = oldSize; i > 0; --i)
            copy->bits->Set(i, copy->bits->Get(i - 1));
        copy->bits->Set(0, item);
        return copy;
    }

    Sequence<Bit> *InsertAt(const Bit &item, size_t index) const override
    {
        if (index > bits->GetSize())
            throw IndexOutOfRange(index, bits->GetSize() + 1, "BitSequence::InsertAt");
        BitSequence *copy = new BitSequence(*this);
        size_t oldSize = copy->bits->GetSize();
        copy->bits->Resize(oldSize + 1);
        for (size_t i = oldSize; i > index; --i)
            copy->bits->Set(i, copy->bits->Get(i - 1));
        copy->bits->Set(index, item);
        return copy;
    }

    Sequence<Bit> *Concat(const Sequence<Bit> *) const override { throw std::runtime_error("."); }
    Sequence<Bit> *Where(std::function<bool(const Bit &)>) const override { throw std::runtime_error("."); }
};