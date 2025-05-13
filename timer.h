#pragma once
#include <chrono>
#include <functional>

class Timer final
{
public:
    Timer()
            : _start(std::chrono::high_resolution_clock::now())
    {
    }

    ~Timer() = default;

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(Timer&&) = delete;

    double get_time() const
    {
        return std::chrono::duration<double, std::micro>(
                std::chrono::high_resolution_clock::now() - _start).count();
    }

    template <typename Func, typename... Args>
    static double time_function(Func&& func, Args&&... args)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::micro>(end - start).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};
