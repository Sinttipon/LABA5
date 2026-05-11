#pragma once
#include "Sequence.h"
#include "LinkedList.hpp"
#include <iostream>

template <typename T>
class MutableListSequence;
template <typename T>
class ImmutableListSequence;

template <typename T>
class BaseListSequence : public Sequence<T>
{
protected:
    LinkedList<T> items;

    BaseListSequence() {}
    BaseListSequence(T *itemsArr, size_t count) : items(itemsArr, count) {}
    BaseListSequence(const LinkedList<T> &list) : items(list) {}
    BaseListSequence(const BaseListSequence<T> &other) : items(other.items) {}

public:
    T GetFirst() const override
    {
        if (items.GetLength() == 0)
            throw IndexOutOfRange(0, 0, "GetFirst в пустом ListSequence");
        return items.GetFirst();
    }

    T GetLast() const override
    {
        if (items.GetLength() == 0)
            throw IndexOutOfRange(0, 0, "GetLast в пустом ListSequence");
        return items.GetLast();
    }

    T Get(size_t index) const override { return items.Get(index); }
    size_t GetLength() const override { return items.GetLength(); }

    Sequence<T> *GetSubsequence(size_t startIndex, size_t endIndex) const override
    {
        size_t len = items.GetLength();
        if (startIndex > endIndex || endIndex >= len)
            throw IndexOutOfRange(startIndex, len, "ListSequence::GetSubsequence");

        LinkedList<T> *subList = items.GetSubList(startIndex, endIndex);
        Sequence<T> *result;
        if (dynamic_cast<const MutableListSequence<T> *>(this))
            result = new MutableListSequence<T>(*subList);
        else
            result = new ImmutableListSequence<T>(*subList);
        delete subList;
        return result;
    }

    Sequence<T> *Append(const T &item) const override
    {
        return static_cast<BaseListSequence<T> *>(Instance())->AppendInternal(item);
    }

    Sequence<T> *Prepend(const T &item) const override
    {
        return static_cast<BaseListSequence<T> *>(Instance())->PrependInternal(item);
    }

    Sequence<T> *InsertAt(const T &item, size_t index) const override
    {
        return static_cast<BaseListSequence<T> *>(Instance())->InsertAtInternal(item, index);
    }

    Sequence<T> *Concat(const Sequence<T> *other) const override
    {
        return static_cast<BaseListSequence<T> *>(Instance())->ConcatInternal(other);
    }

    Sequence<T> *Where(std::function<bool(const T &)> predicate) const override
    {
        LinkedList<T> temp;
        for (size_t i = 0; i < GetLength(); ++i)
        {
            T val = Get(i);
            if (predicate(val))
                temp.Append(val);
        }
        Sequence<T> *result;
        if (dynamic_cast<const MutableListSequence<T> *>(this))
            result = new MutableListSequence<T>(temp);
        else
            result = new ImmutableListSequence<T>(temp);
        return result;
    }

    Sequence<Sequence<T> *> *Split(std::function<bool(const T &)> isSeparator) const
    {
        MutableArraySequence<Sequence<T> *> *result = new MutableArraySequence<Sequence<T> *>();
        MutableListSequence<T> *current = new MutableListSequence<T>();

        for (size_t i = 0; i < GetLength(); ++i)
        {
            T val = Get(i);
            if (isSeparator(val))
            {
                if (current->GetLength() > 0)
                {
                    T *buf = new T[current->GetLength()];
                    for (size_t j = 0; j < current->GetLength(); ++j)
                        buf[j] = current->Get(j);
                    ImmutableListSequence<T> *part = new ImmutableListSequence<T>(buf, current->GetLength());
                    delete[] buf;
                    Sequence<Sequence<T> *> *tmpRes = result->Append(part);
                    delete result;
                    result = static_cast<MutableArraySequence<Sequence<T> *> *>(tmpRes);
                    delete current;
                    current = new MutableListSequence<T>();
                }
            }
            else
            {
                Sequence<T> *tmp = current->Append(val);
                delete current;
                current = static_cast<MutableListSequence<T> *>(tmp);
            }
        }
        if (current->GetLength() > 0)
        {
            T *buf = new T[current->GetLength()];
            for (size_t j = 0; j < current->GetLength(); ++j)
                buf[j] = current->Get(j);
            ImmutableListSequence<T> *part = new ImmutableListSequence<T>(buf, current->GetLength());
            delete[] buf;
            Sequence<Sequence<T> *> *tmpRes = result->Append(part);
            delete result;
            result = static_cast<MutableArraySequence<Sequence<T> *> *>(tmpRes);
        }
        else
            delete current;
        return result;
    }

