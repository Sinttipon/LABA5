#pragma once
#include <stdexcept>
#include <string>

class DataStructureException : public std::runtime_error
{
public:
    explicit DataStructureException(const std::string &message)
        : std::runtime_error(message) {}
};

class IndexOutOfRange : public DataStructureException
{
public:
    IndexOutOfRange(size_t index, size_t size, const std::string &context)
        : DataStructureException(buildMessage(index, size, context)),
          index(index), size(size) {}

    size_t getIndex() const noexcept { return index; }
    size_t getSize() const noexcept { return size; }

private:
    static std::string buildMessage(size_t index, size_t size, const std::string &context)
    {
        if (size == 0)
        {
            return "IndexOutOfRange in " + context + ": index " +
                   std::to_string(index) + " is out of bounds for empty sequence";
        }
        return "IndexOutOfRange in " + context + ": index " +
               std::to_string(index) + " is out of bounds [0, " +
               std::to_string(size - 1) + "]";
    }

    size_t index = 0;
    size_t size = 0;
};