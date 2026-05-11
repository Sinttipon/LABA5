#pragma once
#include "Sequence.h"
#include "DynamicArray.hpp"
#include <iostream>

template <typename T>
class MutableArraySequence;
template <typename T>
class ImmutableArraySequence;

template <typename T>
class BaseArraySequence : public Sequence<T>
{
protected:
    DynamicArray<T> items;

    BaseArraySequence() {}
    BaseArraySequence(T *itemsArr, size_t count) : items(itemsArr, count) {}
    BaseArraySequence(const DynamicArray<T> &arr) : items(arr) {}
    BaseArraySequence(const BaseArraySequence<T> &other) : items(other.items) {}

public:
    T GetFirst() const override
    {
        if (items.GetSize() == 0)
            throw IndexOutOfRange(0, 0, "GetFirst в пустом ArraySequence");
        return items.Get(0);
    }

    T GetLast() const override
    {
        if (items.GetSize() == 0)
            throw IndexOutOfRange(0, 0, "GetLast в пустом ArraySequence");
        return items.Get(items.GetSize() - 1);
    }

    T Get(size_t index) const override { return items.Get(index); }
    size_t GetLength() const override { return items.GetSize(); }

    Sequence<T> *GetSubsequence(size_t startIndex, size_t endIndex) const override
    {
        if (startIndex > endIndex || endIndex >= items.GetSize())
            throw IndexOutOfRange(startIndex, items.GetSize(), "ArraySequence::GetSubsequence");

        size_t subSize = endIndex - startIndex + 1;
        T *subArr = new T[subSize];
        for (size_t i = 0; i < subSize; ++i)
            subArr[i] = items.Get(startIndex + i);

        Sequence<T> *result;
        if (dynamic_cast<const MutableArraySequence<T> *>(this))
            result = new MutableArraySequence<T>(subArr, subSize);
        else
            result = new ImmutableArraySequence<T>(subArr, subSize);

        delete[] subArr;
        return result;
    }

    Sequence<T> *Append(const T &item) const override
    {
        return static_cast<BaseArraySequence<T> *>(Instance())->AppendInternal(item);
    }

    Sequence<T> *Prepend(const T &item) const override
    {
        return static_cast<BaseArraySequence<T> *>(Instance())->PrependInternal(item);
    }

    Sequence<T> *InsertAt(const T &item, size_t index) const override
    {
        return static_cast<BaseArraySequence<T> *>(Instance())->InsertAtInternal(item, index);
    }

    Sequence<T> *Concat(const Sequence<T> *other) const override
    {
        return static_cast<BaseArraySequence<T> *>(Instance())->ConcatInternal(other);
    }

    Sequence<T> *Where(std::function<bool(const T &)> predicate) const override
    {
        DynamicArray<T> temp;
        for (size_t i = 0; i < GetLength(); ++i)
        {
            if (predicate(Get(i)))
            {
                temp.Resize(temp.GetSize() + 1);
                temp.Set(temp.GetSize() - 1, Get(i));
            }
        }

        Sequence<T> *result;
        if (dynamic_cast<const MutableArraySequence<T> *>(this))
            result = new MutableArraySequence<T>(temp);
        else
            result = new ImmutableArraySequence<T>(temp);

        return result;
    }

    Sequence<Sequence<T> *> *Split(std::function<bool(const T &)> isSeparator) const
    {
        MutableArraySequence<Sequence<T> *> *result = new MutableArraySequence<Sequence<T> *>();
        MutableArraySequence<T> *current = new MutableArraySequence<T>();

        for (size_t i = 0; i < GetLength(); ++i)
        {
            T val = Get(i);
            if (isSeparator(val))
            {
                if (current->GetLength() > 0)
                {
                    result->AppendInternal2(current);
                    current = new MutableArraySequence<T>();
                }
            }
            else
            {
                current->AppendInternal2(val);
            }
        }
        if (current->GetLength() > 0)
            result->AppendInternal2(current);
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
        T *tmp = new T[len + (s ? s->GetLength() : 0)];
        size_t pos = 0;

        for (size_t i = 0; i < idx; ++i)
            tmp[pos++] = Get(i);

        if (s)
            for (size_t i = 0; i < s->GetLength(); ++i)
                tmp[pos++] = s->Get(i);

        for (size_t i = idx + count; i < len; ++i)
            tmp[pos++] = Get(i);

        Sequence<T> *result;
        if (dynamic_cast<const MutableArraySequence<T> *>(this))
            result = new MutableArraySequence<T>(tmp, pos);
        else
            result = new ImmutableArraySequence<T>(tmp, pos);

        delete[] tmp;
        return result;
    }

