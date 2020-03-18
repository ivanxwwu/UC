//
// Created by root on 18-11-25.
//

#include <iostream>
#define BOOST_STACKTRACE_LINK
#include "boost/stacktrace.hpp"

using namespace std;

void fn() {
    auto stacktrace = boost::stacktrace::stacktrace();
    for (size_t i=0; i<stacktrace.size(); i++) {
        Frame frame;
        frame.name = stacktrace[i].name();
        frame.sourceFile = stacktrace[i].source_file();
        frame.sourceLine = stacktrace[i].source_line();
        frames.push_back(frame);
    }
}

int main() {
    fn();
}