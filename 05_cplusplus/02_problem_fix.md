# 命名规范

在现代 C++（C++11 及以后）项目中，**命名规范**（Naming Conventions）虽无强制标准（C++ 标准未规定），但业界已形成广泛共识。良好的命名能显著提升代码可读性、可维护性和团队协作效率。

以下基于 **Google C++ Style Guide**、**ISO C++ Core Guidelines**、**LLVM/Clang**、**Qt** 等主流项目的实践，总结 **C++ 现代项目推荐命名规范**。

---

## 📏 一、总体原则

| 原则 | 说明 |
|------|------|
| **清晰 > 简洁** | `user_count` 优于 `uc` |
| **一致性优先** | 全项目统一风格，勿混用 |
| **避免缩写** | 除非广为人知（`ID`, `URL`, `IO`） |
| **使用英文** | 避免拼音或非 ASCII 字符 |
| **体现类型/作用域** | 通过命名暗示生命周期、所有权等 |

---

## 🔠 二、具体命名规范（推荐）

### 1. **类名、结构体、枚举、类型别名** → **大驼峰**（PascalCase）
```cpp
class NetworkManager;          // ✅ 推荐
struct Point3D;
enum class FileMode;
using Callback = std::function<void()>;
```
> ❌ 避免：`network_manager`, `MyClass`

---

### 2. **变量名**（局部、成员、全局） → **小驼峰**（camelCase）
```cpp
int userCount = 0;             // 局部变量
std::string filePath;          // 参数
double maxVelocity;            // 成员变量（见下文变体）
```

#### ▶ 成员变量变体（三种主流风格，选一种并坚持）
| 风格 | 示例 | 优点 | 项目示例 |
|------|------|------|--------|
| **尾下划线** | `count_` | 清晰区分成员 vs 局部变量 | Google, LLVM |
| **m_ 前缀** | `m_count` | 显式标记成员 | Qt, Unreal Engine |
| **无标记** | `count` | 简洁，依赖 IDE 高亮 | Microsoft, C++ Core Guidelines |

✅ **推荐：尾下划线**（`_`），因：
- 不污染命名空间（`m_` 是匈牙利命名残余）；
- 与标准库风格一致（如 `std::vector::size_` 内部实现）；
- 避免与宏冲突（宏通常全大写）。

```cpp
class MyClass {
public:
    void setValue(int value) {
        value_ = value; // 成员 vs 参数一目了然
    }
private:
    int value_;         // ✅ 尾下划线
};
```

---

### 3. **常量**（`const` / `constexpr`）
#### ▶ 全局/静态常量 → **k + 大驼峰**
```cpp
constexpr int kMaxBufferSize = 1024;   // ✅ Google 风格
const double kPi = 3.14159;
```

#### ▶ 局部常量 → **小驼峰**
```cpp
void process() {
    const std::string defaultHost = "localhost"; // ✅
}
```

> 💡 为什么不用 `ALL_CAPS`？  
> `MAX_SIZE` 易与宏冲突（宏应全大写），且无法区分常量与宏。

---

### 4. **函数/方法名** → **小驼峰**（camelCase）
```cpp
void calculateTrajectory();
bool isValidEmail(const std::string& email);
```

#### ▶ 布尔函数前缀：`is`, `has`, `can`, `should`
```cpp
bool isEmpty() const;
bool hasPermission() const;
```

---

### 5. **命名空间** → **全小写 + 下划线**（可选）
```cpp
namespace graphics_engine;
namespace file_io;
// 或简写（若无歧义）
namespace gfx;
namespace net;
```
> ⚠️ 避免嵌套过深：`myproject::utils::io::file` → 考虑扁平化。

---

### 6. **宏** → **全大写 + 下划线**
```cpp
#define MAX_CONNECTIONS 100
#define LOG_ERROR(msg) ...
```
> 🚫 **尽量避免宏**！用 `constexpr`、`inline`、`template` 替代。

---

### 7. **模板参数** → **大驼峰 + T 后缀**（可选）
```cpp
template<typename T>
template<typename ValueType>
template<typename AllocatorT>   // 明确是类型
```
> ISO C++ Core Guidelines 推荐：**`T`, `U` 用于简单场景；语义名用于复杂模板**。

