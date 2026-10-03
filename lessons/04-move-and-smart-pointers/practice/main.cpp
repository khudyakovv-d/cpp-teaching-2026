#include "vector.h"

#include <iostream>
#include <memory>
#include <utility>

void consume(std::unique_ptr<Vector> values) {
    if (values && values->size() > 0) {
        std::cout << "consumed: " << (*values)[0] << '\n';
    }
}

struct Node {
    int id;
    std::shared_ptr<Node> child;
    std::weak_ptr<Node> parent;

    explicit Node(int value) : id(value) {}

    ~Node() {
        std::cout << "destroy node " << id << '\n';
    }
};

int main() {
    std::cout << std::boolalpha;

    Vector source(2);
    source[0] = 10;
    source[1] = 20;
    Vector copy = source;
    copy[0] = 99;
    std::cout << "independent copy: " << source[0] << ' ' << copy[0] << '\n';

    int* array = &source[0];
    Vector moved = std::move(source);
    std::cout << "move constructor: source size = " << source.size()
              << ", same array = " << (array == &moved[0]) << '\n';

    Vector assigned(5);
    assigned = std::move(moved);
    std::cout << "move assignment: source size = " << moved.size()
              << ", same array = " << (array == &assigned[0]) << '\n';
    source = Vector(1);
    source[0] = 7;
    std::cout << "reused source: " << source[0] << '\n';

    auto owner = std::make_unique<Vector>(2);
    (*owner)[0] = 42;
    Vector* object = owner.get();
    auto next = std::move(owner);
    std::cout << "unique move: source empty = " << (owner == nullptr)
              << ", same object = " << (object == next.get()) << '\n';
    consume(std::move(next));
    std::cout << "after consume: empty = " << (next == nullptr) << '\n';

    auto first = std::make_shared<Node>(1);
    auto second = first;
    std::weak_ptr<Node> observer = first;
    first.reset();
    {
        auto current = observer.lock();
        std::cout << "observer while owned: " << static_cast<bool>(current) << '\n';
    }
    second.reset();
    std::cout << "observer after reset: " << static_cast<bool>(observer.lock()) << '\n';

    auto root = std::make_shared<Node>(2);
    root->child = std::make_shared<Node>(3);
    root->child->parent = root;
    auto leaf = root->child;
    root.reset();
    std::cout << "parent still alive: " << static_cast<bool>(leaf->parent.lock()) << '\n';
    leaf.reset();
}
