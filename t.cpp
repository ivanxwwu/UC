//
// Created by root on 18-10-10.
//

#ifndef UC_T_H
#define UC_T_H
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <cxxabi.h>
using namespace std;
namespace ABC
{
    class Foo
    {

    };
}
using namespace ABC;
int main() {
    cout << typeid(Foo*[10]).name() << endl;
    char* name = abi::__cxa_demangle(typeid(Foo*[10]).name(), nullptr, nullptr, nullptr);
    std::cout << name << std::endl;
    free(name);

}

#endif //UC_T_H