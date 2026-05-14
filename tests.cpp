#include <iostream>

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "BitSequence.h"
#include "ListSequence.h"
#include "Option.hpp"
#include "exceptions.hpp"

static int testsFailed = 0;

#define TEST(name)                                    \
    do                                                \
    {                                                 \
        std::cout << "  Тест: " << name << std::endl; \
        try

#define ENDTEST()                                                       \
    catch (const MYException &e)                                        \
    {                                                                   \
        std::cout << "НЕ ПРОЙДЕН: " << e.whatErr() << std::endl;        \
        ++testsFailed;                                                  \
    }                                                                   \
    catch (const std::exception &e)                                     \
    {                                                                   \
        std::cout << "НЕ ПРОЙДЕН: " << e.what() << std::endl;           \
        ++testsFailed;                                                  \
    }                                                                   \
    catch (...)                                                         \
    {                                                                   \
        std::cout << "НЕ ПРОЙДЕН: неизвестное исключение" << std::endl; \
        ++testsFailed;                                                  \
    }                                                                   \
    }                                                                   \
    while (0)

#define CHECK(cond)                                                    \
    do                                                                 \
    {                                                                  \
        if (!(cond))                                                   \
        {                                                              \
            throw TestFailedException("Проверка не пройдена: " #cond); \
        }                                                              \
    } while (0)

#define CHECK_THROW(expr, exc_type)                                                               \
    do                                                                                            \
    {                                                                                             \
        bool caught = false;                                                                      \
        try                                                                                       \
        {                                                                                         \
            expr;                                                                                 \
        }                                                                                         \
        catch (const exc_type &)                                                                  \
        {                                                                                         \
            caught = true;                                                                        \
        }                                                                                         \
        catch (...)                                                                               \
        {                                                                                         \
        }                                                                                         \
        if (!caught)                                                                              \
        {                                                                                         \
            throw TestFailedException("Ожидалось исключение " #exc_type ", но оно не выброшено"); \
        }                                                                                         \
    } while (0)

void TestDynamicArray_Construction()
{
    std::cout << "\nDynamicArray создание" << std::endl;

    TEST("дефолт")
    {
        DynamicArray<int> arr;
        CHECK(arr.GetSize() == 0);
    }
    ENDTEST();

    TEST("с размером")
    {
        DynamicArray<int> arr(5);
        CHECK(arr.GetSize() == 5);
    }
    ENDTEST();

    TEST("с нулевым размером")
    {
        DynamicArray<int> arr(0);
        CHECK(arr.GetSize() == 0);
    }
    ENDTEST();

    TEST("из массива")
    {
        int src[] = {1, 2, 3, 4, 5};
        DynamicArray<int> arr(src, 5);
        CHECK(arr.GetSize() == 5);
        CHECK(arr.Get(0) == 1);
        CHECK(arr.Get(4) == 5);
    }
    ENDTEST();

    TEST("из пустого массива")
    {
        int *src = nullptr;
        DynamicArray<int> arr(src, 0);
        CHECK(arr.GetSize() == 0);
    }
    ENDTEST();

    TEST("копияя")
    {
        int src[] = {10, 20, 30};
        DynamicArray<int> original(src, 3);
        DynamicArray<int> copy(original);
        CHECK(copy.GetSize() == 3);
        CHECK(copy.Get(0) == 10);
        CHECK(copy.Get(1) == 20);
        CHECK(copy.Get(2) == 30);
        original.Set(0, 999);
        CHECK(copy.Get(0) == 10);
    }
    ENDTEST();
}

void TestDynamicArray_GetSet()
{
    std::cout << "\nDynamicArray Get/Set" << std::endl;

    TEST("Get и Set")
    {
        int src[] = {1, 2, 3};
        DynamicArray<int> arr(src, 3);
        CHECK(arr.Get(0) == 1);
        CHECK(arr.Get(2) == 3);
        arr.Set(1, 42);
        CHECK(arr.Get(1) == 42);
    }
    ENDTEST();

    TEST("Get за границами")
    {
        DynamicArray<int> arr(3);
        CHECK_THROW(arr.Get(5), IndexOutOfRange);
    }
    ENDTEST();

    TEST("Get пустой массив")
    {
        DynamicArray<int> arr;
        CHECK_THROW(arr.Get(0), IndexOutOfRange);
    }
    ENDTEST();

    TEST("Set за границами")
    {
        DynamicArray<int> arr(2);
        CHECK_THROW(arr.Set(5, 10), IndexOutOfRange);
    }
    ENDTEST();
}

void TestDynamicArray_Resize()
{
    std::cout << "\nDynamicArray Resize" << std::endl;

    TEST("уменьшение")
    {
        int src[] = {1, 2, 3, 4, 5};
        DynamicArray<int> arr(src, 5);
        arr.Resize(3);
        CHECK(arr.GetSize() == 3);
        CHECK(arr.Get(0) == 1);
        CHECK(arr.Get(2) == 3);
    }
    ENDTEST();

    TEST("увеличение")
    {
        int src[] = {1, 2, 3};
        DynamicArray<int> arr(src, 3);
        arr.Resize(5);
        CHECK(arr.GetSize() == 5);
        CHECK(arr.Get(0) == 1);
        CHECK(arr.Get(2) == 3);
        arr.Set(4, 99);
        CHECK(arr.Get(4) == 99);
    }
    ENDTEST();

    TEST("до нуля")
    {
        int src[] = {1, 2, 3};
        DynamicArray<int> arr(src, 3);
        arr.Resize(0);
        CHECK(arr.GetSize() == 0);
    }
    ENDTEST();
}

void TestDynamicArray_OperatorBracket()
{
    std::cout << "\nDynamicArray operator[]" << std::endl;

    TEST("чтение")
    {
        int src[] = {10, 20, 30};
        DynamicArray<int> arr(src, 3);
        CHECK(arr[0] == 10);
        CHECK(arr[2] == 30);
    }
    ENDTEST();

    TEST("запись")
    {
        int src[] = {1, 2, 3};
        DynamicArray<int> arr(src, 3);
        arr[1] = 77;
        CHECK(arr[1] == 77);
    }
    ENDTEST();

    TEST("за границами")
    {
        DynamicArray<int> arr(2);
        CHECK_THROW(arr[5] = 10, IndexOutOfRange);
    }
    ENDTEST();
}

void TestDynamicArray_Assignment()
{
    std::cout << "\nDynamicArray присваивание" << std::endl;

    TEST("оператор =")
    {
        int s1[] = {1, 2, 3}, s2[] = {4, 5, 6, 7};
        DynamicArray<int> a(s1, 3), b(s2, 4);
        a = b;
        CHECK(a.GetSize() == 4);
        CHECK(a.Get(0) == 4);
    }
    ENDTEST();

    TEST("самоприсваивание")
    {
        int src[] = {1, 2, 3};
        DynamicArray<int> arr(src, 3);
        arr = arr;
        CHECK(arr.GetSize() == 3);
        CHECK(arr.Get(0) == 1);
    }
    ENDTEST();
}

void TestDynamicArray_Iterators()
{
    std::cout << "\nDynamicArray итераторы" << std::endl;

    TEST("begin/end")
    {
        int src[] = {1, 2, 3};
        DynamicArray<int> arr(src, 3);
        int sum = 0;
        for (auto it = arr.begin(); it != arr.end(); ++it)
        {
            sum += *it;
        }
        CHECK(sum == 6);
    }
    ENDTEST();

    TEST("range")
    {
        int src[] = {10, 20, 30};
        DynamicArray<int> arr(src, 3);
        int sum = 0;
        for (const auto &v : arr)
            sum += v;
        CHECK(sum == 60);
    }
    ENDTEST();
}

void TestLinkedList_Construction()
{
    std::cout << "\nLinkedList создание" << std::endl;

    TEST("дефолт")
    {
        LinkedList<int> l;
        CHECK(l.GetLength() == 0);
    }
    ENDTEST();

    TEST("из массива")
    {
        int src[] = {1, 2, 3, 4, 5};
        LinkedList<int> l(src, 5);
        CHECK(l.GetLength() == 5);
        CHECK(l.GetFirst() == 1);
        CHECK(l.GetLast() == 5);
    }
    ENDTEST();

    TEST("копия")
    {
        int src[] = {10, 20, 30};
        LinkedList<int> a(src, 3);
        LinkedList<int> b(a);
        CHECK(b.GetLength() == 3);
        CHECK(b.GetFirst() == 10);
    }
    ENDTEST();
}

void TestLinkedList_GetOperations()
{
    std::cout << "\nLinkedList Get" << std::endl;

    TEST("GetFirst/GetLast")
    {
        int src[] = {1, 2, 3};
        LinkedList<int> l(src, 3);
        CHECK(l.GetFirst() == 1);
        CHECK(l.GetLast() == 3);
    }
    ENDTEST();

    TEST("по индексу")
    {
        int src[] = {10, 20, 30, 40, 50};
        LinkedList<int> l(src, 5);
        CHECK(l.Get(0) == 10);
        CHECK(l.Get(4) == 50);
    }
    ENDTEST();

    TEST("за границами")
    {
        LinkedList<int> l;
        CHECK_THROW(l.Get(0), IndexOutOfRange);
    }
    ENDTEST();
}

void TestLinkedList_AddOperations()
{
    std::cout << "\nLinkedList добавление" << std::endl;

    TEST("Append")
    {
        LinkedList<int> l;
        l.Append(42);
        CHECK(l.GetLength() == 1);
        CHECK(l.GetFirst() == 42);
    }
    ENDTEST();

    TEST("Prepend")
    {
        LinkedList<int> l;
        l.Prepend(42);
        CHECK(l.GetFirst() == 42);
    }
    ENDTEST();

    TEST("InsertAt")
    {
        int src[] = {1, 3};
        LinkedList<int> l(src, 2);
        l.InsertAt(2, 1);
        CHECK(l.Get(1) == 2);
    }
    ENDTEST();

    TEST("InsertAt за границами")
    {
        int src[] = {1, 2};
        LinkedList<int> l(src, 2);
        CHECK_THROW(l.InsertAt(99, 5), IndexOutOfRange);
    }
    ENDTEST();
}

void TestLinkedList_GetSubList()
{
    std::cout << "\nLinkedList GetSubList" << std::endl;

    TEST("подсписок из середины")
    {
        int src[] = {1, 2, 3, 4, 5};
        LinkedList<int> l(src, 5);
        LinkedList<int> *sub = l.GetSubList(1, 3);
        CHECK(sub->GetLength() == 3);
        CHECK(sub->Get(0) == 2);
        CHECK(sub->Get(2) == 4);
        delete sub;
    }
    ENDTEST();

    TEST("недопустимые индексы")
    {
        int src[] = {1, 2, 3};
        LinkedList<int> l(src, 3);
        CHECK_THROW(l.GetSubList(1, 5), IndexOutOfRange);
    }
    ENDTEST();
}

void TestLinkedList_Concat()
{
    std::cout << "\nLinkedList Concat" << std::endl;

    TEST("два списка")
    {
        int s1[] = {1, 2}, s2[] = {3, 4};
        LinkedList<int> a(s1, 2), b(s2, 2);
        LinkedList<int> *r = a.Concat(&b);
        CHECK(r->GetLength() == 4);
        CHECK(r->Get(0) == 1);
        CHECK(r->Get(3) == 4);
        delete r;
    }
    ENDTEST();

    TEST("с пустым")
    {
        int src[] = {1, 2, 3};
        LinkedList<int> a(src, 3), b;
        LinkedList<int> *r = a.Concat(&b);
        CHECK(r->GetLength() == 3);
        delete r;
    }
    ENDTEST();
}

void TestLinkedList_OperatorBracket()
{
    std::cout << "\nLinkedList operator[]" << std::endl;

    TEST("чтение/запись")
    {
        int src[] = {1, 2, 3};
        LinkedList<int> l(src, 3);
        CHECK(l[0] == 1);
        l[1] = 99;
        CHECK(l[1] == 99);
    }
    ENDTEST();

    TEST("за границами")
    {
        int src[] = {1, 2};
        LinkedList<int> l(src, 2);
        CHECK_THROW(l[5], IndexOutOfRange);
    }
    ENDTEST();
}

void TestLinkedList_Assignment()
{
    std::cout << "\nLinkedList присваивание" << std::endl;

    TEST("оператор =")
    {
        int src[] = {1, 2, 3};
        LinkedList<int> a(src, 3), b;
        b.Append(99);
        b = a;
        CHECK(b.GetLength() == 3);
        CHECK(b.Get(0) == 1);
    }
    ENDTEST();

    TEST("самоприсваивание")
    {
        int src[] = {1, 2, 3};
        LinkedList<int> l(src, 3);
        l = l;
        CHECK(l.GetLength() == 3);
    }
    ENDTEST();
}

void TestLinkedList_Iterator()
{
    std::cout << "\nLinkedList итераторы" << std::endl;

    TEST("обход итератором")
    {
        int src[] = {1, 2, 3};
        LinkedList<int> l(src, 3);
        int sum = 0;
        for (auto it = l.begin(); it != l.end(); ++it)
        {
            sum += *it;
        }
        CHECK(sum == 6);
    }
    ENDTEST();

    TEST("модификация через итератор")
    {
        int src[] = {1, 2, 3};
        LinkedList<int> l(src, 3);
        for (auto it = l.begin(); it != l.end(); ++it)
            *it = *it * 10;
        CHECK(l.Get(0) == 10);
        CHECK(l.Get(2) == 30);
    }
    ENDTEST();
}

void TestBitSequence_GetOperations()
{
    std::cout << "\nBitSequence Get" << std::endl;

    TEST("GetFirst непустой")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        CHECK(bs.GetFirst() == Bit(true));
    }
    ENDTEST();

    TEST("GetFirst пустой")
    {
        BitSequence bs;
        CHECK_THROW(bs.GetFirst(), IndexOutOfRange);
    }
    ENDTEST();

    TEST("GetLast непустой")
    {
        bool src[] = {true, false, false};
        BitSequence bs(src, 3);
        CHECK(bs.GetLast() == Bit(false));
    }
    ENDTEST();

    TEST("GetLast пустой")
    {
        BitSequence bs;
        CHECK_THROW(bs.GetLast(), IndexOutOfRange);
    }
    ENDTEST();

    TEST("Get")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        CHECK(bs.Get(0) == Bit(true));
    }
    ENDTEST();

    TEST("Get за границами")
    {
        bool src[] = {true, false};
        BitSequence bs(src, 2);
        CHECK_THROW(bs.Get(5), IndexOutOfRange);
    }
    ENDTEST();

    TEST("Get пустой")
    {
        BitSequence bs;
        CHECK_THROW(bs.Get(0), IndexOutOfRange);
    }
    ENDTEST();

    TEST("GetLength непустой")
    {
        bool src[] = {true, false, true, true};
        BitSequence bs(src, 4);
        CHECK(bs.GetLength() == 4);
    }
    ENDTEST();

    TEST("GetLength пустой")
    {
        BitSequence bs;
        CHECK(bs.GetLength() == 0);
    }
    ENDTEST();

    TEST("GetFirst и GetLast на единичный")
    {
        bool src[] = {true};
        BitSequence bs(src, 1);
        CHECK(bs.GetFirst() == bs.GetLast());
        CHECK(bs.GetLast() == Bit(true));
    }
    ENDTEST();
}

