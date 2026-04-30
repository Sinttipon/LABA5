#pragma once
#include <functional>
#include "exceptions.hpp"

template <typename T>
class Sequence
{
public:
    virtual ~Sequence() = default;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_t index) const = 0;
    virtual size_t GetLength() const = 0;

    virtual Sequence<T> *GetSubsequence(size_t startIndex, size_t endIndex) const = 0;

    virtual Sequence<T> *Append(const T &item) const = 0;
    virtual Sequence<T> *Prepend(const T &item) const = 0;
    virtual Sequence<T> *InsertAt(const T &item, size_t index) const = 0;
    virtual Sequence<T> *Concat(const Sequence<T> *other) const = 0;

    template <typename TOut>
    virtual Sequence<TOut> *Map(std::function<TOut(const T &)> func) const = 0;

    virtual Sequence<T> *Where(std::function<bool(const T &)> predicate) const = 0;

    template <typename TAcc>
    virtual TAcc Reduce(std::function<TAcc(const TAcc &, const T &)> func, const TAcc &init) const = 0;

protected:
    virtual Sequence<T> *Instance() const = 0;
    virtual Sequence<T> *Clone() const = 0;
};