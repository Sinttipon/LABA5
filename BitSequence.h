#pragma once
#include "Sequence.h"
#include "DynamicArray.hpp"
#include "ArraySequence.h"
#include <stdexcept>
#include <string>
#include <cstdint>

class Bit
{
private:
    uint8_t value;

public:
    Bit() : value(0) {}
    Bit(bool v) : value(v ? 1 : 0) {}
    Bit(int v) : value(v != 0 ? 1 : 0) {}

    operator bool() const { return value != 0; }
    bool operator==(const Bit &other) const { return value == other.value; }
    bool operator!=(const Bit &other) const { return value != other.value; }

    Bit operator&(const Bit &other) const { return Bit(value & other.value); }
    Bit operator|(const Bit &other) const { return Bit(value | other.value); }
    Bit operator^(const Bit &other) const { return Bit(value ^ other.value); }
    Bit operator~() const { return Bit(value ^ 1); }

    std::string ToString() const { return value ? "1" : "0"; }
};

class BitSequence : public Sequence<Bit>
{
private:
    DynamicArray<uint8_t> bytes; 
    size_t bitLength;          

    size_t byteIndex(size_t bitIndex) const { return bitIndex / 8; }
    size_t bitOffset(size_t bitIndex) const { return bitIndex % 8; }

public:
    BitSequence() : bitLength(0) {}

    BitSequence(const bool *arr, size_t count) : bitLength(count)
    {
        size_t byteCount = (count + 7) / 8;
        bytes.Resize(byteCount);
        for (size_t i = 0; i < byteCount; ++i)
            bytes.Set(i, 0);
        for (size_t i = 0; i < count; ++i)
            if (arr[i])
                bytes[byteIndex(i)] |= (1 << bitOffset(i));
    }

    BitSequence(const BitSequence &other) : bytes(other.bytes), bitLength(other.bitLength) {}

    ~BitSequence() override {}

    Bit GetFirst() const override
    {
        if (bitLength == 0)
            throw IndexOutOfRange(0, 0, "GetFirst в пустом BitSequence");
        return Get(0);
    }

    Bit GetLast() const override
    {
        if (bitLength == 0)
            throw IndexOutOfRange(0, 0, "GetLast в пустом BitSequence");
        return Get(bitLength - 1);
    }

    Bit Get(size_t index) const override
    {
        if (index >= bitLength)
            throw IndexOutOfRange(index, bitLength, "BitSequence::Get");
        size_t bi = byteIndex(index);
        size_t bo = bitOffset(index);
        return Bit((bytes[bi] >> bo) & 1);
    }

    size_t GetLength() const override { return bitLength; }

    void Set(size_t index, Bit value)
    {
        if (index >= bitLength)
            throw IndexOutOfRange(index, bitLength, "BitSequence::Set");
        size_t bi = byteIndex(index);
        size_t bo = bitOffset(index);
        if (value)
            bytes[bi] |= (1 << bo);
        else
            bytes[bi] &= ~(1 << bo);
    }

    Sequence<Bit> *GetSubsequence(size_t startIndex, size_t endIndex) const override
    {
        if (startIndex > endIndex || endIndex >= bitLength)
            throw IndexOutOfRange(startIndex, bitLength, "BitSequence::GetSubsequence");

        size_t subSize = endIndex - startIndex + 1;
        bool *subArr = new bool[subSize];
        for (size_t i = 0; i < subSize; ++i)
            subArr[i] = Get(startIndex + i);
        Sequence<Bit> *result = new BitSequence(subArr, subSize);
        delete[] subArr;
        return result;
    }

    Sequence<Bit> *Append(const Bit &item) const override
    {
        BitSequence *copy = new BitSequence(*this);
        size_t oldLen = copy->bitLength;
        copy->bitLength++;
        size_t newByteCount = (copy->bitLength + 7) / 8;
        if (newByteCount > copy->bytes.GetSize())
            copy->bytes.Resize(newByteCount);
        copy->Set(oldLen, item);
        return copy;
    }

    Sequence<Bit> *Prepend(const Bit &item) const override
    {
        BitSequence *copy = new BitSequence(*this);
        size_t oldLen = copy->bitLength;
        copy->bitLength++;
        size_t newByteCount = (copy->bitLength + 7) / 8;
        if (newByteCount > copy->bytes.GetSize())
            copy->bytes.Resize(newByteCount);
        for (size_t i = copy->bitLength - 1; i > 0; --i)
            copy->Set(i, copy->Get(i - 1));
        copy->Set(0, item);
        return copy;
    }