void TestBitSequence_Subsequence()
{
    TEST("полная последовательность")
    {
        bool src[] = {true, false, true, false};
        BitSequence bs(src, 4);
        Sequence<Bit> *sub = bs.GetSubsequence(0, 3);

        CHECK(sub->GetLength() == 4);
        CHECK(sub->Get(0) == Bit(true));
        CHECK(sub->Get(1) == Bit(false));
        CHECK(sub->Get(2) == Bit(true));
        CHECK(sub->Get(3) == Bit(false));

        delete sub;
    }
    ENDTEST();

    TEST("часть последовательности")
    {
        bool src[] = {true, false, true, false, true};
        BitSequence bs(src, 5);
        Sequence<Bit> *sub = bs.GetSubsequence(1, 3);

        CHECK(sub->GetLength() == 3);
        CHECK(sub->Get(0) == Bit(false));
        CHECK(sub->Get(1) == Bit(true));
        CHECK(sub->Get(2) == Bit(false));

        delete sub;
    }
    ENDTEST();

    TEST("один элемент")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        Sequence<Bit> *sub = bs.GetSubsequence(1, 1);

        CHECK(sub->GetLength() == 1);
        CHECK(sub->Get(0) == Bit(false));

        delete sub;
    }
    ENDTEST();

    TEST("ошибка индексации")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        CHECK_THROW(bs.GetSubsequence(2, 1), IndexOutOfRange);
    }
    ENDTEST();

    TEST("за границами")
    {
        bool src[] = {true, false};
        BitSequence bs(src, 2);
        CHECK_THROW(bs.GetSubsequence(0, 5), IndexOutOfRange);
    }
    ENDTEST();

    TEST("пустая")
    {
        BitSequence bs;
        CHECK_THROW(bs.GetSubsequence(0, 0), IndexOutOfRange);
    }
    ENDTEST();
}

