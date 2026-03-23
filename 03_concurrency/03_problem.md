# code 

```
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

const int MAX_QUEUE_SIZE = 5;
std::queue<int> task_queue;
std::mutex mtx;
std::condition_variable cv;

// 新增：全局结束标志
bool finished = false;  // 受 mtx 保护

void producer() {
    for (int i = 1; i <= 20; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return task_queue.size() < MAX_QUEUE_SIZE; });
        task_queue.push(i);
        std::cout << "Produced: " << i << "\n";
        lock.unlock();
        cv.notify_all(); // 或 notify_one()
    }

    // 关键：生产完成后，设置结束标志
    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all(); // 唤醒所有等待的消费者
}

void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // 等待：直到“有任务” OR “生产结束”
        cv.wait(lock, []{ 
            return !task_queue.empty() || finished; 
        });

        // 如果队列空 且 已结束 → 退出
        if (task_queue.empty() && finished) {
            std::cout << "Consumer " << id << " exiting.\n";
            break;
        }

        // 否则处理任务
        int task = task_queue.front();
        task_queue.pop();
        lock.unlock();

        std::cout << "Consumer " << id << " consumed: " << task << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}


int main() {
    std::thread p(producer);
    std::thread c1(consumer, 1);
    std::thread c2(consumer, 2);

    p.join();
    c1.join();
    c2.join();
    std::cout << "game over" << std::endl; 
    return 0;
}
```


