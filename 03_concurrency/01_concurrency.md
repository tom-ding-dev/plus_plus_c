#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <vector>
#include <queue>
#include <chrono>

std::mutex cout_mtx; // 保护 std::cout

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(cout_mtx);
    std::cout << msg << "\n";
}

// 1. 基础线程 + 互斥锁
void thread_demo() {
    safe_print("=== Thread + Mutex Demo ===");
    int counter = 0;
    std::mutex mtx;

    auto worker = [&](int id) {
        for (int i = 0; i < 3; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            ++counter;
            safe_print("Thread " + std::to_string(id) + 
                      ": counter = " + std::to_string(counter));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i)
        threads.emplace_back(worker, i);
    
    for (auto& t : threads) t.join();
}

// 2. 条件变量：生产者-消费者
void condvar_demo() {
    safe_print("\n=== Condition Variable Demo ===");
    std::queue<int> buffer;
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

    auto producer = [&]() {
        for (int i = 1; i <= 3; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push(i);
            safe_print("Produced: " + std::to_string(i));
            cv.notify_one();
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    };

    auto consumer = [&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]() { return !buffer.empty() || done; });
            if (done && buffer.empty()) break;
            int val = buffer.front(); buffer.pop();
            lock.unlock();
            safe_print("Consumed: " + std::to_string(val));
        }
    };

    std::thread p(producer), c(consumer);
    p.join(); c.join();
}

// 3. 异步任务 + future
void async_demo() {
    safe_print("\n=== Async + Future Demo ===");
    auto task = [](int x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return x * x;
    };

    auto future1 = std::async(std::launch::async, task, 5);
    auto future2 = std::async(std::launch::async, task, 10);

    safe_print("Result 1: " + std::to_string(future1.get()));
    safe_print("Result 2: " + std::to_string(future2.get()));
}

int main() {
    thread_demo();
    condvar_demo();
    async_demo();
    std::cout << "\nAll concurrency demos completed!\n";
    return 0;
}

```
=== Thread + Mutex Demo ===
Thread 0: counter = 1
Thread 0: counter = 2
Thread 0: counter = 3
Thread 2: counter = 4
Thread 1: counter = 5
Thread 1: counter = 6
Thread 1: counter = 7
Thread 2: counter = 8
Thread 2: counter = 9

=== Condition Variable Demo ===
Produced: 1
Consumed: 1
Produced: 2
Produced: 3
Consumed: 2
Consumed: 3

=== Async + Future Demo ===
Result 1: 25
Result 2: 100

All concurrency demos completed!


=== Code Execution Successful ===
```