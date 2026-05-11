#pragma once
#include <string>

class MYException
{
public:
    virtual ~MYException() = default;
    virtual std::string whatErr() const = 0;
};

class DataStructureException : public MYException
{
protected:
    std::string message;

public:
    DataStructureException(const std::string &msg) : message(msg) {}
    std::string whatErr() const override { return message; }
};

class SequenceException : public DataStructureException
{
public:
    SequenceException(const std::string &msg) : DataStructureException(msg) {}
};

class IndexOutOfRange : public SequenceException
{
private:
    size_t index;
    size_t size;

    static std::string buildMessage(size_t index, size_t size, const std::string &context)
    {
        if (size == 0)
            return "IndexOutOfRange в " + context + ": индекс " +
                   std::to_string(index) + " за пределами ";
        return "IndexOutOfRange в " + context + ": индекс " +
               std::to_string(index) + " за пределами [0, " +
               std::to_string(size - 1) + "]";
    }

public:
    IndexOutOfRange(size_t index, size_t size, const std::string &context)
        : SequenceException(buildMessage(index, size, context)),
          index(index), size(size) {}

    size_t getIndex() const noexcept { return index; }
    size_t getSize() const noexcept { return size; }
};

class TestFailedException : public MYException
{
private:
    std::string message;

public:
    TestFailedException(const std::string &msg) : message(msg) {}
    std::string whatErr() const override { return message; }
};