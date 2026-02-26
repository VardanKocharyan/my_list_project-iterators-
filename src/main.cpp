#include "my_list.hpp"
class Vrdo{
    public:
        int x ;
        void print(){ std::cout << x <<std::endl; }
};
int main() {
    my_list<int> a;//default
    my_list<int> b(5, 6);//parameter
    my_list<int> c(b);//copy ctor
    my_list<int> d {1, 2, 3, 4, 5, 6};
    my_list<int> e {10, 20, 30, 40};

  
    std::cout << "b: " <<  b << std::endl;
    std::cout << "c: " <<  c << std::endl;
    std::cout << "d: " <<  d << std::endl; 

    b = d;
    std::cout << "b = d: " << b << std::endl;

    b = std::move(e);
    std::cout << "b = std::move(e) \n" << b  << "\ne: " << e << "\n" << std::endl;

    b = {123, 234, 345, 456};
    std::cout << "b = {123, 234, 345, 456}: " << b << std::endl;

    my_list<int>::iterator it = b.begin();
    std::cout << *it << std::endl;
    my_list<Vrdo> v{Vrdo(6), Vrdo(5)};
    auto i = v.begin();
    i->print();
    std::cout << i->x << std::endl;



    return 0;
}

/*
Քայլ 1
✔ Constructor
✔ Destructor
✔ size()
✔ empty()

Քայլ 2
✔ clear()

Քայլ 3
✔ insert_before (private)
✔ remove_node (private)

Քայլ 4
✔ push_back
✔ push_front
✔ pop_back
✔ pop_front

Քայլ 5
✔ copy constructor
✔ move constructor
✔ copy assignment
✔ move assignment
*/