void TestBitSequence_Append()
{
    TEST("добавление в конец")
    {
        bool src[] = {true, false};
        BitSequence bs(src, 2);
        Sequence<Bit> *newBs = bs.Append(Bit(true));

        CHECK(bs.GetLength() == 2);
        CHECK(newBs->GetLength() == 3);
        CHECK(newBs->Get(0) == Bit(true));
        CHECK(newBs->Get(1) == Bit(false));
        CHECK(newBs->Get(2) == Bit(true));

        delete newBs;
    }
    ENDTEST();

    TEST("добавление к пустой")
    {
        BitSequence bs;
        Sequence<Bit> *newBs = bs.Append(Bit(false));

        CHECK(newBs->GetLength() == 1);
        CHECK(newBs->Get(0) == Bit(false));

        delete newBs;
    }
    ENDTEST();
}

void TestBitSequence_Prepend()
{
    TEST("добавление в начало")
    {
        bool src[] = {false, true};
        BitSequence bs(src, 2);
        Sequence<Bit> *newBs = bs.Prepend(Bit(true));

        CHECK(bs.GetLength() == 2);
        CHECK(newBs->GetLength() == 3);
        CHECK(newBs->Get(0) == Bit(true));  
        CHECK(newBs->Get(1) == Bit(false)); 
        CHECK(newBs->Get(2) == Bit(true));  

        delete newBs;
    }
    ENDTEST();

    TEST("добавление к пустой")
    {
        BitSequence bs;
        Sequence<Bit> *newBs = bs.Prepend(Bit(true));

        CHECK(newBs->GetLength() == 1);
        CHECK(newBs->Get(0) == Bit(true));

        delete newBs;
    }
    ENDTEST();
}

void TestBitSequence_InsertAt()
{
    TEST("вставка в середину")
    {
        bool src[] = {true, false, true}; 
        BitSequence bs(src, 3);
        Sequence<Bit> *newBs = bs.InsertAt(Bit(false), 1);

        CHECK(bs.GetLength() == 3); 
        CHECK(newBs->GetLength() == 4);
        CHECK(newBs->Get(0) == Bit(true));
        CHECK(newBs->Get(1) == Bit(false)); 
        CHECK(newBs->Get(2) == Bit(false)); 
        CHECK(newBs->Get(3) == Bit(true));

        delete newBs;
    }
    ENDTEST();

    TEST("вставка в начало")
    {
        bool src[] = {false, true};
        BitSequence bs(src, 2);
        Sequence<Bit> *newBs = bs.InsertAt(Bit(true), 0);

        CHECK(newBs->GetLength() == 3);
        CHECK(newBs->Get(0) == Bit(true));
        CHECK(newBs->Get(1) == Bit(false));
        CHECK(newBs->Get(2) == Bit(true));

        delete newBs;
    }
    ENDTEST();

    TEST("вставка в конец")
    {
        bool src[] = {true, false};
        BitSequence bs(src, 2);
        Sequence<Bit> *newBs = bs.InsertAt(Bit(true), 2);

        CHECK(newBs->GetLength() == 3);
        CHECK(newBs->Get(0) == Bit(true));
        CHECK(newBs->Get(1) == Bit(false));
        CHECK(newBs->Get(2) == Bit(true));

        delete newBs;
    }
    ENDTEST();

    TEST("за границами")
    {
        bool src[] = {true, false};
        BitSequence bs(src, 2);
        CHECK_THROW(bs.InsertAt(Bit(true), 3), IndexOutOfRange);
    }
    ENDTEST();
}

void TestBitSequence_Where()
{
    TEST("Where true")
    {
        bool src[] = {true, false, true, false, true};
        BitSequence bs(src, 5);
        Sequence<Bit> *result = bs.Where([](const Bit &x)
            { return x == Bit(true); });
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == Bit(true));
        CHECK(result->Get(1) == Bit(true));
        CHECK(result->Get(2) == Bit(true));
        delete result;
    }
    ENDTEST();

    TEST("Where false")
    {
        bool src[] = {true, false, true, false};
        BitSequence bs(src, 4);
        Sequence<Bit> *result = bs.Where([](const Bit &x)
            { return x == Bit(false); });
        CHECK(result->GetLength() == 2);
        CHECK(result->Get(0) == Bit(false));
        CHECK(result->Get(1) == Bit(false));
        delete result;
    }
    ENDTEST();

    TEST("нет совпадений")
    {
        bool src[] = {false, false, false};
        BitSequence bs(src, 3);
        Sequence<Bit> *result = bs.Where([](const Bit &x)
            { return x == Bit(true); });
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("все совпадения")
    {
        bool src[] = {true, true, true};
        BitSequence bs(src, 3);
        Sequence<Bit> *result = bs.Where([](const Bit &x)
            { return x == Bit(true); });
        CHECK(result->GetLength() == 3);
        delete result;
    }
    ENDTEST();

    TEST("пустая")
    {
        BitSequence bs;
        Sequence<Bit> *result = bs.Where([](const Bit &x)
            { return x == Bit(true); });
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("не меняет исходную")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        Sequence<Bit> *result = bs.Where([](const Bit &x)
            { return x == Bit(true); });
        CHECK(bs.GetLength() == 3);
        CHECK(bs.Get(0) == Bit(true));
        delete result;
    }
    ENDTEST();
}

