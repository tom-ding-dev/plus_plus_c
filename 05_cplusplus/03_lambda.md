# 防止虚假唤醒生产者线程

{
  std::unique_lock lock(mutex_);
  cv.wait(lock, [this] {
    return !paused_ || !running_;
  };
}

这个 return 是 Lambda 表达式的一部分，不是普通的函数返回。

为什么需要 return？

因为 wait() 方法的第二个参数要求一个 可调用对象，这个对象必须：

不接受参数（或者接受特定参数）
返回 bool 类型
返回 true 表示"条件满足，可以继续"
返回 false 表示"条件不满足，继续等待"

1. 线程调用 wait() 开始等待
2. 等待期间，如果有其他线程调用：
   - pause_condition_.notify_one() 或
   - pause_condition_.notify_all()
3. wait() 被唤醒，但不是立即继续
4. 而是先调用 Lambda 函数检查条件
   ↓
   Lambda 返回 true/false
   ↓
5. 如果返回 true：结束等待，继续执行循环
6. 如果返回 false：重新进入等待状态

关键点总结：
return 是 Lambda 表达式的返回值
这个值告诉 wait() "是否应该结束等待"
条件是：解除暂停 或者 完全停止
避免虚假唤醒（spurious wakeup）：即使被唤醒了，也要检查条件
所以这个 return 不是跳出函数，而是提供等待条件的判断结果，这是 C++ 条件变量 wait() 方法的特殊设计。


