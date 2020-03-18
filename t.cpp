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
#include <vector>
#include <stack>
#include <map>
#include <chrono>
#include <thread>
#include <sstream>
#include <zconf.h>
#include "time.h"

using namespace std;

#define INIT(yyuy) \
    cout << yyuy << endl;
namespace Srf_Agent
{
    namespace UtilAll
    {
        void encode() {
            while(true) {
                for(int j=0;j<10000; j++) {

                    stringstream buf;
                    //buf << "djfckjlsdljfsdj;lkfjdslkf";

                }
                //this_thread::sleep_for(chrono::microseconds(1));
            }
        }
    }
}

namespace srf
{
    class Span
    {
    public:
        Span(){}
        ~Span(){}
        long traceId;
        long spanId;
        long parentId;
        string caller;
        string callee;

    };
    class SRFMsg
    {
    public:
        void HandleProcess()
        {

            while (true) {
//                Span* span = new Span();
//                span->callee = "1232";
//
//                delete span;
                int *pInt = (int*)malloc(4);
                free(pInt);
//                char buf[1024];
//                snprintf(buf, sizeof(buf),
//                         "123213213 df %d %d", 123, 345344);
                //this_thread::sleep_for(chrono::microseconds(1));
            }
        }
    };
}
#include "sys/time.h"
#include "time.h"
#define rdtsc(low,high) __asm__ \
 __volatile__("rdtsc" : "=a" (low), "=d" (high))
unsigned long long get_cycles()
{
    unsigned low, high;
    unsigned long long val;
    rdtsc(low,high);
    val = high;
    val = (val << 32) | low; //将 low 和 high 合成一个 64 位值
    return val;
}
double get_cpu_mhz(void)
{
    FILE* f;
    char buf[256];
    double mhz = 0.0;

    f = fopen("/proc/cpuinfo","r"); //打开 proc/cpuinfo 文件
    if (!f)
        return 0.0;
    while(fgets(buf, sizeof(buf), f)) {
        double m;
        int rc;
        rc = sscanf(buf, "cpu MHz : %lf", &m); //读取 cpu MHz
        printf("buf:%s\n", buf);
        if (mhz == 0.0) {
            mhz = m;
            mhz=2601;
            break;
        }
    }
    fclose(f);
    return mhz; //返回 HZ 值
}

int main() {
    srf::SRFMsg msg;
    msg.HandleProcess();
//    typedef unsigned long cycles_t;
//    struct timeval t1, t2;
//    struct timeval t3;
//    puts("111111111111111111111");
//    gettimeofday(&t3, NULL);
//    puts("222222222222222222222");
//    time(NULL);
//    int a; int b;
//    rdtsc(a, b);
//    cout << a << endl;
//    cout << b << endl;
////    while(true) {
////        time(NULL);
////    }
//    double mhz;
//    mhz = get_cpu_mhz();
//    cycles_t c1, c2;
//
//    for(;;)
//    {
//        c1 = get_cycles();
//        printf("%ld\n", c1/mhz);
//        cout << c1/mhz << endl;
//        sleep(1);
//        c2 = get_cycles();
////c2 和 c1 的差值应该为 1000000us，即 1 秒
//        printf("1 sec = %g usec\n", (c2 - c1) / mhz);
//    }
}

#endif //UC_T_H