void TestBitSequence_Concat()
{
    TEST("дефолт")
    {
        bool s1[] = {true, false};
        bool s2[] = {true, true};
        BitSequence a(s1, 2);
        BitSequence b(s2, 2);
        Sequence<Bit> *result = a.Concat(&b);
        CHECK(a.GetLength() == 2);
        CHECK(result->GetLength() == 4);
        CHECK(result->Get(0) == Bit(true));
        CHECK(result->Get(1) == Bit(false));
        CHECK(result->Get(2) == Bit(true));
        CHECK(result->Get(3) == Bit(true));
        delete result;
    }
    ENDTEST();

    TEST("c пустой")
    {
        bool src[] = {true, false};
        BitSequence a(src, 2);
        BitSequence b;
        Sequence<Bit> *result = a.Concat(&b);
        CHECK(result->GetLength() == 2);
        CHECK(result->Get(0) == Bit(true));
        CHECK(result->Get(1) == Bit(false));
        delete result;
    }
    ENDTEST();

    TEST("пустая c непустой")
    {
        BitSequence a;
        bool src[] = {true, false};
        BitSequence b(src, 2);
        Sequence<Bit> *result = a.Concat(&b);
        CHECK(result->GetLength() == 2);
        CHECK(result->Get(0) == Bit(true));
        CHECK(result->Get(1) == Bit(false));
        delete result;
    }
    ENDTEST();

    TEST("две пустых")
    {
        BitSequence a, b;
        Sequence<Bit> *result = a.Concat(&b);
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("с nullptr")
    {
        bool src[] = {true};
        BitSequence a(src, 1);
        Sequence<Bit> *result = a.Concat(nullptr);
        CHECK(result->GetLength() == 1);
        CHECK(result->Get(0) == Bit(true));
        delete result;
    }
    ENDTEST();

    TEST("не меняет исходные")
    {
        bool s1[] = {true};
        bool s2[] = {false, true};
        BitSequence a(s1, 1);
        BitSequence b(s2, 2);
        Sequence<Bit> *result = a.Concat(&b);
        CHECK(a.GetLength() == 1);
        CHECK(b.GetLength() == 2);
        delete result;
    }
    ENDTEST();
}

void TestBitSequence_Map()
{
    TEST("Map в Bit инверсия")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        Sequence<Bit> *result = bs.Map<Bit>([](const Bit &x)
            { return ~x; });
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == Bit(false));
        CHECK(result->Get(1) == Bit(true));
        CHECK(result->Get(2) == Bit(false));
        delete result;
    }
    ENDTEST();

    TEST("Map в Bit всё в true")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        Sequence<Bit> *result = bs.Map<Bit>([](const Bit &x)
            { return Bit(true); });
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == Bit(true));
        CHECK(result->Get(1) == Bit(true));
        CHECK(result->Get(2) == Bit(true));
        delete result;
    }
    ENDTEST();

    TEST("Map в int 1 и 0")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        Sequence<int> *result = bs.Map<int>([](const Bit &x)
            { return x == Bit(true) ? 1 : 0; });
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 0);
        CHECK(result->Get(2) == 1);
        delete result;
    }
    ENDTEST();

    TEST("Map в string")
    {
        bool src[] = {true, false};
        BitSequence bs(src, 2);
        Sequence<std::string> *result = bs.Map<std::string>([](const Bit &x)
            { return x == Bit(true) ? "yes" : "no"; });
        CHECK(result->GetLength() == 2);
        CHECK(result->Get(0) == "yes");
        CHECK(result->Get(1) == "no");
        delete result;
    }
    ENDTEST();

    TEST("Map пустая в Bit")
    {
        BitSequence bs;
        Sequence<Bit> *result = bs.Map<Bit>([](const Bit &x)
            { return ~x; });
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("Map пустая в int")
    {
        BitSequence bs;
        Sequence<int> *result = bs.Map<int>([](const Bit &x)
            { return 42; });
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("Map не меняет исходную")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        Sequence<Bit> *result = bs.Map<Bit>([](const Bit &x)
            { return ~x; });
        CHECK(bs.GetLength() == 3);
        CHECK(bs.Get(0) == Bit(true));
        CHECK(bs.Get(1) == Bit(false));
        CHECK(bs.Get(2) == Bit(true));
        delete result;
    }
    ENDTEST();

    TEST("Map один элемент")
    {
        bool src[] = {true};
        BitSequence bs(src, 1);
        Sequence<int> *result = bs.Map<int>([](const Bit &x)
            { return 99; });
        CHECK(result->GetLength() == 1);
        CHECK(result->Get(0) == 99);
        delete result;
    }
    ENDTEST();
}

void TestBitSequence_Reduce()
{
    TEST("AND все true")
    {
        bool src[] = {true, true, true};
        BitSequence bs(src, 3);
        Bit result = bs.Reduce<Bit>([](const Bit &a, const Bit &x)
            { return a & x; }, Bit(true));
        CHECK(result == Bit(true));
    }
    ENDTEST();

    TEST("AND есть false")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        Bit result = bs.Reduce<Bit>([](const Bit &a, const Bit &x)
            { return a & x; }, Bit(true));
        CHECK(result == Bit(false));
    }
    ENDTEST();

    TEST("OR есть true")
    {
        bool src[] = {false, false, true};
        BitSequence bs(src, 3);
        Bit result = bs.Reduce<Bit>([](const Bit &a, const Bit &x)
            { return a | x; }, Bit(false));
        CHECK(result == Bit(true));
    }
    ENDTEST();

    TEST("OR все false")
    {
        bool src[] = {false, false, false};
        BitSequence bs(src, 3);
        Bit result = bs.Reduce<Bit>([](const Bit &a, const Bit &x)
            { return a | x; }, Bit(false));
        CHECK(result == Bit(false));
    }
    ENDTEST();

    TEST("int подсчёт true")
    {
        bool src[] = {true, false, true, true, false};
        BitSequence bs(src, 5);
        int count = bs.Reduce<int>([](const int &a, const Bit &x)
            { return a + (x == Bit(true) ? 1 : 0); }, 0);
        CHECK(count == 3);
    }
    ENDTEST();

    TEST("int подсчёт false")
    {
        bool src[] = {true, false, true, false};
        BitSequence bs(src, 4);
        int count = bs.Reduce<int>([](const int &a, const Bit &x)
            { return a + (x == Bit(false) ? 1 : 0); }, 0);
        CHECK(count == 2);
    }
    ENDTEST();

    TEST("int сумма 1/0")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        int sum = bs.Reduce<int>([](const int &a, const Bit &x)
            { return a + (x == Bit(true) ? 1 : 0); }, 0);
        CHECK(sum == 2);
    }
    ENDTEST();

    TEST("пустая Bit возвращает init")
    {
        BitSequence bs;
        Bit result = bs.Reduce<Bit>([](const Bit &a, const Bit &x)
            { return a | x; }, Bit(false));
        CHECK(result == Bit(false));
    }
    ENDTEST();

    TEST("пустая int возвращает init")
    {
        BitSequence bs;
        int result = bs.Reduce<int>([](const int &a, const Bit &x)
            { return a + 1; }, 100);
        CHECK(result == 100);
    }
    ENDTEST();

    TEST("один элемент")
    {
        bool src[] = {true};
        BitSequence bs(src, 1);
        int result = bs.Reduce<int>([](const int &a, const Bit &x)
            { return a + (x == Bit(true) ? 1 : 0); }, 0);
        CHECK(result == 1);
    }
    ENDTEST();

    TEST("XOR всех")
    {
        bool src[] = {true, true, false, false};
        BitSequence bs(src, 4);
        Bit result = bs.Reduce<Bit>([](const Bit &a, const Bit &x)
            { return a ^ x; }, Bit(false));
        CHECK(result == Bit(false));
    }
    ENDTEST();

    TEST("не меняет исходную")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        int result = bs.Reduce<int>([](const int &a, const Bit &x)
            { return a + 1; }, 0);
        CHECK(bs.GetLength() == 3);
        CHECK(bs.Get(0) == Bit(true));
        CHECK(result == 3);
    }
    ENDTEST();
}

