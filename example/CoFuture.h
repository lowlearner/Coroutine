/// 针对future<T>类，进行coroutine_traits特化，以支持协程

#include <future>
#include <vector>
#include <coroutine>
#include <numeric>

// 特化coroutine_traits，使future支持协程
struct AsCoroFuture {};

template<typename T, typename... Args>
struct std::coroutine_traits<std::future<T>,AsCoroFuture, Args...> {
    struct promise_type: std::promise<T> {
        std::future<T> get_return_object() { return this->get_future(); };
        auto initial_suspend() noexcept { return std::suspend_never(); }
        auto final_suspend() noexcept { return std::suspend_never(); }
        void unhandled_exception() {
            this->set_exception(std::current_exception());
        }
        template<typename U>
        void return_value(U&& value){
            this->set_value(std::forward<U>(value));
        }

        struct Awaiter{
            bool await_ready() { return fut_.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }
            void await_suspend(std::coroutine_handle<> handle){
                std::thread([=, this]() {
                    fut_.wait();
                    handle.resume();
                }).detach();
            }

            decltype(auto) await_resume(){
                return fut_.get();
            }

            std::future<T> fut_;
        };

        Awaiter await_transform(std::future<T> fut) { return {std::move(fut)}; }
    };
};

// 并行求和
template<std::random_access_iterator RandIt>
std::future<int> parallel_sum(AsCoroFuture a,RandIt beg, RandIt end) {
    auto len = end - beg;
    if (len == 0) { co_return 0; }
    RandIt mid = beg + len / 2;
    auto rest_task = std::async(
        [](RandIt b, RandIt e) { return std::accumulate(b, e, 0); }, mid, end);
    auto first_task = parallel_sum(a, beg, mid);

    auto first = co_await std::move(first_task); // 协程挂起
    auto rest = co_await std::move(rest_task);   //
    co_return first + rest;
}

int test_parallel_sum(AsCoroFuture a, std::vector<int> &v) {
    return parallel_sum(a, v.begin(), v.end()).get();
}
