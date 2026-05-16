#include "tests.h"
#include <windows.h>
#include "DynamicArray.hpp"
#include "iostream"

int main()
{
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);

    // RunAllTests();
    // return 0;
    DynamicArray<int> *Marray = new DynamicArray<int> (10);
    std::cout << sizeof(Marray) << std::endl;
    std::cout << sizeof(*Marray) << std::endl;
    // std::cout << sizeof(Marray) << std::endl;
    // int x=5;
    // int *p=&x;
}


//в следующий раз попрошу руками написать реализацию односвязного списка