#include <iostream>
#include <coroutine>


//计算斐波那契数列的值 f(n) = f(n-1) + f(n-2)  n==0 -> 1 n==1 -> 1
//无栈协程，即不需要函数调用栈，那么需要保存函数的执行状态。

struct fib_frame{
    int a;  //中间结果 f(n-1)
    int b;  //中间结果 f(n-2)
    int c; // 中间结果 f(n)
    int state = 0; //当前状态

    //resume函数，返回当前的结果
    int resume(){
        switch(state){
            case 0:
                a = b = 1;
                state = 1;
                return a;
            case 1:
                state = 2;
                return b;
            case 2:
                c = a + b;
                a = b;
                b = c;
                state = 3;
                return c;
            case 3:
                c = a + b;
                a = b;
                b = c;
                return c;
        }

    }
};

int main(int argc, char **argv){
    fib_frame f;
    for (int i = 0; i < 10; i++){
        int x = f.resume();
         std:: cout << x << std:: endl;
    }
     return 0;
}