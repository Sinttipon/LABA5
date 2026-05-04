#include <iostream>

#include "DynamicArray.hpp"
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