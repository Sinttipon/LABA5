#pragma once
#include "exceptions.hpp"

template <typename T>
class LinkedList
{
private:
    struct Node
    {
        T value;
        Node *next;
        explicit Node(const T &val) : value(val), next(nullptr) {}
    };

    Node *head; 
    Node *tail; 
    int length; 

    void Clear()
    {
        while (head != nullptr)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        length = 0;
    }

    void CopyFrom(const LinkedList<T> &other)
    {
        Node *current = other.head;
        while (current != nullptr)
        {
            Append(current->value);
            current = current->next;
        }
    }

public:
    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    explicit LinkedList(T *items, int count) : head(nullptr), tail(nullptr), length(0)
    {
        if (count < 0)
            throw IndexOutOfRange("Negative count in LinkedList");
        for (int i = 0; i < count; ++i)
        {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T> &other) : head(nullptr), tail(nullptr), length(0)
    {
        CopyFrom(other);
    }

    ~LinkedList()
    {
        Clear();
    }

    LinkedList<T> &operator=(const LinkedList<T> &other)
    {
        if (this == &other)
            return *this;
        Clear();
        CopyFrom(other);
        return *this;
    }


    T GetFirst()
    {
        if (head == nullptr)
            throw IndexOutOfRange("GetFirst called on empty list");
        return head->value;
    }

    T GetLast()
    {
        if (tail == nullptr)
            throw IndexOutOfRange("GetLast called on empty list");
        return tail->value;
    }

    T Get(int index)
    {
        if (index < 0 || index >= length)
            throw IndexOutOfRange(index, length, "LinkedList::Get");
        Node *current = head;
        for (int i = 0; i < index; ++i)
            current = current->next;
        return current->value;
    }

    LinkedList<T> *GetSubList(int startIndex, int endIndex)
    {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex)
            throw IndexOutOfRange("Invalid range in GetSubList");

        LinkedList<T> *result = new LinkedList<T>();
        Node *current = head;
        for (int i = 0; i <= endIndex; ++i)
        {
            if (i >= startIndex)
                result->Append(current->value);
            current = current->next;
        }
        return result;
    }

    int GetLength()
    {
        return length;
    }


    void Append(T item)
    {
        Node *newNode = new Node(item);
        if (tail == nullptr)
            head = tail = newNode;
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        ++length;
    }

    void Prepend(T item)
    {
        Node *newNode = new Node(item);
        if (head == nullptr)
            head = tail = newNode;
        else
        {
            newNode->next = head;
            head = newNode;
        }
        ++length;
    }

    void InsertAt(T item, int index)
    {
        if (index < 0 || index > length)
            throw IndexOutOfRange(index, length + 1, "LinkedList::InsertAt");
        if (index == 0)
        {
            Prepend(item);
            return;
        }
        if (index == length)
        {
            Append(item);
            return;
        }

        Node *newNode = new Node(item);
        Node *current = head;
        for (int i = 0; i < index - 1; ++i)
            current = current->next;
        newNode->next = current->next;
        current->next = newNode;
        ++length;
    }

    LinkedList<T> *Concat(LinkedList<T> *other)
    {
        if (other == nullptr)
            return new LinkedList<T>(*this);

        LinkedList<T> *result = new LinkedList<T>(*this);
        Node *current = other->head;
        while (current != nullptr)
        {
            result->Append(current->value);
            current = current->next;
        }
        return result;
    }
};