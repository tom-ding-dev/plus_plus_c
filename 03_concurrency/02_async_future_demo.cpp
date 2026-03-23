#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include <stdexcept>

// 模拟耗时计算
int slow_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (x < 0) throw std::invalid_argument("Negative input!");
    return x * x;
}

// 无返回值任务（通过引用传递结果）
void slow_task(int id, int& result) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    result = id * 10;
}

int main() {
    using namespace std::chrono_literals;

    std::cout << "=== 1. 基本异步计算 ===\n";
    {
        auto fut = std::async(slow_square, 4);
        std::cout << "Async task launched. Doing other work...\n";
        int res = fut.get(); // 阻塞直到完成
        std::cout << "Result: " << res << "\n\n"; // 16
    }

    std::cout << "=== 2. 异常处理 ===\n";
    {
        auto fut = std::async(slow_square, -3);
        try {
            fut.get(); // 重新抛出异常
        } catch (const std::exception& e) {
            std::cout << "Caught exception: " << e.what() << "\n\n";
        }
    }

    std::cout << "=== 3. 超时控制 ===\n";
    {
        auto fut = std::async(std::launch::async, slow_square, 10);
        
        // 等待最多 300ms
        if (fut.wait_for(300ms) == std::future_status::ready) {
            std::cout << "Got result in time: " << fut.get() << "\n";
        } else {
            std::cout << "Timeout! Task still running.\n";
            // 注意：fut 析构会等待任务完成
        }
        std::cout << "\n";
    }

    std::cout << "=== 4. 并行多个任务 ===\n";
    {
        auto f1 = std::async(slow_square, 2);
        auto f2 = std::async(slow_square, 3);
        auto f3 = std::async(slow_square, 4);

        // 按任意顺序获取结果（实际已并行执行）
        std::cout << "Results: " 
                  << f1.get() << ", " 
                  << f2.get() << ", " 
                  << f3.get() << "\n\n"; // 4, 9, 16
    }

    std::cout << "=== 5. 启动策略对比 ===\n";
    {
        auto async_fut = std::async(std::launch::async, []{
            std::cout << "Async: Running in thread " 
                      << std::this_thread::get_id() << "\n";
            return 1;
        });

        auto deferred_fut = std::async(std::launch::deferred, []{
            std::cout << "Deferred: Running in thread " 
                      << std::this_thread::get_id() << "\n";
            return 2;
        });

        std::cout << "Main thread: " << std::this_thread::get_id() << "\n";
        async_fut.get();      // 已在新线程运行
        deferred_fut.get();   // 此刻才在主线程运行
        std::cout << "\n";
    }

    std::cout << "=== 6. 无返回值任务（通过引用） ===\n";
    {
        int result = 0;
        auto fut = std::async(std::launch::async, slow_task, 5, std::ref(result));
        fut.wait(); // 等待完成（不取返回值）
        std::cout << "Task result: " << result << "\n\n"; // 50
    }

    std::cout << "All demos completed successfully!\n";
    return 0;
}