#pragma once
#include <iostream>
#include <cstddef>

#include "conditional.hpp"
#include "enable_if.hpp"

template<typename T>
class my_list {
    public:
        using value_type = T;
        using size_type = size_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

    private: //Node
        struct NodeBase {
            NodeBase* next;
            NodeBase* prev;            
        };
        struct Node : NodeBase {
            value_type value;

            Node(const_reference v = value_type()) : value(v) {}
        };

        NodeBase* sentinel;
        size_type msize;

        //iterator
        template <bool isConst>
        class list_iterator {
            public:
                using differance_type = ptrdiff_t;
                using iterator_category = std::bidirectional_iterator_tag;

                using reference = conditional_t<
                    isConst,
                    const_reference,
                    reference
                >;

                using pointer = conditional_t<
                    isConst,
                    const_pointer,
                    pointer
                >;

            private:
                NodeBase* curr;
                using NodePtr = NodeBase*;

            public:
                list_iterator(NodePtr r = nullptr) : curr(r) {}

                template <bool B = isConst, typename = enable_if_t<B>>
                list_iterator(const list_iterator<false>& r) : curr(r.curr) {}

                list_iterator& operator=(const list_iterator& r) {
                    if (this != &r)
                        curr = r.curr;
                    return *this;
                }

                list_iterator& operator++() { curr = curr->next; return *this; }
                list_iterator operator++(int) { auto tmp = *this; curr = curr->next; return tmp; }
                list_iterator& operator--() { curr = curr->prev; return *this; }
                list_iterator operator--(int) { auto tmp = *this; curr = curr->prev; return tmp; }
                reference operator*() const { return static_cast<Node*>(curr)->value; }
                pointer operator->() const { return &(static_cast<Node*>(curr)->value); }

                bool operator==(const list_iterator& r) const { return curr == r.curr; }
                bool operator!=(const list_iterator& r) const { return curr != r.curr; }
        };

    public:
        using iterator = list_iterator<false>;
        using const_iterator = list_iterator<true>;

        //ctors
        my_list();
        my_list(size_type count, const_reference val = T());
        my_list(const my_list& r);
        my_list(my_list&& r);
        my_list(std::initializer_list<T> list);
        //dtor
        ~my_list();

        //operator =
        my_list& operator=(const my_list& r);
        my_list& operator=(my_list&& r);
        my_list& operator=(std::initializer_list<value_type> ilist);

        //iterators (don't use. :) ) 
        iterator begin() { return iterator(sentinel->next); }
        const_iterator begin() const { return const_iterator(sentinel->next); }
//        const_iterator cbegin() const;

//        reverce_iterator rbegin();
//        const_reverce_iterator rbegin();
//        const_reverce_iterator crbegin();

        iterator end() { return iterator(sentinel); }
        const_iterator end() const { return const_iterator(sentinel); }
//        const_iterator cend() const

//        reverce_iterator rend()
//        const_reverce_iterator rend() const;
//        const_reverce_iterator crend() const;


        bool empty() const { return sentinel->next = sentinel; }
        size_type size() { return msize; }

        void clear();

        template<typename U>
        friend std::ostream& operator<<(std::ostream& ost, const my_list<U>& l);
};

//constructors
//default
template<typename T>
my_list<T>::my_list() : sentinel(new NodeBase) , msize(0) {
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
}

//param ctor
template<typename T>
my_list<T>::my_list(size_type count, const_reference val) : sentinel(new NodeBase), msize(count) {
    sentinel->next = sentinel;
    sentinel->prev = sentinel;

    NodeBase* tmp = sentinel;

    for (size_type i{}; i < count; ++i) {
        Node* node = new Node(T{val});

        tmp->next = node;
        node->prev = tmp;
        node->next = sentinel;
        sentinel->prev = node;

        tmp = node;
    }
}

//copy ctor
template<typename T>
my_list<T>::my_list(const my_list<T>& r) : sentinel(new NodeBase), msize(r.msize) {
        sentinel->next = sentinel;
        sentinel->prev = sentinel;

        NodeBase* left = sentinel;
        NodeBase* right = r.sentinel->next;

        while (right != r.sentinel) {
            Node* node = new Node(static_cast<Node*>(right)->value);

            left->next = node;
            node->prev = left;
            node->next = sentinel;
            sentinel->prev = node;

            left = node;
            right = right->next;
        }
}

//move ctor
template<typename T>
my_list<T>::my_list(my_list<T>&& r) : sentinel(new NodeBase), msize(r.msize) {
    if (r.msize == 0) {
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    } else {
        sentinel->next = r.sentinel->next;
        sentinel->prev = r.sentinel->prev;

        sentinel->next->prev = sentinel;
        sentinel->prev->next = sentinel;
    }

    r.msize = 0;
    r.sentinel->next = r.sentinel;
    r.sentinel->prev = r.sentinel;
}

//initializer_list ctor
template <typename T>
my_list<T>::my_list(std::initializer_list<T> list) : sentinel(new NodeBase), msize(0) {
    sentinel->next = sentinel;
    sentinel->prev = sentinel;

    NodeBase* tmp = sentinel;

    for (const_reference x : list) {
        Node* node = new Node(x);
    
        tmp->next = node;
        node->prev = tmp;
        node->next = sentinel;
        sentinel->prev = node;

        tmp = node;
        ++msize;
    }
}



//destructor
template<typename T>
my_list<T>::~my_list() {
    clear();
    delete sentinel;
}


//operator =
template<typename T>
my_list<T>& my_list<T>::operator=(const my_list<T>& r) {
    if (this == &r) return *this;

    clear();
    NodeBase* left = sentinel;
    NodeBase* right = r.sentinel->next;

    while (right != r.sentinel) {
        Node* node = new Node((static_cast<Node*>(right))->value);

        left->next = node;
        node->prev = left;
        node->next = sentinel;
        sentinel->prev = node;

        left = node;
        right = right->next;
    }
    msize = r.msize;
    return *this;
}

template<typename T>
my_list<T>& my_list<T>::operator=(my_list<T>&& r) {
    if (this == &r) return *this;

    clear();
    if (r.msize != 0) {
        sentinel->next = r.sentinel->next;
        sentinel->prev = r.sentinel->prev;

        sentinel->next->prev = sentinel;
        sentinel->prev->next = sentinel;
        msize = r.msize;
    }
        r.sentinel->next = r.sentinel;
        r.sentinel->prev = r.sentinel;
        r.msize = 0;

    return *this;
}

template <typename T>
my_list<T>& my_list<T>::operator=(std::initializer_list<T> ilist) {

    clear();
    NodeBase* tmp = sentinel;

    for (const_reference x : ilist) {
        Node* node = new Node(x);

        tmp->next = node;
        node->prev = tmp;
        sentinel->prev = node;
        node->next = sentinel;

        tmp = node;

        ++msize;
    }
    return *this;
}


template<typename T>
void my_list<T>::clear() {
    NodeBase* dummy = sentinel->next;

    while(dummy != sentinel) {
        NodeBase* next = dummy->next;
        delete static_cast<Node*>(dummy);
        dummy = next;
    }

    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    msize = 0;
}
//operator<<
template<typename T>
std::ostream& operator<<(std::ostream& ost, const my_list<T>& l) {
        typename my_list<T>::NodeBase* tmp = l.sentinel->next;
        while(tmp != l.sentinel) {
            typename my_list<T>::Node* n = static_cast<typename my_list<T>::Node*>(tmp);
            ost << n->value << ' ';
            tmp = tmp->next;
        }
        return ost;
}