void TestBitSequence_And()
{
    TEST("And одинаковая длина")
    {
        bool s1[] = {true, true, false, false};
        bool s2[] = {true, false, true, false};
        BitSequence a(s1, 4), b(s2, 4);
        BitSequence *r = a.And(b);
        CHECK(r->GetLength() == 4);
        CHECK(r->Get(0) == Bit(true));
        CHECK(r->Get(1) == Bit(false));
        CHECK(r->Get(2) == Bit(false));
        CHECK(r->Get(3) == Bit(false));
        CHECK(r->ToString() == "1000");
        delete r;
    }
    ENDTEST();

    TEST("And все true")
    {
        bool s1[] = {true, true, true};
        bool s2[] = {true, true, true};
        BitSequence a(s1, 3), b(s2, 3);
        BitSequence *r = a.And(b);
        CHECK(r->ToString() == "111");
        delete r;
    }
    ENDTEST();

    TEST("And разная длина")
    {
        bool s1[] = {true, false};
        bool s2[] = {true, true, false};
        BitSequence a(s1, 2), b(s2, 3);
        BitSequence *r = a.And(b);
        CHECK(r->GetLength() == 2);
        CHECK(r->Get(0) == Bit(true));
        CHECK(r->Get(1) == Bit(false));
        delete r;
    }
    ENDTEST();

    TEST("And пустая и непустая")
    {
        bool s2[] = {true, false};
        BitSequence a, b(s2, 2);
        BitSequence *r = a.And(b);
        CHECK(r->GetLength() == 0);
        delete r;
    }
    ENDTEST();

    TEST("And обе пустые")
    {
        BitSequence a, b;
        BitSequence *r = a.And(b);
        CHECK(r->GetLength() == 0);
        delete r;
    }
    ENDTEST();

    TEST("And не меняет исходные")
    {
        bool s1[] = {true, false};
        bool s2[] = {true, true};
        BitSequence a(s1, 2), b(s2, 2);
        BitSequence *r = a.And(b);
        CHECK(a.GetLength() == 2);
        CHECK(a.Get(0) == Bit(true));
        CHECK(a.Get(1) == Bit(false));
        CHECK(b.GetLength() == 2);
        CHECK(b.Get(0) == Bit(true));
        CHECK(b.Get(1) == Bit(true));
        delete r;
    }
    ENDTEST();

    TEST("And один элемент")
    {
        bool s1[] = {true};
        bool s2[] = {false};
        BitSequence a(s1, 1), b(s2, 1);
        BitSequence *r = a.And(b);
        CHECK(r->GetLength() == 1);
        CHECK(r->Get(0) == Bit(false));
        delete r;
    }
    ENDTEST();
}

void TestBitSequence_Or()
{
    TEST("Or одинаковая длина")
    {
        bool s1[] = {true, true, false, false};
        bool s2[] = {true, false, true, false};
        BitSequence a(s1, 4), b(s2, 4);
        BitSequence *r = a.Or(b);
        CHECK(r->GetLength() == 4);
        CHECK(r->Get(0) == Bit(true));
        CHECK(r->Get(1) == Bit(true));
        CHECK(r->Get(2) == Bit(true));
        CHECK(r->Get(3) == Bit(false));
        CHECK(r->ToString() == "1110");
        delete r;
    }
    ENDTEST();

    TEST("Or все true")
    {
        bool s1[] = {true, true};
        bool s2[] = {false, false};
        BitSequence a(s1, 2), b(s2, 2);
        BitSequence *r = a.Or(b);
        CHECK(r->ToString() == "11");
        delete r;
    }
    ENDTEST();

    TEST("Or смешанные")
    {
        bool s1[] = {true, false};
        bool s2[] = {false, true};
        BitSequence a(s1, 2), b(s2, 2);
        BitSequence *r = a.Or(b);
        CHECK(r->ToString() == "11");
        delete r;
    }
    ENDTEST();

    TEST("Or разная длина")
    {
        bool s1[] = {true};
        bool s2[] = {false, true};
        BitSequence a(s1, 1), b(s2, 2);
        BitSequence *r = a.Or(b);
        CHECK(r->GetLength() == 1);
        CHECK(r->Get(0) == Bit(true));
        delete r;
    }
    ENDTEST();

    TEST("Or пустые")
    {
        BitSequence a, b;
        BitSequence *r = a.Or(b);
        CHECK(r->GetLength() == 0);
        delete r;
    }
    ENDTEST();

    TEST("Or не меняет исходные")
    {
        bool s1[] = {true, false};
        bool s2[] = {false, false};
        BitSequence a(s1, 2), b(s2, 2);
        BitSequence *r = a.Or(b);
        CHECK(a.Get(0) == Bit(true));
        CHECK(b.Get(1) == Bit(false));
        delete r;
    }
    ENDTEST();
}

void TestBitSequence_Xor()
{

    TEST("Xor одинаковая длина")
    {
        bool s1[] = {true, true, false, false};
        bool s2[] = {true, false, true, false};
        BitSequence a(s1, 4), b(s2, 4);
        BitSequence *r = a.Xor(b);
        CHECK(r->GetLength() == 4);
        CHECK(r->Get(0) == Bit(false));
        CHECK(r->Get(1) == Bit(true));
        CHECK(r->Get(2) == Bit(true));
        CHECK(r->Get(3) == Bit(false));
        CHECK(r->ToString() == "0110");
        delete r;
    }
    ENDTEST();

    TEST("Xor одинаковые дают 0")
    {
        bool s1[] = {true, false, true};
        bool s2[] = {true, false, true};
        BitSequence a(s1, 3), b(s2, 3);
        BitSequence *r = a.Xor(b);
        CHECK(r->ToString() == "000");
        delete r;
    }
    ENDTEST();

    TEST("Xor противоположные дают 1")
    {
        bool s1[] = {true, false, true};
        bool s2[] = {false, true, false};
        BitSequence a(s1, 3), b(s2, 3);
        BitSequence *r = a.Xor(b);
        CHECK(r->ToString() == "111");
        delete r;
    }
    ENDTEST();

    TEST("Xor все true")
    {
        bool s1[] = {true, true};
        bool s2[] = {true, true};
        BitSequence a(s1, 2), b(s2, 2);
        BitSequence *r = a.Xor(b);
        CHECK(r->ToString() == "00");
        delete r;
    }
    ENDTEST();

    TEST("Xor разная длина")
    {
        bool s1[] = {true, false, true};
        bool s2[] = {false, true};
        BitSequence a(s1, 3), b(s2, 2);
        BitSequence *r = a.Xor(b);
        CHECK(r->GetLength() == 2);
        CHECK(r->Get(0) == Bit(true));
        CHECK(r->Get(1) == Bit(true));
        delete r;
    }
    ENDTEST();

    TEST("Xor пустые")
    {
        BitSequence a, b;
        BitSequence *r = a.Xor(b);
        CHECK(r->GetLength() == 0);
        delete r;
    }
    ENDTEST();

    TEST("Xor не меняет исходные")
    {
        bool s1[] = {true, false};
        bool s2[] = {false, true};
        BitSequence a(s1, 2), b(s2, 2);
        BitSequence *r = a.Xor(b);
        CHECK(a.GetLength() == 2);
        CHECK(b.GetLength() == 2);
        delete r;
    }
    ENDTEST();
}

