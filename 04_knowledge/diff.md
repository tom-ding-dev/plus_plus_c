以下是 **C++ 语法知识点的系统性分类大纲详解**，覆盖从基础到高级的核心内容，适用于学习路线规划、面试准备或知识体系梳理。本大纲基于 **现代 C++**（C++11/14/17/20），强调实用性与工程规范。

---

## 📚 一、基础语法（C++ Core）

### 1.1 基本结构
- 程序入口：`main()` 函数
- 头文件与预处理：`#include`, `#define`, 条件编译
- 命名空间：`namespace`, `using`
- 注释：`//`, `/* */`

### 1.2 数据类型
| 类别 | 类型 |
|------|------|
| **基本类型** | `int`, `double`, `bool`, `char`, `void` |
| **复合类型** | 数组、结构体（`struct`）、联合（`union`） |
| **指针与引用** | `T*`, `T&`, `T&&`（右值引用） |
| **类型别名** | `typedef`, `using T = int;` |

### 1.3 变量与常量
- 初始化：`{}`, `=`, `()`（统一初始化）
- 存储类别：`auto`, `static`, `thread_local`
- 常量：`const`, `constexpr`（编译期常量）

### 1.4 运算符
- 算术：`+ - * / %`
- 关系：`== != < > <= >=`
- 逻辑：`&& || !`
- 位运算：`& | ^ ~ << >>`
- 赋值：`= += -=`
- 其他：`?:`, `sizeof`, `typeid`, `->`, `.*`

### 1.5 控制流
- 条件：`if-else`, `switch-case`
- 循环：`for`, `while`, `do-while`
- 跳转：`break`, `continue`, `return`, `goto`（慎用）

---

## 🧱 二、面向对象编程（OOP）

### 2.1 类与对象
- 定义：`class`, `struct`（默认访问权限差异）
- 成员：数据成员、成员函数
- 访问控制：`public`, `private`, `protected`

### 2.2 构造与析构
- 构造函数：默认、带参、委托构造（C++11）
- 初始化列表：`ClassName() : member(value) {}`
- 析构函数：`~ClassName()`
- 特殊成员函数（Rule of Five）：
  - 拷贝构造
  - 拷贝赋值
  - 移动构造（C++11）
  - 移动赋值（C++11）
  - 析构函数

### 2.3 继承与多态
- 继承：`class Derived : public Base`
- 虚函数：`virtual`, `override`, `final`
- 抽象类：纯虚函数 `= 0`
- 多重继承（谨慎使用）

### 2.4 运算符重载
- 成员 vs 非成员函数
- 常见重载：`[]`, `()`, `<<`, `+`, `=`
- 移动语义支持：`operator=(T&&)`

---

## 🧩 三、模板与泛型编程

### 3.1 函数模板
```cpp
template<typename T>
T max(T a, T b) { return a > b ? a : b; }
```

### 3.2 类模板
```cpp
template<typename T, size_t N>
class Array { T data[N]; };
```

### 3.3 高级模板特性
- **SFINAE**：`std::enable_if`
- **概念**（Concepts, C++20）：约束模板参数
- **变参模板**（Variadic Templates）：
  ```cpp
  template<typename... Args>
  void print(Args... args);
  ```

### 3.4 模板特化
- 全特化：`template<> class MyClass<int> {...}`
- 偏特化：仅用于类模板

---

## ⚡ 四、现代 C++ 核心特性（C++11+）

### 4.1 自动类型推导
- `auto`：变量类型推导
- `decltype`：表达式类型推导

### 4.2 智能指针（RAII）
| 指针类型 | 用途 |
|----------|------|
| `std::unique_ptr` | 独占所有权 |
| `std::shared_ptr` | 共享所有权 |
| `std::weak_ptr` | 打破循环引用 |

### 4.3 移动语义
- 右值引用：`T&&`
- `std::move()`
- 完美转发：`std::forward<T>()`

