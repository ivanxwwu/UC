#include <map>
#include <aio.h>
#include <fcntl.h>
#include <iostream>
#include <strings.h>
#include <set>
#include <map>
#include <unordered_map>
using namespace std;

#define BUFSIZE 1024

int main() {
    unordered_multimap<int, int> m;
    m.emplace(1,2);
    m.emplace(12,2);
    m.emplace(1,3);
    for (auto it = m.begin(); it != m.end(); it++) {
        cout << it->first << endl;
    }
    auto skget = m.equal_range(1);
    cout << (skget.first == skget.second) << endl ;
    for (auto skbeg=skget.first; skbeg!=skget.second; skbeg++) {
        cout << skbeg->first << "->" << skbeg->second << " " << endl;
    }
}

