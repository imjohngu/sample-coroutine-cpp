#ifndef MODERN_COROUTINE_HPP
#define MODERN_COROUTINE_HPP

#include <coroutine>
#include <exception>
#include <concepts>
#include <type_traits>
#include <utility>
#include <memory>

namespace modern_coroutine {

// 基础的Task类型，用于表示一个协程任务
template<typename T = void>
class [[nodiscard]] Task {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T result;
        std::exception_ptr exception;

        Task get_return_object() noexcept {
            return Task{handle_type::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        template<typename U>
            requires std::convertible_to<U, T>
        void return_value(U&& value) noexcept(std::is_nothrow_constructible_v<T, U>) {
            result = std::forward<U>(value);
        }

        void unhandled_exception() noexcept {
            exception = std::current_exception();
        }
    };

    Task(handle_type h) noexcept : handle_(h) {}
    ~Task() {
        if (handle_) handle_.destroy();
    }

    // 禁用拷贝
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    // 允许移动
    Task(Task&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr;
    }
    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    // 恢复协程执行
    bool resume() {
        if (!handle_ || handle_.done()) return false;
        handle_.resume();
        return !handle_.done();
    }

    // 获取协程结果
    T get_result() {
        if (!handle_) throw std::runtime_error("Task has no valid handle");
        if (!handle_.done()) throw std::runtime_error("Task not completed");
        if (handle_.promise().exception)
            std::rethrow_exception(handle_.promise().exception);
        return std::move(handle_.promise().result);
    }

private:
    handle_type handle_;
};

// Task<void>的特化版本
template<>
class Task<void> {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        std::exception_ptr exception;

        Task get_return_object() noexcept {
            return Task{handle_type::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}

        void unhandled_exception() noexcept {
            exception = std::current_exception();
        }
    };

    Task(handle_type h) noexcept : handle_(h) {}
    ~Task() {
        if (handle_) handle_.destroy();
    }

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr;
    }
    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    bool resume() {
        if (!handle_ || handle_.done()) return false;
        handle_.resume();
        return !handle_.done();
    }

    void get_result() {
        if (!handle_) throw std::runtime_error("Task has no valid handle");
        if (!handle_.done()) throw std::runtime_error("Task not completed");
        if (handle_.promise().exception)
            std::rethrow_exception(handle_.promise().exception);
    }

private:
    handle_type handle_;
};

} // namespace modern_coroutine

#endif // MODERN_COROUTINE_HPP