### 4.4 Lambda 表达式
```cpp
auto lambda = [](int x) { return x * 2; };
```
- 捕获列表：`[=]`, `[&]`, `[this]`, `[x, &y]`

### 4.5 新容器与工具
- `std::array`, `std::unordered_map`
- `std::optional`（C++17）, `std::variant`（C++17）
- `std::string_view`（C++17）

---

## 🧵 五、并发与多线程（C++11+）

### 5.1 线程管理
- `std::thread`
- `join()`, `detach()`

### 5.2 同步原语
| 工具 | 用途 |
|------|------|
| `std::mutex` | 互斥锁 |
| `std::lock_guard` | RAII 锁 |
| `std::atomic` | 原子操作 |
| `std::condition_variable` | 条件变量 |

### 5.3 高级并发
- `std::async`, `std::future`, `std::promise`
- 内存模型：`memory_order`

---

## 📦 六、标准库（STL）核心组件

### 6.1 容器
| 类型 | 示例 |
|------|------|
| **序列容器** | `vector`, `deque`, `list` |
| **关联容器** | `set`, `map`, `unordered_set` |
| **容器适配器** | `stack`, `queue`, `priority_queue` |

### 6.2 算法
- 非修改：`find`, `count`
- 修改：`sort`, `transform`, `copy`
- 数值：`accumulate`, `inner_product`

### 6.3 迭代器
- 五类迭代器：输入/输出/前向/双向/随机访问
- 范围 for：`for (auto& x : container)`

### 6.4 函数对象
- 函数指针
- `std::function`
- `std::bind`

---

## 🔒 七、内存管理

### 7.1 动态内存
- `new`/`delete`, `new[]`/`delete[]`
- **禁止混用**：`malloc`/`free` 与 `new`/`delete`

### 7.2 RAII 原则
- 资源获取即初始化
- 智能指针替代裸指针

### 7.3 自定义分配器
- `std::allocator`
- 内存池设计

---

## 🛠️ 八、错误处理

### 8.1 异常处理
- `try`, `catch`, `throw`
- 异常安全保证：无异常、强保证、基本保证

### 8.2 错误码（替代方案）
- `std::error_code`
- 返回 `std::optional` 或 `expected`（C++23）

---

## 🧪 九、元编程与编译期计算

### 9.1 constexpr 编程
- `constexpr` 函数与变量
- 编译期循环与条件（C++14+）

### 9.2 类型特征（Type Traits）
- `std::is_integral<T>`
- `std::enable_if`
- SFINAE 应用

### 9.3 模板元编程（TMP）
- 递归模板
- 编译期计算（如阶乘）

---

## 📈 十、工程实践与最佳规范

### 10.1 代码风格
- Google C++ Style Guide
- 命名约定：`CamelCase`, `snake_case`

### 10.2 编译与构建
- CMake 基础
- 静态/动态库

### 10.3 调试与测试
- GDB/LLDB 调试
- 单元测试：Google Test

### 10.4 性能优化
- 避免不必要的拷贝
- 移动语义应用
- 缓存友好设计

---

## 🌐 十一、C++20/23 新特性（前瞻）

| 特性 | 说明 |
|------|------|
| **模块**（Modules） | 替代头文件 |
| **协程**（Coroutines） | 异步编程 |
| **范围**（Ranges） | STL 算法现代化 |
| **format** | 安全格式化（`std::format`） |

---

## 💎 总结：学习路径建议

1. **基础 → OOP → STL**（掌握 C++98 核心）
2. **智能指针 → 移动语义 → Lambda**（拥抱 C++11）
3. **并发 → 模板进阶 → 元编程**（深入现代 C++）
4. **工程实践 → 性能调优 → 新标准**（生产级能力）

> 📌 **核心思想**：  
> **“用 RAII 管理资源，用移动语义避免拷贝，用模板实现泛型，用并发提升性能。”**

此大纲可作为 **C++ 知识地图**，根据实际需求深入各模块。如果需要 **某一部分的详细展开**（如“移动语义实战”或“并发模型详解”），欢迎指定！ 😊