    void AppendInternal2(const T &item)
    {
        items.Resize(items.GetSize() + 1);
        items.Set(items.GetSize() - 1, item);
    }

    void AppendInternal2(Sequence<T> *seq)
    {
        for (size_t i = 0; i < seq->GetLength(); ++i)
            AppendInternal2(seq->Get(i));
    }

    template <typename TOut>
    Sequence<TOut> *Map(std::function<TOut(const T &)> func) const
    {
        size_t len = GetLength();
        TOut *newArr = new TOut[len];
        for (size_t i = 0; i < len; ++i)
            newArr[i] = func(Get(i));

        Sequence<TOut> *res;
        if (dynamic_cast<const MutableArraySequence<T> *>(this))
            res = new MutableArraySequence<TOut>(newArr, len);
        else
            res = new ImmutableArraySequence<TOut>(newArr, len);

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
    BaseArraySequence<T> *AppendInternal(const T &item)
    {
        items.Resize(items.GetSize() + 1);
        items.Set(items.GetSize() - 1, item);
        return this;
    }

    BaseArraySequence<T> *PrependInternal(const T &item)
    {
        items.Resize(items.GetSize() + 1);
        for (size_t i = items.GetSize() - 1; i > 0; --i)
            items.Set(i, items.Get(i - 1));
        items.Set(0, item);
        return this;
    }

    BaseArraySequence<T> *InsertAtInternal(const T &item, size_t index)
    {
        if (index > items.GetSize())
            throw IndexOutOfRange(index, items.GetSize() + 1, "ArraySequence::InsertAt");
        items.Resize(items.GetSize() + 1);
        for (size_t i = items.GetSize() - 1; i > index; --i)
            items.Set(i, items.Get(i - 1));
        items.Set(index, item);
        return this;
    }

    BaseArraySequence<T> *ConcatInternal(const Sequence<T> *other)
    {
        if (!other)
            return this;
        size_t otherLen = other->GetLength();
        size_t startLen = items.GetSize();
        items.Resize(startLen + otherLen);
        for (size_t i = 0; i < otherLen; ++i)
            items.Set(startLen + i, other->Get(i));
        return this;
    }

    virtual Sequence<T> *Instance() const = 0;
    virtual Sequence<T> *Clone() const = 0;
};

template <typename T>
class MutableArraySequence : public BaseArraySequence<T>
{
public:
    MutableArraySequence() : BaseArraySequence<T>() {}
    MutableArraySequence(T *itemsArr, size_t count) : BaseArraySequence<T>(itemsArr, count) {}
    MutableArraySequence(const DynamicArray<T> &arr) : BaseArraySequence<T>(arr) {}
    MutableArraySequence(const MutableArraySequence<T> &other) : BaseArraySequence<T>(other) {}

protected:
    Sequence<T> *Instance() const override
    {
        return const_cast<MutableArraySequence<T> *>(this);
    }

    Sequence<T> *Clone() const override
    {
        return new MutableArraySequence<T>(*this);
    }
};

template <typename T>
class ImmutableArraySequence : public BaseArraySequence<T>
{
public:
    ImmutableArraySequence() : BaseArraySequence<T>() {}
    ImmutableArraySequence(T *itemsArr, size_t count) : BaseArraySequence<T>(itemsArr, count) {}
    ImmutableArraySequence(const DynamicArray<T> &arr) : BaseArraySequence<T>(arr) {}
    ImmutableArraySequence(const ImmutableArraySequence<T> &other) : BaseArraySequence<T>(other) {}

protected:
    Sequence<T> *Instance() const override
    {
        return this->Clone();
    }

    Sequence<T> *Clone() const override
    {
        return new ImmutableArraySequence<T>(*this);
    }
};