    Sequence<T> *Slice(int index, size_t count, const Sequence<T> *s = nullptr) const
    {
        size_t len = GetLength();
        int realIndex = index;
        if (realIndex < 0)
            realIndex = static_cast<int>(len) + realIndex;
        if (realIndex < 0 || static_cast<size_t>(realIndex) > len)
            throw IndexOutOfRange(realIndex, len, "Slice: индекс за границами");

        size_t idx = static_cast<size_t>(realIndex);
        Sequence<T> *result = static_cast<Sequence<T> *>(new MutableListSequence<T>());

        for (size_t i = 0; i < idx; ++i)
            result = result->Append(Get(i));

        if (s)
            for (size_t i = 0; i < s->GetLength(); ++i)
                result = result->Append(s->Get(i));

        for (size_t i = idx + count; i < len; ++i)
            result = result->Append(Get(i));

        return result;
    }

    template <typename TOut>
    Sequence<TOut> *Map(std::function<TOut(const T &)> func) const
    {
        size_t len = GetLength();
        TOut *newArr = new TOut[len];
        for (size_t i = 0; i < len; ++i)
            newArr[i] = func(Get(i));
        Sequence<TOut> *res;
        if (dynamic_cast<const MutableListSequence<T> *>(this))
            res = new MutableListSequence<TOut>(newArr, len);
        else
            res = new ImmutableListSequence<TOut>(newArr, len);
        delete[] newArr;
        return res;
    }

    template <typename TAcc>
    TAcc Reduce(std::function<TAcc(const TAcc &, const T &)> func, const TAcc &init) const
    {
        TAcc acc = init;
        for (size_t i = 0; i < GetLength(); ++i)
            acc = func(acc, Get(i));
        return acc;
    }

protected:
    BaseListSequence<T> *AppendInternal(const T &item)
    {
        items.Append(item);
        return this;
    }

    BaseListSequence<T> *PrependInternal(const T &item)
    {
        items.Prepend(item);
        return this;
    }

    BaseListSequence<T> *InsertAtInternal(const T &item, size_t index)
    {
        size_t len = items.GetLength();
        if (index > len)
            throw IndexOutOfRange(index, len + 1, "ListSequence::InsertAt");
        items.InsertAt(item, index);
        return this;
    }

    BaseListSequence<T> *ConcatInternal(const Sequence<T> *other)
    {
        if (!other)
            return this;
        for (size_t i = 0; i < other->GetLength(); ++i)
            items.Append(other->Get(i));
        return this;
    }

    virtual Sequence<T> *Instance() const = 0;
    virtual Sequence<T> *Clone() const = 0;
};

template <typename T>
class MutableListSequence : public BaseListSequence<T>
{
public:
    MutableListSequence() : BaseListSequence<T>() {}
    MutableListSequence(T *itemsArr, size_t count) : BaseListSequence<T>(itemsArr, count) {}
    MutableListSequence(const LinkedList<T> &list) : BaseListSequence<T>(list) {}
    MutableListSequence(const MutableListSequence<T> &other) : BaseListSequence<T>(other) {}

protected:
    Sequence<T> *Instance() const override
    {
        return const_cast<MutableListSequence<T> *>(this);
    }
    Sequence<T> *Clone() const override
    {
        return new MutableListSequence<T>(*this);
    }
};

template <typename T>
class ImmutableListSequence : public BaseListSequence<T>
{
public:
    ImmutableListSequence() : BaseListSequence<T>() {}
    ImmutableListSequence(T *itemsArr, size_t count) : BaseListSequence<T>(itemsArr, count) {}
    ImmutableListSequence(const LinkedList<T> &list) : BaseListSequence<T>(list) {}
    ImmutableListSequence(const ImmutableListSequence<T> &other) : BaseListSequence<T>(other) {}

protected:
    Sequence<T> *Instance() const override
    {
        return this->Clone();
    }
    Sequence<T> *Clone() const override
    {
        return new ImmutableListSequence<T>(*this);
    }
};