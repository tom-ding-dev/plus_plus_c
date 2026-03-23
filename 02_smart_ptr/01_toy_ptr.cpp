#include <iostream>
#include <memory>
#include <vector>

class Resource {
public:
    Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " created\n";
    }
    ~Resource() {
        std::cout << "Resource " << id_ << " destroyed\n";
    }
    void use() const { std::cout << "Using Resource " << id_ << "\n"; }

private:
    int id_;
};

void demo_unique_ptr() {
    std::cout << "\n--- unique_ptr demo ---\n";
    auto ptr1 = std::make_unique<Resource>(1);
    ptr1->use();

    // 转移所有权（不能复制）
    auto ptr2 = std::move(ptr1);
    if (!ptr1) std::cout << "ptr1 is null after move\n";
    ptr2->use();
} // ptr2 析构 → Resource 1 销毁

void demo_shared_ptr() {
    std::cout << "\n--- shared_ptr demo ---\n";
    auto sp1 = std::make_shared<Resource>(2);
    {
        auto sp2 = sp1; // 引用计数 +1
        std::cout << "Ref count: " << sp1.use_count() << "\n";
        sp2->use();
    } // sp2 析构 → 引用计数 -1
    std::cout << "Ref count: " << sp1.use_count() << "\n";
    sp1->use();
} // sp1 析构 → Resource 2 销毁

void demo_weak_ptr() {
    std::cout << "\n--- weak_ptr demo (break cyclic ref) ---\n";
    struct Node;
    using NodePtr = std::shared_ptr<Node>;
    using WeakNodePtr = std::weak_ptr<Node>;

    struct Node {
        int id;
        NodePtr next;
        WeakNodePtr prev; // 用 weak_ptr 避免循环引用

        Node(int i) : id(i) { std::cout << "Node " << id << " created\n"; }
        ~Node() { std::cout << "Node " << id << " destroyed\n"; }
    };

    auto n1 = std::make_shared<Node>(1);
    auto n2 = std::make_shared<Node>(2);

    n1->next = n2;
    n2->prev = n1; // weak_ptr 不增加引用计数

    std::cout << "n1 ref count: " << n1.use_count() << "\n"; // 1
    std::cout << "n2 ref count: " << n2.use_count() << "\n"; // 1

    // 安全访问 weak_ptr
    if (auto locked = n2->prev.lock()) {
        std::cout << "n2's prev is Node " << locked->id << "\n";
    }
} // 无循环引用 → 正常析构

int main() {
    demo_unique_ptr();
    demo_shared_ptr();
    demo_weak_ptr();
    std::cout << "\nAll resources cleaned up!\n";
    return 0;
}

/*

--- unique_ptr demo ---
Resource 1 created
Using Resource 1
ptr1 is null after move
Using Resource 1
Resource 1 destroyed

--- shared_ptr demo ---
Resource 2 created
Ref count: 2
Using Resource 2
Ref count: 1
Using Resource 2
Resource 2 destroyed

--- weak_ptr demo (break cyclic ref) ---
Node 1 created
Node 2 created
n1 ref count: 1
n2 ref count: 2
n2's prev is Node 1
Node 1 destroyed
Node 2 destroyed

All resources cleaned up!


=== Code Execution Successful ===
*/