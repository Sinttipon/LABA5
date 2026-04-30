#pragma once
#include "exceptions.hpp"
#include <cstddef>

template <typename T>
class LinkedList
{
private:
    struct Node
    {
        T value;
        Node *next;
        explicit Node(const T &val) : value{val}, next{nullptr} {}
    };

    Node *head;
    Node *tail;

    void Clear()
    {
        while (head != nullptr)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

public:
    class Iterator
    {
    private:
        Node *ptr;

    public:
        explicit Iterator(Node *p) : ptr{p} {}

        T &operator*() const { return ptr->value; }

        Iterator &operator++()
        {
            if (ptr)
                ptr = ptr->next;
            return *this;
        }

        bool operator!=(const Iterator &other) const
        {
            return ptr != other.ptr;
        }

        bool operator==(const Iterator &other) const
        {
            return ptr == other.ptr;
        }
    };

    LinkedList() : head{nullptr}, tail{nullptr} {}
    explicit LinkedList(const T *items, size_t count) : head{nullptr}, tail{nullptr}
    {
        for (size_t i = 0; i < count; ++i)
        {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList &other) : head{nullptr}, tail{nullptr}
    {
        Node *current = other.head;
        while (current != nullptr)
        {
            Append(current->value);
            current = current->next;
        }
    }

    ~LinkedList()
    {
        Clear();
    }

    LinkedList &operator=(const LinkedList &other)
    {
        if (this != &other)
        {
            Clear();
            Node *current = other.head;
            while (current != nullptr)
            {
                Append(current->value);
                current = current->next;
            }
        }
        return *this;
    }

    T GetFirst() const
    {
        if (head == nullptr)
            throw IndexOutOfRange(-1, 0, "GetFirst");
        return head->value;
    }

    T GetLast() const
    {
        if (tail == nullptr)
            throw IndexOutOfRange(-1, 0, "GetLast");
        return tail->value;
    }

    T Get(size_t index) const
    {
        Node *current = head;
        for (size_t i = 0; i < index; ++i)
        {
            if (current == nullptr)
            {
                throw IndexOutOfRange(static_cast<int>(index), static_cast<int>(GetLength()), "Get");
            }
            current = current->next;
        }
        if (current == nullptr)
        {
            throw IndexOutOfRange(static_cast<int>(index), static_cast<int>(GetLength()), "Get");
        }
        return current->value;
    }

    T &operator[](size_t index)
    {
        Node *current = head;
        for (size_t i = 0; i < index; ++i)
        {
            if (current == nullptr)
                throw IndexOutOfRange(static_cast<int>(index), static_cast<int>(GetLength()), "operator[]");
            current = current->next;
        }
        if (current == nullptr)
            throw IndexOutOfRange(static_cast<int>(index), static_cast<int>(GetLength()), "operator[]");
        return current->value;
    }

    const T &operator[](size_t index) const
    {
        Node *current = head;
        for (size_t i = 0; i < index; ++i)
        {
            if (current == nullptr)
                throw IndexOutOfRange(static_cast<int>(index), static_cast<int>(GetLength()), "operator[] const");
            current = current->next;
        }
        if (current == nullptr)
            throw IndexOutOfRange(static_cast<int>(index), static_cast<int>(GetLength()), "operator[] const");
        return current->value;
    }

    size_t GetLength() const
    {
        size_t count = 0;
        Node *current = head;
        while (current != nullptr)
        {
            ++count;
            current = current->next;
        }
        return count;
    }

    void Append(const T &item)
    {
        Node *newNode = new Node{item};
        if (tail == nullptr)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void Prepend(const T &item)
    {
        Node *newNode = new Node{item};
        if (head == nullptr)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
    }

    void InsertAt(const T &item, size_t index)
    {
        size_t len = GetLength();
        if (index > len)
        {
            throw IndexOutOfRange(static_cast<int>(index), static_cast<int>(len) + 1, "InsertAt");
        }

        if (index == 0)
        {
            Prepend(item);
            return;
        }

        Node *current = head;
        for (size_t i = 0; i < index - 1; ++i)
        {
            current = current->next;
        }

        Node *newNode = new Node{item};
        newNode->next = current->next;
        current->next = newNode;

        if (newNode->next == nullptr)
        {
            tail = newNode;
        }
    }

    LinkedList *GetSubList(size_t startIndex, size_t endIndex) const
    {
        size_t len = GetLength();
        if (startIndex > endIndex || endIndex >= len)
        {
            throw IndexOutOfRange(static_cast<int>(startIndex), static_cast<int>(len), "GetSubList");
        }

        LinkedList *result = new LinkedList();
        Node *current = head;
        for (size_t i = 0; i <= endIndex; ++i)
        {
            if (i >= startIndex)
            {
                result->Append(current->value);
            }
            current = current->next;
        }
        return result;
    }

    LinkedList *Concat(const LinkedList *other) const
    {
        if (other == nullptr)
            return new LinkedList(*this);

        LinkedList *result = new LinkedList(*this);
        Node *current = other->head;
        while (current != nullptr)
        {
            result->Append(current->value);
            current = current->next;
        }
        return result;
    }

    Iterator begin()
    {
        return Iterator(head);
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

    const Iterator begin() const
    {
        return Iterator(head);
    }

    const Iterator end() const
    {
        return Iterator(nullptr);
    }
};