void TestBitSequence_Not()
{
    TEST("Not смешанные")
    {
        bool src[] = {true, false, true, false};
        BitSequence bs(src, 4);
        BitSequence *r = bs.Not();
        CHECK(r->GetLength() == 4);
        CHECK(r->Get(0) == Bit(false));
        CHECK(r->Get(1) == Bit(true));
        CHECK(r->Get(2) == Bit(false));
        CHECK(r->Get(3) == Bit(true));
        CHECK(r->ToString() == "0101");
        delete r;
    }
    ENDTEST();

    TEST("Not все true")
    {
        bool src[] = {true, true, true};
        BitSequence bs(src, 3);
        BitSequence *r = bs.Not();
        CHECK(r->ToString() == "000");
        delete r;
    }
    ENDTEST();

    TEST("Not пустая")
    {
        BitSequence bs;
        BitSequence *r = bs.Not();
        CHECK(r->GetLength() == 0);
        CHECK(r->ToString() == "");
        delete r;
    }
    ENDTEST();

    TEST("Not не меняет исходную")
    {
        bool src[] = {true, false};
        BitSequence bs(src, 2);
        BitSequence *r = bs.Not();
        CHECK(bs.GetLength() == 2);
        CHECK(bs.Get(0) == Bit(true));
        CHECK(bs.Get(1) == Bit(false));
        delete r;
    }
    ENDTEST();

    TEST("Not дважды")
    {
        bool src[] = {true, false, true};
        BitSequence bs(src, 3);
        BitSequence *r1 = bs.Not();
        BitSequence *r2 = r1->Not();
        CHECK(r2->ToString() == bs.ToString());
        delete r1;
        delete r2;
    }
    ENDTEST();
}

void TestListSequence_GetOperations()
{
    TEST("GetFirst непустой")
    {
        int src[] = {10, 20, 30};
        MutableListSequence<int> seq(src, 3);
        CHECK(seq.GetFirst() == 10);
    }
    ENDTEST();

    TEST("GetFirst пустая")
    {
        MutableListSequence<int> seq;
        CHECK_THROW(seq.GetFirst(), IndexOutOfRange);
    }
    ENDTEST();

    TEST("GetLast непустой")
    {
        int src[] = {10, 20, 30};
        MutableListSequence<int> seq(src, 3);
        CHECK(seq.GetLast() == 30);
    }
    ENDTEST();

    TEST("GetLast пустая")
    {
        MutableListSequence<int> seq;
        CHECK_THROW(seq.GetLast(), IndexOutOfRange);
    }
    ENDTEST();

    TEST("Get за границами")
    {
        int src[] = {1, 2};
        MutableListSequence<int> seq(src, 2);
        CHECK_THROW(seq.Get(5), IndexOutOfRange);
    }
    ENDTEST();

    TEST("Get пустая")
    {
        MutableListSequence<int> seq;
        CHECK_THROW(seq.Get(0), IndexOutOfRange);
    }
    ENDTEST();

    TEST("GetLength непустой")
    {
        int src[] = {1, 2, 3, 4};
        MutableListSequence<int> seq(src, 4);
        CHECK(seq.GetLength() == 4);
    }
    ENDTEST();

    TEST("GetLength пустая")
    {
        MutableListSequence<int> seq;
        CHECK(seq.GetLength() == 0);
    }
    ENDTEST();
}

void TestListSequence_GetSubsequence()
{
    TEST("GetSubsequence с начала")
    {
        int src[] = {1, 2, 3, 4, 5};
        ImmutableListSequence<int> seq(src, 5);
        Sequence<int> *sub = seq.GetSubsequence(0, 2);
        CHECK(sub->GetLength() == 3);
        CHECK(sub->Get(0) == 1);
        CHECK(sub->Get(1) == 2);
        CHECK(sub->Get(2) == 3);
        delete sub;
    }
    ENDTEST();

    TEST("GetSubsequence из середины")
    {
        int src[] = {10, 20, 30, 40, 50};
        ImmutableListSequence<int> seq(src, 5);
        Sequence<int> *sub = seq.GetSubsequence(1, 3);
        CHECK(sub->GetLength() == 3);
        CHECK(sub->Get(0) == 20);
        CHECK(sub->Get(1) == 30);
        CHECK(sub->Get(2) == 40);
        delete sub;
    }
    ENDTEST();

    TEST("GetSubsequence вся последовательность")
    {
        int src[] = {1, 2, 3};
        ImmutableListSequence<int> seq(src, 3);
        Sequence<int> *sub = seq.GetSubsequence(0, 2);
        CHECK(sub->GetLength() == 3);
        CHECK(sub->Get(0) == 1);
        CHECK(sub->Get(2) == 3);
        delete sub;
    }
    ENDTEST();

    TEST("GetSubsequence один элемент")
    {
        int src[] = {5, 10, 15};
        ImmutableListSequence<int> seq(src, 3);
        Sequence<int> *sub = seq.GetSubsequence(1, 1);
        CHECK(sub->GetLength() == 1);
        CHECK(sub->Get(0) == 10);
        delete sub;
    }
    ENDTEST();

    TEST("GetSubsequence start больше end")
    {
        int src[] = {1, 2, 3};
        ImmutableListSequence<int> seq(src, 3);
        CHECK_THROW(seq.GetSubsequence(2, 0), IndexOutOfRange);
    }
    ENDTEST();

    TEST("GetSubsequence end за границами")
    {
        int src[] = {1, 2};
        ImmutableListSequence<int> seq(src, 2);
        CHECK_THROW(seq.GetSubsequence(0, 5), IndexOutOfRange);
    }
    ENDTEST();

    TEST("GetSubsequence start за границами")
    {
        int src[] = {1, 2};
        ImmutableListSequence<int> seq(src, 2);
        CHECK_THROW(seq.GetSubsequence(3, 5), IndexOutOfRange);
    }
    ENDTEST();

    TEST("GetSubsequence пустая")
    {
        ImmutableListSequence<int> seq;
        CHECK_THROW(seq.GetSubsequence(0, 0), IndexOutOfRange);
    }
    ENDTEST();
}

