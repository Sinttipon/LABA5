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

    Sequence<Bit> *GetSubsequence(size_t, size_t) const override { throw std::runtime_error("."); }
    Sequence<Bit> *Append(const Bit &) const override { throw std::runtime_error("."); }
    Sequence<Bit> *Prepend(const Bit &) const override { throw std::runtime_error("."); }
    Sequence<Bit> *InsertAt(const Bit &, size_t) const override { throw std::runtime_error("."); }
    Sequence<Bit> *Concat(const Sequence<Bit> *) const override { throw std::runtime_error("."); }
    Sequence<Bit> *Where(std::function<bool(const Bit &)>) const override { throw std::runtime_error("."); }
};