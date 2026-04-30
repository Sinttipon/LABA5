#pragma once
#include "Sequence.h"
#include "DynamicArray.hpp"
#include <iostream>

template <typename T>
class BaseArraySequence : public Sequence<T>
{
protected:
    DynamicArray<T> *items;

    BaseArraySequence() : items(new DynamicArray<T>()) {}
    BaseArraySequence(T *itemsArr, size_t count) : items(new DynamicArray<T>(itemsArr, count)) {}
    BaseArraySequence(const DynamicArray<T> &arr) : items(new DynamicArray<T>(arr)) {}
    BaseArraySequence(const BaseArraySequence<T> &other) : items(new DynamicArray<T>(*other.items)) {}

    ~BaseArraySequence() override
    {
        delete items;
    }

    T GetFirst() const override
    {
        if (items->GetSize() == 0)
            throw IndexOutOfRange(0, 0, "GetFirst on empty ArraySequence");
        return items->Get(0);
    }

    T GetLast() const override
    {
        if (items->GetSize() == 0)
            throw IndexOutOfRange(0, 0, "GetLast on empty ArraySequence");
        return items->Get(items->GetSize() - 1);
    }

    T Get(size_t index) const override { return items->Get(index); }
    size_t GetLength() const override { return items->GetSize(); }

    Sequence<T> *GetSubsequence(size_t startIndex, size_t endIndex) const override
    {
        if (startIndex > endIndex || endIndex >= items->GetSize())
        {
            throw IndexOutOfRange(startIndex, items->GetSize(), "ArraySequence::GetSubsequence");
        }

        size_t subSize = endIndex - startIndex + 1;
        T *subArr = new T[subSize];
        for (size_t i = 0; i < subSize; ++i)
        {
            subArr[i] = items->Get(startIndex + i);
        }

        Sequence<T> *result;

        if (dynamic_cast<const MutableArraySequence<T> *>(this))
        {
            result = new MutableArraySequence<T>(subArr, subSize);
        }
        else
        {
            result = new ImmutableArraySequence<T>(subArr, subSize);
        }

        delete[] subArr;
        return result;
    }

    BaseArraySequence<T> *AppendInternal(const T &item)
    {
        items->Resize(items->GetSize() + 1);
        items->Set(items->GetSize() - 1, item);
        return this;
    }

    BaseArraySequence<T> *PrependInternal(const T &item)
    {
        items->Resize(items->GetSize() + 1);
        for (size_t i = items->GetSize() - 1; i > 0; --i)
            items->Set(i, items->Get(i - 1));
        items->Set(0, item);
        return this;
    }

    BaseArraySequence<T> *InsertAtInternal(const T &item, size_t index)
    {
        if (index > items->GetSize())
            throw IndexOutOfRange(index, items->GetSize() + 1, "ArraySequence::InsertAt");
        items->Resize(items->GetSize() + 1);
        for (size_t i = items->GetSize() - 1; i > index; --i)
            items->Set(i, items->Get(i - 1));
        items->Set(index, item);
        return this;
    }

    BaseArraySequence<T> *ConcatInternal(const Sequence<T> *other)
    {
        if (!other)
            return this;
        size_t otherLen = other->GetLength();
        size_t startLen = items->GetSize();
        items->Resize(startLen + otherLen);
        for (size_t i = 0; i < otherLen; ++i)
            items->Set(startLen + i, other->Get(i));
        return this;
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

    template <typename TOut>
    Sequence<TOut> *Map(std::function<TOut(const T &)> func) const override
    {
        size_t len = GetLength();
        TOut *newArr = new TOut[len];
        for (size_t i = 0; i < len; ++i)
            newArr[i] = func(Get(i));

        Sequence<TOut> *res;
        if (dynamic_cast<const MutableArraySequence<T> *>(this))
        {
            res = new MutableArraySequence<TOut>(newArr, len);
        }
        else
        {
            res = new ImmutableArraySequence<TOut>(newArr, len);
        }

        delete[] newArr;
        return res;
    }

    Sequence<T> *Where(std::function<bool(const T &)> predicate) const override
    {
        DynamicArray<T> *temp = new DynamicArray<T>();
        for (size_t i = 0; i < GetLength(); ++i)
        {
            if (predicate(Get(i)))
            {
                temp->Resize(temp->GetSize() + 1);
                temp->Set(temp->GetSize() - 1, Get(i));
            }
        }

        Sequence<T> *result;

        if (dynamic_cast<const MutableArraySequence<T> *>(this))
        {
            result = new MutableArraySequence<T>(*temp);
        }
        else
        {
            result = new ImmutableArraySequence<T>(*temp);
        }

        delete temp;
        return result;
    }

    template <typename TAcc>
    TAcc Reduce(std::function<TAcc(const TAcc &, const T &)> func, const TAcc &init) const override
    {
        TAcc acc = init;
        for (size_t i = 0; i < GetLength(); ++i)
            acc = func(acc, Get(i));
        return acc;
    }

protected:
    virtual Sequence<T> *Instance() const override = 0;
    virtual Sequence<T> *Clone() const override = 0;
};

template <typename T>
class MutableArraySequence : public BaseArraySequence<T>
{
public:
    using BaseArraySequence<T>::BaseArraySequence;

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
    using BaseArraySequence<T>::BaseArraySequence;

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