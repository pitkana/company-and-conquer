#include <chrono>
#include <iostream>

class Timer {
public:
    Timer() {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start_).time_since_epoch().count();
        auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();

        //auto duration = end_time - start_time;
        //std::cout << "Duration: " << duration * 0.001 << " ms" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};
