#pragma once
#include "exceptions.hpp"
#include <algorithm>

template <typename T>
class DynamicArray
{
private:
    T *data;
    int size;

    void CopyFrom(const T *source, int count)
    {
        for (int i = 0; i < count; ++i)
            data[i] = source[i];
    }

public:
    DynamicArray(T *items, int count) : size(count)
    {
        if (count < 0)
            throw IndexOutOfRange("Negative count in DynamicArray");
        data = (count > 0) ? new T[count] : nullptr;
        if (count > 0)
            CopyFrom(items, count);
    }

    explicit DynamicArray(int size) : size(size)
    {
        if (size < 0)
            throw IndexOutOfRange("Negative size in DynamicArray");
        data = (size > 0) ? new T[size]() : nullptr;
    }

    DynamicArray(const DynamicArray<T> &other) : size(other.size)
    {
        data = (size > 0) ? new T[size] : nullptr;
        if (size > 0)
            CopyFrom(other.data, size);
    }

    ~DynamicArray() { delete[] data; }

    DynamicArray<T> &operator=(const DynamicArray<T> &other)
    {
        if (this == &other)
            return *this;
        delete[] data;
        size = other.size;
        data = (size > 0) ? new T[size] : nullptr;
        if (size > 0)
            CopyFrom(other.data, size);
        return *this;
    }

    T Get(int index)
    {
        if (index < 0 || index >= size)
            throw IndexOutOfRange("Index out of range in DynamicArray::Get");
        return data[index];
    }

    int GetSize()
    {
        return size;
    }

    void Set(int index, T value)
    {
        if (index < 0 || index >= size)
            throw IndexOutOfRange("Index out of range in DynamicArray::Set");
        data[index] = value;
    }

    void Resize(int newSize)
    {
        if (newSize < 0)
            throw IndexOutOfRange("Negative newSize in DynamicArray::Resize");
        if (newSize == size)
            return;

        T *newData = (newSize > 0) ? new T[newSize]() : nullptr;
        int copyCount = (newSize < size) ? newSize : size;
        if (copyCount > 0 && data != nullptr)
        {
            for (int i = 0; i < copyCount; ++i)
                newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        size = newSize;
    }

    T &operator[](int index) { return Get(index); }
    const T &operator[](int index) const
    {
        if (index < 0 || index >= size)
            throw IndexOutOfRange("Index out of range in operator[]");
        return data[index];
    }
};