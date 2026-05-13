#pragma once
#include <stdexcept>
#include <string>
#include <new>

template <typename T>
class Option
{
private:
    alignas(T) unsigned char storage[sizeof(T)];
    bool hasValue;

    T *ptr() { return reinterpret_cast<T *>(storage); }
    const T *ptr() const { return reinterpret_cast<const T *>(storage); }

    void destroy()
    {
        if (hasValue)
        {
            ptr()->~T();
            hasValue = false;
        }
    }

public:
    Option() : hasValue(false) {}

    Option(const T &val) : hasValue(true)
    {
        new (storage) T(val);
    }

    Option(const Option<T> &other) : hasValue(other.hasValue)
    {
        if (other.hasValue)
            new (storage) T(*other.ptr());
    }

    Option(Option<T> &&other) noexcept : hasValue(other.hasValue)
    {
        if (other.hasValue)
        {
            new (storage) T(std::move(*other.ptr()));
            other.destroy();
        }
    }

    ~Option()
    {
        destroy();
    }

    Option &operator=(const Option<T> &other)
    {
        if (this != &other)
        {
            destroy();
            hasValue = other.hasValue;
            if (other.hasValue)
                new (storage) T(*other.ptr());
        }
        return *this;
    }

    Option &operator=(Option<T> &&other) noexcept
    {
        if (this != &other)
        {
            destroy();
            hasValue = other.hasValue;
            if (other.hasValue)
            {
                new (storage) T(std::move(*other.ptr()));
                other.destroy();
            }
        }
        return *this;
    }

    static Option<T> None() { return Option<T>(); }

    bool IsSome() const noexcept { return hasValue; }
    bool IsNone() const noexcept { return !hasValue; }

    T Get() const
    {
        if (!hasValue)
            throw std::runtime_error("Option::Get: значение отсутствует (None)");
        return *ptr();
    }

    T GetOrDefault(const T &defaultVal) const
    {
        return hasValue ? *ptr() : defaultVal;
    }

    bool operator==(const Option<T> &other) const
    {
        if (hasValue != other.hasValue)
            return false;
        if (!hasValue)
            return true;
        return *ptr() == *other.ptr();
    }

    bool operator!=(const Option<T> &other) const
    {
        return !(*this == other);
    }
};