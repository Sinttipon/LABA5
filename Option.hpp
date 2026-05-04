#pragma once
#include <stdexcept>
#include <string>

template <typename T>
class Option
{
private:
    T value;
    bool hasValue;

public:
    Option() : value{}, hasValue(false) {}

    explicit Option(const T &val) : value(val), hasValue(true) {}

    static Option<T> None() { return Option<T>(); }

    bool IsSome() const noexcept { return hasValue; }
    bool IsNone() const noexcept { return !hasValue; }

    T Get() const
    {
        if (!hasValue)
            throw std::runtime_error("Option::Get: значение отсутствует (None)");
        return value;
    }

    T GetOrDefault(const T &defaultVal) const
    {
        return hasValue ? value : defaultVal;
    }

    bool operator==(const Option<T> &other) const
    {
        if (hasValue != other.hasValue)
            return false;
        if (!hasValue)
            return true;
        return value == other.value;
    }

    bool operator!=(const Option<T> &other) const
    {
        return !(*this == other);
    }
};