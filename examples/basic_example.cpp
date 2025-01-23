#include <modern_coroutine/coroutine.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace modern_coroutine;

// 模拟一个异步操作
Task<int> async_operation(int value) {
    // 模拟一些耗时的工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    co_return value * 2;
}

// 组合多个异步操作
Task<int> combined_operations() {
    auto task1 = async_operation(10);
    while (task1.resume()) {}
    int result1 = task1.get_result();

    auto task2 = async_operation(result1);
    while (task2.resume()) {}
    int result2 = task2.get_result();

    co_return result2;
}

// 演示void类型的Task
Task<void> print_result(int value) {
    std::cout << "最终结果: " << value << std::endl;
    co_return;
}

int main() {
    std::cout << "开始执行异步操作..." << std::endl;

    auto combined_task = combined_operations();
    while (combined_task.resume()) {}
    int final_result = combined_task.get_result();

    auto print_task = print_result(final_result);
    while (print_task.resume()) {}
    print_task.get_result();

    return 0;
}