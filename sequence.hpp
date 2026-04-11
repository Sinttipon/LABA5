#pragma once
#include <functional>
#include <stdexcept>

template <typename T>
class Sequence
{
public:
    virtual ~Sequence() = default;
    virtual T Get(int index) const = 0;
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T> *Append(const T &item) const = 0;
    virtual Sequence<T> *Prepend(const T &item) const = 0;
    virtual Sequence<T> *InsertAt(const T &item, int index) const = 0;
    virtual Sequence<T> *Concat(const Sequence<T> *other) const = 0;
    virtual Sequence<T> *GetSubsequence(int start, int end) const = 0;

};