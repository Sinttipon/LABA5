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
    
    IndexOutOfRange(int index, int size, const std::string &context)
        : DataStructureException(buildMessage(index, size, context)),
          index(index), size(size) {}
   
    int getIndex() const noexcept { return index; }
    int getSize() const noexcept { return size; }

private:
    static std::string buildMessage(int index, int size, const std::string &context)
    {
    }

    int index = 0;
    int size = 0;
};