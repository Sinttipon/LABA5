#pragma once
#include <functional>
#include <string>
#include "exceptions.hpp"
#include "Option.hpp"

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

    virtual Sequence<T> *Where(std::function<bool(const T &)> predicate) const = 0;

    Option<T> TryGetFirst() const
    {
        try
        {
            return Option<T>(GetFirst());
        }
        catch (const SequenceException &)
        {
            return Option<T>::None();
        }
    }

    Option<T> TryGetLast() const
    {
        try
        {
            return Option<T>(GetLast());
        }
        catch (const SequenceException &)
        {
            return Option<T>::None();
        }
    }

    Option<T> TryGet(size_t index) const
    {
        try
        {
            return Option<T>(Get(index));
        }
        catch (const SequenceException &)
        {
            return Option<T>::None();
        }
    }

    Option<T> TryFind(std::function<bool(const T &)> predicate) const
    {
        for (size_t i = 0; i < GetLength(); ++i)
        {
            T val = Get(i);
            if (predicate(val))
                return Option<T>(val);
        }
        return Option<T>::None();
    }
};