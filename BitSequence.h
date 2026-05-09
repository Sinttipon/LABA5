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

    Sequence<Bit> *Concat(const Sequence<Bit> *other) const override
    {
        if (!other)
            return new BitSequence(*this);
        BitSequence *result = new BitSequence(*this);
        size_t oldSize = result->bits->GetSize();
        size_t otherLen = other->GetLength();
        result->bits->Resize(oldSize + otherLen);
        for (size_t i = 0; i < otherLen; ++i)
            result->bits->Set(oldSize + i, other->Get(i));
        return result;
    }

    Sequence<Bit> *Where(std::function<bool(const Bit &)> predicate) const override
    {
        DynamicArray<Bit> *temp = new DynamicArray<Bit>();
        for (size_t i = 0; i < bits->GetSize(); ++i)
        {
            if (predicate(bits->Get(i)))
            {
                temp->Resize(temp->GetSize() + 1);
                temp->Set(temp->GetSize() - 1, bits->Get(i));
            }
        }
        BitSequence *result = new BitSequence();
        delete result->bits;
        result->bits = temp;
        return result;
    }
    template <typename TOut>
    Sequence<TOut> *Map(std::function<TOut(const Bit &)> func) const
    {
        size_t len = bits->GetSize();
        TOut *newArr = new TOut[len];
        for (size_t i = 0; i < len; ++i)
            newArr[i] = func(bits->Get(i));
        Sequence<TOut> *res = new MutableArraySequence<TOut>(newArr, len);
        delete[] newArr;
        return res;
    }

    template <typename TAcc>
    TAcc Reduce(std::function<TAcc(const TAcc &, const Bit &)> func, const TAcc &init) const
    {
        TAcc acc = init;
        for (size_t i = 0; i < bits->GetSize(); ++i)
            acc = func(acc, bits->Get(i));
        return acc;
    }

    BitSequence *And(const BitSequence &other) const
    {
        size_t minLen = bits->GetSize() < other.bits->GetSize() ? bits->GetSize() : other.bits->GetSize();
        BitSequence *result = new BitSequence();
        for (size_t i = 0; i < minLen; ++i)
        {
            Bit b = bits->Get(i) & other.bits->Get(i);
            result->bits->Resize(result->bits->GetSize() + 1);
            result->bits->Set(result->bits->GetSize() - 1, b);
        }
        return result;
    }

    BitSequence *Or(const BitSequence &other) const
    {
        size_t minLen = bits->GetSize() < other.bits->GetSize() ? bits->GetSize() : other.bits->GetSize();
        BitSequence *result = new BitSequence();
        for (size_t i = 0; i < minLen; ++i)
        {
            Bit b = bits->Get(i) | other.bits->Get(i);
            result->bits->Resize(result->bits->GetSize() + 1);
            result->bits->Set(result->bits->GetSize() - 1, b);
        }
        return result;
    }

    BitSequence *Xor(const BitSequence &other) const
    {
        size_t minLen = bits->GetSize() < other.bits->GetSize() ? bits->GetSize() : other.bits->GetSize();
        BitSequence *result = new BitSequence();
        for (size_t i = 0; i < minLen; ++i)
        {
            Bit b = bits->Get(i) ^ other.bits->Get(i);
            result->bits->Resize(result->bits->GetSize() + 1);
            result->bits->Set(result->bits->GetSize() - 1, b);
        }
        return result;
    }

    BitSequence *Not() const
    {
        BitSequence *result = new BitSequence();
        for (size_t i = 0; i < bits->GetSize(); ++i)
        {
            Bit b = ~bits->Get(i);
            result->bits->Resize(result->bits->GetSize() + 1);
            result->bits->Set(result->bits->GetSize() - 1, b);
        }
        return result;
    }

    std::string ToString() const
    {
        std::string s;
        for (size_t i = 0; i < bits->GetSize(); ++i)
            s += bits->Get(i).ToString();
        return s;
    }

    void Set(size_t index, Bit value)
    {
        if (index >= bits->GetSize())
            throw IndexOutOfRange(index, bits->GetSize(), "BitSequence::Set");
        bits->Set(index, value);
    }
};