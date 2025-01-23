# Modern Coroutine Library

一个现代C++协程库，提供简单易用的异步编程接口。

## 特性

- 基于C++20协程标准实现
- 提供Task<T>模板类支持异步操作
- 支持异常处理和错误传播
- 简洁的API设计
- 支持void返回类型的协程

## 要求

- C++20或更高版本
- 支持协程的编译器（如GCC 10+、Clang 11+、MSVC 2019+）
- CMake 3.15或更高版本

## 使用方法

### 基本示例

```cpp
#include <modern_coroutine/coroutine.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace modern_coroutine;

// 创建一个异步操作
Task<int> async_operation(int value) {
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

int main() {
    auto task = combined_operations();
    while (task.resume()) {}
    std::cout << "结果: " << task.get_result() << std::endl;
    return 0;
}
```

## 构建和运行

```bash

# 创建构建目录
mkdir build && cd build

# 配置和构建
cmake ..
make

# 运行示例
./examples/basic_example
```
