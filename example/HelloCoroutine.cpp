#include <coroutine>
#include <iostream>

using namespace std;

struct HelloCoroutine {
    struct promise_type {
        promise_type(){
            cout <<"Construct promise" << endl;
        }
        ~promise_type(){
            cout <<"Destroy promise" << endl;
        }
        suspend_never initial_suspend() {
            cout << "Initial suspend" << endl;
            return {};
        }
        suspend_never final_suspend() noexcept {
            cout << "Final suspend" << endl;
            return {};
        }
        void unhandled_exception() noexcept {
            cout << "Unhandled exception" << endl;
        }
        HelloCoroutine get_return_object() {
            cout << "Return object" << endl;
            return {};
        }
    };
};

HelloCoroutine hello(string &message) {
    cout << "Hello" << endl;
    co_await suspend_never();
    cout << message << endl;
}

int main() {
    cout << "========" << endl;
    string message("world");
    HelloCoroutine h = hello(message);
    cout << "========" << endl;

    return 0;
}