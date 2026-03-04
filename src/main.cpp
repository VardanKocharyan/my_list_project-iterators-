// main.cpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <initializer_list>
#include <algorithm>

#include "my_list.hpp" // <-- make sure this header is the file that contains the code you gave

using std::cout;
using std::endl;

// simple test harness
static int failures = 0;
static int checks = 0;

void expect(bool cond, const std::string& what) {
    ++checks;
    if (cond) {
        cout << "[PASS] " << what << '\n';
    } else {
        ++failures;
        cout << "[FAIL] " << what << '\n';
    }
}

// convert list to vector for comparison (non-const)
template<typename T>
std::vector<T> to_vector(my_list<T>& l) {
    std::vector<T> v;
    for (auto it = l.begin(); it != l.end(); ++it)
        v.push_back(*it);
    return v;
}
template<typename T>
std::vector<T> to_vector(const my_list<T>& l) {
    std::vector<T> v;
    for (auto it = l.begin(); it != l.end(); ++it)
        v.push_back(*it);
    return v;
}

int main() {
    cout << "=== my_list test suite ===\n\n";

    // 1) Default ctor
    my_list<int> a;
    expect(a.empty(), "default ctor: list is empty");
    expect(a.size() == 0, "default ctor: size() == 0");

    // 2) push_back / push_front / pop_back / pop_front
    a.push_back(10);
    a.push_back(20);
    a.push_front(5);
    expect(a.size() == 3, "push_back/push_front: size == 3");
    {
        auto v = to_vector(a);
        std::vector<int> expected = {5, 10, 20};
        expect(v == expected, "push_back/push_front: contents are 5 10 20");
    }
    a.pop_back();
    expect(a.size() == 2, "pop_back reduces size");
    {
        std::vector<int> expected = {5, 10};
        expect(to_vector(a) == expected, "pop_back: contents are 5 10");
    }
    a.pop_front();
    expect(a.size() == 1, "pop_front reduces size");
    expect(to_vector(a) == std::vector<int>{10}, "pop_front: contents are 10");

    // 3) constructor (count, val)
    my_list<int> b(3, 7);
    expect(b.size() == 3, "ctor(count, val): size == 3");
    expect(to_vector(b) == std::vector<int>{7,7,7}, "ctor(count,val): all elements == 7");

    // 4) initializer_list ctor
    my_list<int> c{1,2,3};
    expect(c.size() == 3, "initializer_list ctor: size == 3");
    expect(to_vector(c) == std::vector<int>{1,2,3}, "initializer_list ctor: contents 1 2 3");

    // 5) copy ctor (deep copy)
    my_list<int> d = c;
    expect(d.size() == 3, "copy ctor: copied size == 3");
    expect(to_vector(d) == to_vector(c), "copy ctor: contents equal to source");
    // modify source, make sure copy doesn't change
    c.push_back(99);
    expect(to_vector(d) == std::vector<int>{1,2,3}, "copy ctor: copy unchanged when source modified");

    // 6) move ctor
    my_list<int> e = std::move(d); // d should become empty
    expect(e.size() == 3, "move ctor: target has moved elements");
    expect(d.empty(), "move ctor: source is empty after move");

    // 7) operator= copy
    my_list<int> f;
    f = c; // c currently {1,2,3,99}
    expect(to_vector(f) == to_vector(c), "copy assignment operator: contents match");

    // 8) operator= move
    my_list<int> g;
    g = std::move(f);
    expect(g.size() == c.size(), "move assignment operator: size matches source before move");
    expect(f.empty(), "move assignment operator: source empty after move");

    // 9) operator= initializer_list
    g = {7,8};
    expect(g.size() == 2 && to_vector(g) == std::vector<int>{7,8}, "initializer_list operator= works");

    // 10) assign(count, value) and assign(initializer_list)
    my_list<int> h;
    h.assign(4, 42);
    expect(h.size() == 4 && to_vector(h) == std::vector<int>{42,42,42,42}, "assign(count, value)");
    h.assign({9,8,7});
    expect(h.size() == 3 && to_vector(h) == std::vector<int>{9,8,7}, "assign(initializer_list)");

    // 11) front / back (non-const)
    expect(h.front() == 9, "front() returns first");
    expect(h.back() == 7, "back() returns last");

    // 12) front / back (const)
    const my_list<int> hc{11, 22};
    expect(hc.front() == 11, "const front() OK");
    expect(hc.back() == 22, "const back() OK");

    // 13) insert single (const_iterator param)
    my_list<int> ins;
    ins = {1,3};
    auto it = ins.begin(); ++it; // points to '3' (begin() points at 1)
    auto inserted = ins.insert(it, 2); // insert before 3
    expect(*inserted == 2, "insert(pos, value): returned iterator points to inserted element");
    expect(to_vector(ins) == std::vector<int>{1,2,3}, "insert single: contents are 1 2 3");

    // rvalue insert
    inserted = ins.insert(ins.end(), 4);
    expect(to_vector(ins) == std::vector<int>{1,2,3,4}, "insert rvalue at end");

    // 14) insert(count, value)
    auto it2 = ins.begin(); ++it2; // points to 2
    auto first_inserted = ins.insert(it2, 2, 99); // insert two 99s before 2
    // first_inserted should point to first inserted 99
    expect(*first_inserted == 99, "insert(count,val) returns iterator to first inserted");
    // expected sequence: 1, 99, 99, 2, 3, 4
    expect(to_vector(ins) == std::vector<int>{1,99,99,2,3,4}, "insert(count,val) contents check");

    // 15) insert(initializer_list)
    auto pos = ins.begin(); ++pos; // point to first 99
    auto fi = ins.insert(pos, {7,8}); // insert 7,8 before first 99
    expect(*fi == 7, "insert(initializer_list) returns first inserted element");
    expect(to_vector(ins) == std::vector<int>{1,7,8,99,99,2,3,4}, "insert(initializer_list) contents check");

    // 16) emplace (test with std::string)
    my_list<std::string> s;
    s.emplace(s.end(), "hello");
    s.emplace(s.begin(), "world");
    expect(to_vector(s) == std::vector<std::string>{"world","hello"}, "emplace constructs elements in place");

    // 17) erase single (iterator)
    my_list<int> er = {1,2,3,4,5};
    auto erase_it = er.begin(); ++erase_it; // points to '2'
    auto next_after_erase = er.erase(erase_it); // erase '2'
    expect(*next_after_erase == 3, "erase(iterator) returns iterator to element after erased one");
    expect(to_vector(er) == std::vector<int>{1,3,4,5}, "erase(iterator) removed correct element");

    // 18) erase single (const_iterator)
    // create const_iterator by using const reference
    my_list<int> er2 = {10,20,30};
    const my_list<int>& er2_const_ref = er2;
    auto cit = er2_const_ref.begin(); // const_iterator to 10
    // note: your erase(const_iterator) expects a const_iterator; pass er2.begin() casted
    auto cit2 = er2.erase(cit); // erase 10
    expect(to_vector(er2) == std::vector<int>{20,30}, "erase(const_iterator) removed first element");
    // cit2 should point to 20
    expect(*cit2 == 20, "erase(const_iterator) returned iterator to next element");

    // 19) erase range (iterator, iterator)
    my_list<int> er3 = {1,2,3,4,5};
    auto itf = er3.begin(); ++itf; // points to 2
    auto l = er3.begin(); ++l; ++l; ++l; // move to 4
    // erase [2,4) i.e. 2 and 3
    auto rret = er3.erase(itf, l);
    expect(to_vector(er3) == std::vector<int>{1,4,5}, "erase(iterator, iterator) removed range");
    expect(*rret == 4, "erase(range) returned iterator to first element after erased range");

    // 20) erase range (const_iterator, const_iterator)
    my_list<int> er4 = {9,8,7,6};
    auto f4 = er4.begin(); ++f4; // 8
    auto l4 = er4.begin(); ++l4; ++l4; // 7
    er4.erase(f4, l4); // erase {8}
    expect(to_vector(er4) == std::vector<int>{9,7,6}, "erase(const_iterator, const_iterator) works");

    // 21) operator<<
    my_list<int> out = {1,2,3};
    std::ostringstream oss;
    oss << out;
    expect(oss.str() == "1 2 3 " , "operator<< prints elements separated by spaces");

    // 22) clear()
    my_list<int> cl = {1,2,3};
    cl.clear();
    expect(cl.empty() && cl.size() == 0, "clear() empties list (size==0)");

    // 23) reverse iterator basic behavior (rbegin)
    my_list<int> rev = {10,20,30};
    auto riter = rev.rbegin();
    bool rbegin_ok = (*riter == 30); // first deref from rbegin should be last element
    ++riter;
    bool rnext_ok = (*riter == 20);
    ++riter;
    bool rlast_ok = (*riter == 10);
    expect(rbegin_ok && rnext_ok && rlast_ok, "reverse iterator (rbegin) yields elements in reverse order when advanced size() times");

    // 24) iterator pre/post increment and decrement
    my_list<int> ittest = {1,2,3};
    auto itpre = ittest.begin();
    ++itpre; // pre-inc -> points to 2
    expect(*itpre == 2, "pre-increment moves to next element");
    auto itpost = itpre++;
    expect(*itpost == 2 && *itpre == 3, "post-increment returns old value and advances iterator");
    // test decrement
    --itpre;
    expect(*itpre == 2, "pre-decrement works");
    auto itpostdec = itpre--;
    expect(*itpostdec == 2 && *itpre == 1, "post-decrement works");

    // 25) const_iterator conversion from iterator (construct const from non-const)
    my_list<int> conv = {1,2};
    my_list<int>::iterator nonc = conv.begin();
    my_list<int>::const_iterator constc(nonc);
    expect(*constc == 1, "const_iterator can be constructed from iterator");

    // 26) size(), empty() interplay after many ops
    my_list<int> many;
    for (int i = 0; i < 20; ++i) many.push_back(i);
    expect(many.size() == 20, "many.push_back increased size to 20");
    for (int i = 0; i < 20; ++i) many.pop_back();
    expect(many.empty(), "after popping all elements, empty() == true");

    // 27) destructor tested by scoping (no runtime check, but we can create inside block)
    {
        my_list<int> tmp = {1,2,3,4,5};
    } // should not leak in a proper run; nothing to assert automatically

    // Summary
    cout << "\n=== SUMMARY ===\n";
    cout << "Checks performed : " << checks << '\n';
    if (failures == 0) {
        cout << "All tests passed ✅\n";
        return 0;
    } else {
        cout << "Failures         : " << failures << '\n';
        cout << "Some tests failed ❌\n";
        return 1;
    }
}