void TestListSequence_Append()
{
    TEST("Append к непустой Immutable")
    {
        int src[] = {1, 2};
        ImmutableListSequence<int> seq(src, 2);
        Sequence<int> *result = seq.Append(3);
        CHECK(seq.GetLength() == 2);
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 2);
        CHECK(result->Get(2) == 3);
        delete result;
    }
    ENDTEST();

    TEST("Append к пустой Immutable")
    {
        ImmutableListSequence<int> seq;
        Sequence<int> *result = seq.Append(42);
        CHECK(seq.GetLength() == 0);
        CHECK(result->GetLength() == 1);
        CHECK(result->Get(0) == 42);
        delete result;
    }
    ENDTEST();

    TEST("Append к непустой Mutable")
    {
        int src[] = {1, 2};
        MutableListSequence<int> seq(src, 2);
        Sequence<int> *result = seq.Append(3);
        CHECK(seq.GetLength() == 3);
        CHECK(seq.GetLast() == 3);
        delete result;
    }
    ENDTEST();

    TEST("Append несколько раз")
    {
        ImmutableListSequence<int> seq;
        Sequence<int> *r1 = seq.Append(1);
        Sequence<int> *r2 = r1->Append(2);
        Sequence<int> *r3 = r2->Append(3);
        CHECK(r3->GetLength() == 3);
        CHECK(r3->Get(0) == 1);
        CHECK(r3->Get(1) == 2);
        CHECK(r3->Get(2) == 3);
        delete r1;
        delete r2;
        delete r3;
    }
    ENDTEST();
}

void TestListSequence_Prepend()
{
    TEST("Prepend к непустой Immutable")
    {
        int src[] = {2, 3};
        ImmutableListSequence<int> seq(src, 2);
        Sequence<int> *result = seq.Prepend(1);
        CHECK(seq.GetLength() == 2);
        CHECK(seq.GetFirst() == 2);
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 2);
        CHECK(result->Get(2) == 3);
        delete result;
    }
    ENDTEST();

    TEST("Prepend к пустой Immutable")
    {
        ImmutableListSequence<int> seq;
        Sequence<int> *result = seq.Prepend(99);
        CHECK(seq.GetLength() == 0);
        CHECK(result->GetLength() == 1);
        CHECK(result->Get(0) == 99);
        delete result;
    }
    ENDTEST();

    TEST("Prepend несколько раз")
    {
        int src[] = {3};
        ImmutableListSequence<int> seq(src, 1);
        Sequence<int> *r1 = seq.Prepend(2);
        Sequence<int> *r2 = r1->Prepend(1);
        CHECK(r2->GetLength() == 3);
        CHECK(r2->Get(0) == 1);
        CHECK(r2->Get(1) == 2);
        CHECK(r2->Get(2) == 3);
        delete r1;
        delete r2;
    }
    ENDTEST();
}

void TestListSequence_InsertAt()
{
    TEST("InsertAt в начало Immutable")
    {
        int src[] = {2, 3};
        ImmutableListSequence<int> seq(src, 2);
        Sequence<int> *result = seq.InsertAt(1, 0);
        CHECK(seq.GetLength() == 2);
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 2);
        CHECK(result->Get(2) == 3);
        delete result;
    }
    ENDTEST();

    TEST("InsertAt в середину Immutable")
    {
        int src[] = {1, 3};
        ImmutableListSequence<int> seq(src, 2);
        Sequence<int> *result = seq.InsertAt(2, 1);
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 2);
        CHECK(result->Get(2) == 3);
        delete result;
    }
    ENDTEST();

    TEST("InsertAt в конец Immutable")
    {
        int src[] = {1, 2};
        ImmutableListSequence<int> seq(src, 2);
        Sequence<int> *result = seq.InsertAt(3, 2);
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 2);
        CHECK(result->Get(2) == 3);
        delete result;
    }
    ENDTEST();

    TEST("InsertAt в пустую")
    {
        ImmutableListSequence<int> seq;
        Sequence<int> *result = seq.InsertAt(42, 0);
        CHECK(result->GetLength() == 1);
        CHECK(result->Get(0) == 42);
        delete result;
    }
    ENDTEST();

    TEST("InsertAt за границами")
    {
        int src[] = {1, 2};
        ImmutableListSequence<int> seq(src, 2);
        CHECK_THROW(seq.InsertAt(99, 5), IndexOutOfRange);
    }
    ENDTEST();
}

void TestListSequence_Concat()
{
    TEST("Concat две непустых")
    {
        int s1[] = {1, 2};
        int s2[] = {3, 4};
        ImmutableListSequence<int> a(s1, 2), b(s2, 2);
        Sequence<int> *result = a.Concat(&b);
        CHECK(a.GetLength() == 2);
        CHECK(b.GetLength() == 2);
        CHECK(result->GetLength() == 4);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 2);
        CHECK(result->Get(2) == 3);
        CHECK(result->Get(3) == 4);
        delete result;
    }
    ENDTEST();

    TEST("Concat c пустой")
    {
        int src[] = {1, 2, 3};
        ImmutableListSequence<int> a(src, 3), b;
        Sequence<int> *result = a.Concat(&b);
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 1);
        delete result;
    }
    ENDTEST();

    TEST("Concat пустая c непустой")
    {
        ImmutableListSequence<int> a;
        int src[] = {1, 2};
        ImmutableListSequence<int> b(src, 2);
        Sequence<int> *result = a.Concat(&b);
        CHECK(result->GetLength() == 2);
        CHECK(result->Get(0) == 1);
        delete result;
    }
    ENDTEST();

    TEST("Concat две пустых")
    {
        ImmutableListSequence<int> a, b;
        Sequence<int> *result = a.Concat(&b);
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("Concat с nullptr")
    {
        int src[] = {42};
        ImmutableListSequence<int> a(src, 1);
        Sequence<int> *result = a.Concat(nullptr);
        CHECK(result->GetLength() == 1);
        CHECK(result->Get(0) == 42);
        delete result;
    }
    ENDTEST();
}

void TestListSequence_Where()
{
    TEST("Where на четные")
    {
        int src[] = {1, 2, 3, 4, 5, 6};
        ImmutableListSequence<int> seq(src, 6);
        Sequence<int> *result = seq.Where([](const int &x)
            { return x % 2 == 0; });
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 2);
        CHECK(result->Get(1) == 4);
        CHECK(result->Get(2) == 6);
        delete result;
    }
    ENDTEST();

    TEST("Where нет совпадений")
    {
        int src[] = {1, 3, 5};
        ImmutableListSequence<int> seq(src, 3);
        Sequence<int> *result = seq.Where([](const int &x)
            { return x > 10; });
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("Where все подходят")
    {
        int src[] = {2, 4, 6};
        ImmutableListSequence<int> seq(src, 3);
        Sequence<int> *result = seq.Where([](const int &x)
            { return x % 2 == 0; });
        CHECK(result->GetLength() == 3);
        delete result;
    }
    ENDTEST();

    TEST("Where пустая")
    {
        ImmutableListSequence<int> seq;
        Sequence<int> *result = seq.Where([](const int &x)
            { return x > 0; });
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("Where не меняет исходную")
    {
        int src[] = {1, 2, 3};
        ImmutableListSequence<int> seq(src, 3);
        Sequence<int> *result = seq.Where([](const int &x)
            { return x > 1; });
        CHECK(seq.GetLength() == 3);
        CHECK(seq.Get(0) == 1);
        delete result;
    }
    ENDTEST();
}

void TestListSequence_Map()
{
    TEST("Map умножение")
    {
        int src[] = {1, 2, 3};
        ImmutableListSequence<int> seq(src, 3);
        Sequence<int> *result = seq.Map<int>([](const int &x)
            { return x * 10; });
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 10);
        CHECK(result->Get(1) == 20);
        CHECK(result->Get(2) == 30);
        delete result;
    }
    ENDTEST();

    TEST("Map в строку")
    {
        int src[] = {1, 2};
        ImmutableListSequence<int> seq(src, 2);
        Sequence<std::string> *result = seq.Map<std::string>([](const int &x)
            { return "x" + std::to_string(x); });
        CHECK(result->GetLength() == 2);
        CHECK(result->Get(0) == "x1");
        CHECK(result->Get(1) == "x2");
        delete result;
    }
    ENDTEST();

    TEST("Map пустая")
    {
        ImmutableListSequence<int> seq;
        Sequence<int> *result = seq.Map<int>([](const int &x)
            { return x * 2; });
        CHECK(result->GetLength() == 0);
        delete result;
    }
    ENDTEST();

    TEST("Map не меняет исходную")
    {
        int src[] = {1, 2, 3};
        ImmutableListSequence<int> seq(src, 3);
        Sequence<int> *result = seq.Map<int>([](const int &x)
            { return x * 100; });
        CHECK(seq.GetLength() == 3);
        CHECK(seq.Get(0) == 1);
        delete result;
    }
    ENDTEST();
}

