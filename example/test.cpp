#include "CoFuture.h"
#include <vector>
#include <iostream>


int main(){
    std::vector<int> v(100000000, 1);
    AsCoroFuture a;
    std::cout << test_parallel_sum(a, v) << std::endl;

    return 0;
}