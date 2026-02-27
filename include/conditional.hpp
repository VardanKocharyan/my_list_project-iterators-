#pragma once 

template <bool B, class T, class F>
struct conditional { using type = T; };

template <class T, class F>
struct conditional<false, T, F> { using type = F; };

template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;
