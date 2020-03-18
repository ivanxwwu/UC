//
// Created by root on 18-11-24.
//


#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

int main() {
    steady_clock::time_point t1 = steady_clock::now();
    this_thread::sleep_for(chrono::seconds(1));
    steady_clock::time_point t2 = steady_clock::now();
    cout << duration_cast<duration<double>>(t2-t1).count() <<endl;

}

