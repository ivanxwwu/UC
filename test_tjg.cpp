//
// Created by root on 19-1-20.
//
#include <chrono>
#include <iostream>
using namespace std;

int main()
{
    while (1) {
        this_thread::sleep_for(chrono::seconds(1));
    }
}