    Sequence<Bit> *InsertAt(const Bit &item, size_t index) const override
    {
        if (index > bitLength)
            throw IndexOutOfRange(index, bitLength + 1, "BitSequence::InsertAt");
        BitSequence *copy = new BitSequence(*this);
        size_t oldLen = copy->bitLength;
        copy->bitLength++;
        size_t newByteCount = (copy->bitLength + 7) / 8;
        if (newByteCount > copy->bytes.GetSize())
            copy->bytes.Resize(newByteCount);
        for (size_t i = copy->bitLength - 1; i > index; --i)
            copy->Set(i, copy->Get(i - 1));
        copy->Set(index, item);
        return copy;
    }

    Sequence<Bit> *Concat(const Sequence<Bit> *other) const override
    {
        if (!other)
            return new BitSequence(*this);
        BitSequence *result = new BitSequence(*this);
        size_t oldLen = result->bitLength;
        size_t otherLen = other->GetLength();
        result->bitLength = oldLen + otherLen;
        size_t newByteCount = (result->bitLength + 7) / 8;
        if (newByteCount > result->bytes.GetSize())
            result->bytes.Resize(newByteCount);
        for (size_t i = 0; i < otherLen; ++i)
            result->Set(oldLen + i, other->Get(i));
        return result;
    }

    Sequence<Bit> *Where(std::function<bool(const Bit &)> predicate) const override
    {
        bool *temp = new bool[bitLength];
        size_t count = 0;
        for (size_t i = 0; i < bitLength; ++i)
            if (predicate(Get(i)))
                temp[count++] = Get(i);
        BitSequence *result = new BitSequence(temp, count);
        delete[] temp;
        return result;
    }

    template <typename TOut>
    Sequence<TOut> *Map(std::function<TOut(const Bit &)> func) const
    {
        size_t len = bitLength;
        TOut *newArr = new TOut[len];
        for (size_t i = 0; i < len; ++i)
            newArr[i] = func(Get(i));
        Sequence<TOut> *res = new MutableArraySequence<TOut>(newArr, len);
        delete[] newArr;
        return res;
    }

    template <typename TAcc>
    TAcc Reduce(std::function<TAcc(const TAcc &, const Bit &)> func, const TAcc &init) const
    {
        TAcc acc = init;
        for (size_t i = 0; i < bitLength; ++i)
            acc = func(acc, Get(i));
        return acc;
    }

    BitSequence *And(const BitSequence &other) const
    {
        size_t minLen = bitLength < other.bitLength ? bitLength : other.bitLength;
        bool *tmp = new bool[minLen];
        for (size_t i = 0; i < minLen; ++i)
            tmp[i] = Get(i) & other.Get(i);
        BitSequence *result = new BitSequence(tmp, minLen);
        delete[] tmp;
        return result;
    }

    BitSequence *Or(const BitSequence &other) const
    {
        size_t minLen = bitLength < other.bitLength ? bitLength : other.bitLength;
        bool *tmp = new bool[minLen];
        for (size_t i = 0; i < minLen; ++i)
            tmp[i] = Get(i) | other.Get(i);
        BitSequence *result = new BitSequence(tmp, minLen);
        delete[] tmp;
        return result;
    }

    BitSequence *Xor(const BitSequence &other) const
    {
        size_t minLen = bitLength < other.bitLength ? bitLength : other.bitLength;
        bool *tmp = new bool[minLen];
        for (size_t i = 0; i < minLen; ++i)
            tmp[i] = Get(i) ^ other.Get(i);
        BitSequence *result = new BitSequence(tmp, minLen);
        delete[] tmp;
        return result;
    }

    BitSequence *Not() const
    {
        bool *tmp = new bool[bitLength];
        for (size_t i = 0; i < bitLength; ++i)
            tmp[i] = ~Get(i);
        BitSequence *result = new BitSequence(tmp, bitLength);
        delete[] tmp;
        return result;
    }

    std::string ToString() const
    {
        std::string s;
        for (size_t i = 0; i < bitLength; ++i)
            s += Get(i).ToString();
        return s;
    }
};