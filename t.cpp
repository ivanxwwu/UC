#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

int main() {
    set<int> set1 = {1,2,3};
    set<int> set2 = {2,3,34};
    set_union(set1.begin(), set1.end(), set2.begin(),set2.end(), set1.begin());
}