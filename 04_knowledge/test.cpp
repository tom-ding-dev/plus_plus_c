#include <iostream>
#include <cstring>
#include <vector>
#include <utility> // for std::move
// main.cpp
#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>
#include <cstring>

#include <iostream>
#include <type_traits>
#include <utility>

class MyString {
private:
    char* data_;
    size_t size_;

public:
    // 构造函数
    explicit MyString(const char* str = "") 
        : size_(std::strlen(str)), 
          data_(new char[size_ + 1]) {
        std::strcpy(data_, str);
        std::cout << "Constructed: " << data_ << "\n";
    }

    // 拷贝构造函数（深拷贝）
    MyString(const MyString& other) 
        : size_(other.size_), 
          data_(new char[size_ + 1]) {
        std::strcpy(data_, other.data_);
        std::cout << "Copied: " << data_ << "\n";
    }

    // ✅ 移动构造函数（关键！）
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        // "偷走"资源
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "Moved: " << (data_ ? data_ : "(null)") << "\n";
    }

    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
            std::cout << "Copy-assigned: " << data_ << "\n";
        }
        return *this;
    }

    // ✅ 移动赋值运算符（关键！）
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;      // 释放自身资源
            data_ = other.data_; // 偷走对方资源
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "Move-assigned: " << (data_ ? data_ : "(null)") << "\n";
        }
        return *this;
    }

    // 析构函数
    ~MyString() {
        delete[] data_;
        std::cout << "Destroyed: " << (data_ ? data_ : "(null)") << "\n";
    }

    // 工具函数
    const char* c_str() const { return data_ ? data_ : ""; }
    size_t size() const { return size_; }
};

// 测试函数
void test_move_semantics() {
    std::cout << "\n=== 测试 1: 返回值优化 (RVO) ===\n";
    auto s1 = MyString("Hello"); // 构造

    std::cout << "\n=== 测试 2: 显式移动 ===\n";
    MyString s2 = std::move(s1); // 移动构造（s1 现在为空）

    std::cout << "\n=== 测试 3: 容器中的移动 ===\n";
    std::vector<MyString> vec;
    vec.push_back(MyString("World")); // 临时对象 → 移动构造

    std::cout << "\n=== 测试 4: 移动赋值 ===\n";
    MyString s3("C++");
    s3 = std::move(vec[0]); // 移动赋值

    std::cout << "\n程序结束\n";
}


// [在此处插入 MyString 类和测试函数]

int main() {
    test_move_semantics();
    // test_universal_references();
    return 0;
}
