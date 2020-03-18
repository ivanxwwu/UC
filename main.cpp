#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>
#include <memory>
#include <map>
using namespace std;

struct A
{
    vector<int> v;
};

void f(const A& a) {
    a.v.push_back(111);
}

void f(const map<string, string>& m) {
    cout << m.at("cc") << endl;
    cout << m.find("cc")->second << endl;
//    const map<string, string>& map2 = m;
//    map<string, string>::iterator iterator2 = const_cast<map<string, string>::iterator>(map2.find("cc"));
//    if (iterator2 != map2.end()) {
//        cout << iterator2->second << endl;
//    }
}

int main() {
    map<string, string> map1;
    map1.insert(make_pair("cc", "334"));
    f(map1);


}