void TestListSequence_Reduce()
{
    TEST("Reduce сумма")
    {
        int src[] = {1, 2, 3, 4, 5};
        ImmutableListSequence<int> seq(src, 5);
        int result = seq.Reduce<int>([](const int &a, const int &x)
            { return a + x; }, 0);
        CHECK(result == 15);
    }
    ENDTEST();

    TEST("Reduce произведение")
    {
        int src[] = {1, 2, 3, 4};
        ImmutableListSequence<int> seq(src, 4);
        int result = seq.Reduce<int>([](const int &a, const int &x)
            { return a * x; }, 1);
        CHECK(result == 24);
    }
    ENDTEST();

    TEST("Reduce с init")
    {
        int src[] = {1, 2, 3};
        ImmutableListSequence<int> seq(src, 3);
        int result = seq.Reduce<int>([](const int &a, const int &x)
            { return a + x; }, 10);
        CHECK(result == 16);
    }
    ENDTEST();

    TEST("Reduce пустая возвращает init")
    {
        ImmutableListSequence<int> seq;
        int result = seq.Reduce<int>([](const int &a, const int &x)
            { return a + x; }, 50);
        CHECK(result == 50);
    }
    ENDTEST();

    TEST("Reduce не меняет исходную")
    {
        int src[] = {1, 2, 3};
        ImmutableListSequence<int> seq(src, 3);
        int result = seq.Reduce<int>([](const int &a, const int &x)
            { return a + x; }, 0);
        CHECK(seq.GetLength() == 3);
        CHECK(seq.Get(0) == 1);
        CHECK(result == 6);
    }
    ENDTEST();
}

void TestListSequence_Split()
{
    TEST("Split")
    {
        int src[] = {1, 2, 0, 3, 4, 0, 5};
        ImmutableListSequence<int> seq(src, 7);
        Sequence<Sequence<int> *> *parts = seq.Split([](const int &x)
            { return x == 0; });
        CHECK(parts->GetLength() == 3);
        CHECK(parts->Get(0)->GetLength() == 2);
        CHECK(parts->Get(0)->Get(0) == 1);
        CHECK(parts->Get(0)->Get(1) == 2);
        CHECK(parts->Get(2)->Get(0) == 5);
        delete parts->Get(0);
        delete parts->Get(1);
        delete parts->Get(2);
        delete parts;
    }
    ENDTEST();

    TEST("Split разделитель в начале")
    {
        int src[] = {0, 1, 2};
        ImmutableListSequence<int> seq(src, 3);
        Sequence<Sequence<int> *> *parts = seq.Split([](const int &x)
            { return x == 0; });
        CHECK(parts->GetLength() == 1);
        CHECK(parts->Get(0)->GetLength() == 2);
        CHECK(parts->Get(0)->Get(0) == 1);
        delete parts->Get(0);
        delete parts;
    }
    ENDTEST();

    TEST("Split пустая")
    {
        ImmutableListSequence<int> seq;
        Sequence<Sequence<int> *> *parts = seq.Split([](const int &x)
            { return x == 0; });
        CHECK(parts->GetLength() == 0);
        delete parts;
    }
    ENDTEST();
}

void TestListSequence_Slice()
{
    TEST("Slice замена")
    {
        int src[] = {1, 2, 3, 4, 5};
        int ins[] = {9, 10};
        ImmutableListSequence<int> seq(src, 5);
        ImmutableListSequence<int> insSeq(ins, 2);
        Sequence<int> *result = seq.Slice(1, 2, &insSeq);
        CHECK(result->GetLength() == 5);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 9);
        CHECK(result->Get(2) == 10);
        CHECK(result->Get(3) == 4);
        CHECK(result->Get(4) == 5);
        delete result;
    }
    ENDTEST();

    TEST("Slice удаление")
    {
        int src[] = {1, 2, 3, 4, 5};
        ImmutableListSequence<int> seq(src, 5);
        Sequence<int> *result = seq.Slice(1, 2);
        CHECK(result->GetLength() == 3);
        CHECK(result->Get(0) == 1);
        CHECK(result->Get(1) == 4);
        CHECK(result->Get(2) == 5);
        delete result;
    }
    ENDTEST();

    TEST("Slice отрицательный индекс")
    {
        int src[] = {1, 2, 3, 4, 5};
        ImmutableListSequence<int> seq(src, 5);
        Sequence<int> *result = seq.Slice(-2, 1);
        CHECK(result->GetLength() == 4);
        CHECK(result->Get(3) == 5);
        delete result;
    }
    ENDTEST();
}

void RunAllTests()
{
    std::cout << "tests running" << std::endl;
    testsFailed = 0;

    TestDynamicArray_Construction();
    TestDynamicArray_GetSet();
    TestDynamicArray_Resize();
    TestDynamicArray_OperatorBracket();
    TestDynamicArray_Assignment();
    TestDynamicArray_Iterators();

    TestLinkedList_Construction();
    TestLinkedList_GetOperations();
    TestLinkedList_AddOperations();
    TestLinkedList_GetSubList();
    TestLinkedList_Concat();
    TestLinkedList_OperatorBracket();
    TestLinkedList_Assignment();
    TestLinkedList_Iterator();

    TestBitSequence_GetOperations();
    TestBitSequence_Subsequence();
    TestBitSequence_Append();
    TestBitSequence_Prepend();
    TestBitSequence_InsertAt();
    TestBitSequence_Where();
    TestBitSequence_Concat();
    TestBitSequence_Map();
    TestBitSequence_Reduce();
    TestBitSequence_And();
    TestBitSequence_Or();
    TestBitSequence_Xor();
    TestBitSequence_Not();

    TestListSequence_GetOperations();
    TestListSequence_GetSubsequence();
    TestListSequence_Append();
    TestListSequence_Prepend();
    TestListSequence_InsertAt();
    TestListSequence_Concat();
    TestListSequence_Where();
    TestListSequence_Map();
    TestListSequence_Reduce();
    TestListSequence_Split();
    TestListSequence_Slice();

    std::cout<< "   РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ" << std::endl;
    std::cout << "Не пройдено: " << testsFailed << std::endl;
    if (testsFailed == 0)
        std::cout << "\nВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!" << std::endl;
    else
        std::cout << "\nЕСТЬ НЕПРОЙДЕННЫЕ ТЕСТЫ!" << std::endl;
}