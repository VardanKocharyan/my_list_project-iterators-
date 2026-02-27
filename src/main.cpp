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
    my_list<Vrdo>::iterator Vrdo_it = v.begin();

    Vrdo_it->print();
    std::cout << Vrdo_it->x << std::endl;

    my_list<int>::const_iterator const_it = b.begin();
    std::cout << *const_it << std::endl;

    //reverce it
    my_list<int>::reverse_iterator r_it = b.rbegin();
    ++r_it;
    std::cout << *r_it << std::endl;

    return 0;
}



