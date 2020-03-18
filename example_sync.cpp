//
// Created by root on 18-12-14.
//


#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <ostream>
using namespace std;
//https://blog.csdn.net/hzhsan/article/details/25124901

struct A
{
    int a_;
    int b_;
    friend ostream& operator<<(ostream& o, const A& obj) {
        o << "a:" << obj.a_ << " b:" << obj.b_ << endl;
        return o;
    }

};

union RateLimitStat
{
    uint64_t u64;
    struct {
        uint32_t timestamp;
        uint32_t count;
    } fields;
};

int main()
{
    int global_int = 0;
    __sync_fetch_and_add(&global_int, 2);
    printf("%d\n", global_int);
    __sync_fetch_and_sub(&global_int, 2);
    printf("%d\n", global_int);
    __sync_fetch_and_or(&global_int, 1);
    printf("%d\n", global_int);
    __sync_fetch_and_xor(&global_int, 3);
    printf("%d\n", global_int);
    __sync_fetch_and_nand(&global_int, 1);
    printf("%d\n", global_int);
    cout << "first add:" << __sync_add_and_fetch(&global_int, 2) << endl;
    RateLimitStat stat;
    stat.fields.timestamp = time(NULL);
    stat.fields.count = 3;
    this_thread::sleep_for(chrono::milliseconds(1));
    time_t now = time(NULL);
    if (stat.fields.timestamp != now && __sync_lock_test_and_set(&stat.fields.timestamp, now) != now) {
        stat.fields.count = 0;
    }
    cout << stat.fields.count << endl;
}