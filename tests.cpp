#include <iostream>

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "BitSequence.h"
#include "Option.hpp"
#include "exceptions.hpp"

static int testsPassed = 0;
static int testsFailed = 0;

#define TEST(name)                                 \
    do                                             \
    {                                              \
        std::cout << "  Тест: " << name << "... "; \
        try

#define ENDTEST()                                                       \
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

#define CHECK(cond)                                                   \
    do                                                                \
    {                                                                 \
        if (!(cond))                                                  \
        {                                                             \
            throw std::runtime_error("Проверка не пройдена: " #cond); \
        }                                                             \
    } while (0)

#define CHECK_THROW(expr, exc_type)                                                              \
    do                                                                                           \
    {                                                                                            \
        bool caught = false;                                                                     \
        try                                                                                      \
        {                                                                                        \
            expr;                                                                                \
        }                                                                                        \
        catch (const exc_type &)                                                                 \
        {                                                                                        \
            caught = true;                                                                       \
        }                                                                                        \
        catch (...)                                                                              \
        {                                                                                        \
        }                                                                                        \
        if (!caught)                                                                             \
        {                                                                                        \
            throw std::runtime_error("Ожидалось исключение " #exc_type ", но оно не выброшено"); \
        }                                                                                        \
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

void RunAllTests()
{
    std::cout << "tests running" << std::endl;
    int testsPassed;
    int testsFailed;
    testsPassed = 0;
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

    std::cout<< "   РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ" << std::endl;
    std::cout << "Пройдено: " << testsPassed << std::endl;
    std::cout << "Не пройдено: " << testsFailed << std::endl;
    std::cout << "Всего: " << (testsPassed + testsFailed) << std::endl;

    if (testsFailed == 0)
        std::cout << "\nВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!" << std::endl;
    else
        std::cout << "\nЕСТЬ НЕПРОЙДЕННЫЕ ТЕСТЫ!" << std::endl;
}