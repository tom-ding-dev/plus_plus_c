#include <iostream>
#include <string>

class BankAccount {
private:
    std::string owner;
    double balance = 0.0;           // 默认初始化
    static int totalAccounts;       // 静态成员：所有账户总数

public:
    // 构造函数（含默认参数）
    BankAccount(const std::string& name, double init = 0.0) 
        : owner(name), balance(init) {
        ++totalAccounts;
        std::cout << "Account created for " << owner << "\n";
    }

    // 析构函数
    ~BankAccount() {
        --totalAccounts;
        std::cout << "Account destroyed for " << owner << "\n";
    }

    // 普通成员函数
    void deposit(double amount) {
        if (amount > 0) balance += amount;
    }

    // const 成员函数（承诺不修改对象）
    double getBalance() const { return balance; }
    std::string getOwner() const { return owner; }

    // 静态成员函数（无 this 指针）
    static int getTotalAccounts() { return totalAccounts; }

    // 运算符重载（成员函数）
    BankAccount& operator+=(double amount) {
        deposit(amount);
        return *this;
    }

    // 友元函数（可访问 private 成员）
    friend bool operator==(const BankAccount& a, const BankAccount& b);
};

// 静态成员定义（必须在类外）
int BankAccount::totalAccounts = 0;

// 友元函数实现
bool operator==(const BankAccount& a, const BankAccount& b) {
    return a.owner == b.owner && a.balance == b.balance;
}

// 演示函数
void demo() {
    std::cout << "Total accounts: " << BankAccount::getTotalAccounts() << "\n\n";

    {
        BankAccount acc1("Alice", 100.0);  // 调用构造
        BankAccount acc2("Bob");           // 使用默认参数

        acc1 += 50.0;                      // 运算符重载
        acc2.deposit(200.0);

        std::cout << acc1.getOwner() << "'s balance: $" << acc1.getBalance() << "\n";
        std::cout << "Accounts equal? " << (acc1 == acc2 ? "Yes" : "No") << "\n";
        std::cout << "Total accounts: " << BankAccount::getTotalAccounts() << "\n";
    } // 作用域结束 → 自动调用析构函数

    std::cout << "\nAfter scope: Total accounts: " 
              << BankAccount::getTotalAccounts() << "\n";
}

int main() {
    demo();
    return 0;
}