---

## 🚫 三、应避免的命名反模式

| 反模式 | 问题 | 正确做法 |
|--------|------|--------|
| `int l = 10;` | `l` 与 `1` 难区分 | `int length = 10;` |
| `void DoSomething();` | 混合大小写不一致 | `void doSomething();` |
| `#define PI 3.14` | 宏无作用域，易冲突 | `constexpr double kPi = 3.14;` |
| `bool flag;` | 无业务含义 | `bool isConnected;` |
| `class my_class;` | 下划线风格不统一 | `class MyClass;` |

---

## 🧩 四、特殊场景命名建议

### 1. **RAII 资源管理类**
- 名称体现资源类型：
  ```cpp
  class FileHandle;      // 文件句柄
  class SocketGuard;     // 自动关闭 socket
  class LockGuard;       // 互斥锁（标准库已有）
  ```

### 2. **工厂函数**
- 前缀 `make_` 或 `create`：
  ```cpp
  std::unique_ptr<NetworkClient> makeHttpClient();
  std::shared_ptr<Task> createBackgroundTask();
  ```

### 3. **测试代码**
- 测试类：`FooTest`
- 测试函数：`TEST(FooTest, HandlesEmptyInput)`
- （遵循 Google Test 风格）

---

## 🛠️ 五、工具支持

| 工具 | 作用 |
|------|------|
| **clang-format** | 自动格式化（含命名检查需配合 clang-tidy） |
| **clang-tidy** | 检查命名违规（如 `readability-identifier-naming`） |
| **EditorConfig** | 统一团队编辑器设置 |

#### `.clang-tidy` 示例（强制命名规则）
```yaml
Checks: >
  readability-identifier-naming,
  ...

CheckOptions:
  - key: readability-identifier-naming.ClassCase
    value: CamelCase
  - key: readability-identifier-naming.VariableCase
    value: camelBack
  - key: readability-identifier-naming.MemberSuffix
    value: _
```

---

## ✅ 六、总结：现代 C++ 命名速查表

| 实体 | 推荐风格 | 示例 |
|------|--------|------|
| 类 / 结构体 / 枚举 | PascalCase | `HttpRequest`, `Point2D` |
| 函数 / 方法 / 变量 | camelCase | `processData`, `userCount` |
| 成员变量 | camelCase + `_` | `bufferSize_` |
| 全局常量 | `k` + PascalCase | `kDefaultTimeout` |
| 命名空间 | lowercase | `network`, `file_system` |
| 宏 | UPPER_SNAKE_CASE | `MAX_THREADS` |

> 💡 **终极心法**：  
> **“命名即文档”** —— 优秀的命名让代码自解释，减少注释依赖。  
> **“一致性胜过个人偏好”** —— 团队约定 > 个人习惯。

选择一套规范（如 Google 或 LLVM），用工具自动化 enforcement，你的 C++ 项目将更专业、更易维护。

# 在软件工程实践上的不足之处

火箭信息采集系统。缺点与问题。作为一个典型的嵌入式系统信息采集模块，功能完整，但设计上存在明显改进空间。

1、类的设计过于简单

```
class RocketSysteminfo {
public:
    int method();
private:
    int variable_;
}
```

重构版本

```
#pragma once

class RocketSystemInfo {
public:
    int getCurrentThrust() const;
    void setFuelLevel(int level);

private:
    int thrustLevel_ = 0;      // 初始化默认值（C++11+）
    int fuelLevel_ = 100;
};
```

# 笑死

只有一个公共方法的类，没有任何成员变量、构造函数、析构函数，这样的类设计你们见过么？》

在数据封装上，所有的实际数据都存储在全局变量中。

在头文件中，应当暴露的接口、常量、结构体都没有定义。

全局变量滥用，破坏了封装性，增加了代码耦合。多线程环境下不安全，虽然我这也不是多线程应用程序。难以进行单元测试。

重复代码严重，在函数中存在大量重复模式。

全局变量问题通过 依赖注入、单例模式 解决。

重试的重复逻辑 通过 模板方法模式解决

多个数据获取逻辑相似的问题 通过 策略模式 + 工厂模式 解决。

JSON解析职责过重的问题 通过 解析器模式解决。


