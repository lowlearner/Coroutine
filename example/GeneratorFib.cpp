#include "Generator.h"
#include <tuple>
#include <iostream>

Generator fib(){
    int a = 1, b = 1;
    while (a < 1000) { co_yield a;
        std::tie(a, b) = std::make_tuple(b, a + b);
    }
    co_return;
}


void test_fib(){
    for (auto f = fib(); !f.done(); f.next()){
        std::cout << f.current_value() << std::endl;
    } 
}


int main(){
    test_fib();

    return 0;
}