#include "shapes.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace lifetime {

class Base {
public:
    explicit Base(int id) {
        std::cout << "Base(" << id << ")\n";
    }

    ~Base() { std::cout << "~Base\n"; }
};

class Member {
public:
    Member() { std::cout << "Member\n"; }
    ~Member() { std::cout << "~Member\n"; }
};

class Derived : public Base {
public:
    explicit Derived(int id, bool fail = false) : Base(id) {
        std::cout << "Derived body\n";
        if (fail) {
            throw std::runtime_error("construction failed");
        }
    }

    ~Derived() { std::cout << "~Derived body\n"; }

private:
    Member member_;
};

} // namespace lifetime

namespace dispatch {

// Промежуточная конкретная версия фигур позволяет показать срезку.
class Shape {
public:
    explicit Shape(int id) : id_(id) {}
    virtual ~Shape() = default;

    int id() const { return id_; }
    std::string_view label() const { return "shape"; }
    virtual std::string_view name() const { return "shape"; }
    virtual std::string_view category() const { return "geometry"; }

private:
    int id_;
};

class Circle : public Shape {
public:
    Circle(int id, double radius) : Shape(id), radius_(radius) {}
    Circle(const Circle& other) : Shape(other), radius_(other.radius_) {}

    double radius() const { return radius_; }
    std::string_view label() const { return "circle"; }
    std::string_view name() const override { return "circle"; }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(int id, double width, double height)
        : Shape(id), width_(width), height_(height) {}
    std::string_view name() const override { return "rectangle"; }

private:
    double width_;
    double height_;
};

void printReference(const Shape& object) {
    std::cout << "by reference: " << object.name() << '\n';
}

void printValue(Shape object) {
    std::cout << "by value: " << object.name() << '\n';
}

} // namespace dispatch

namespace construction_dispatch {

class Base {
public:
    Base() { printName(); }
    virtual ~Base() { printName(); }
    virtual void printName() const { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    Derived() { printName(); }
    ~Derived() override { printName(); }
    void printName() const override { std::cout << "Derived\n"; }
};

} // namespace construction_dispatch

namespace destruction {

class Base {
public:
    virtual ~Base() { std::cout << "~Base\n"; }
};

class Resource {
public:
    ~Resource() { std::cout << "~Resource\n"; }
};

class Derived : public Base {
public:
    ~Derived() override { std::cout << "~Derived body\n"; }

private:
    Resource resource_;
};

} // namespace destruction

namespace test_lifecycle {

class Test {
public:
    virtual ~Test() = default;

    void Run() {
        SetUp();
        TestBody();
        TearDown();
    }

protected:
    virtual void SetUp() {}
    virtual void TestBody() = 0;
    virtual void TearDown() { std::cout << "cleanup\n"; }
};

class PreparedTest : public Test {
protected:
    void SetUp() override {
        value_ = 7;
        std::cout << "prepare\n";
    }

    void TestBody() override {
        std::cout << "value = " << value_ << '\n';
    }

private:
    int value_ = 0;
};

} // namespace test_lifecycle

void printArea(const Shape& shape) {
    std::cout << shape.name() << " area = " << shape.area() << '\n';
}

int main() {
    std::cout << "== lifetime ==\n";
    {
        lifetime::Derived object(7);
    }

    std::cout << "== constructor throws ==\n";
    try {
        lifetime::Derived object(8, true);
    } catch (const std::runtime_error& error) {
        std::cout << "caught: " << error.what() << '\n';
    }

    std::cout << "== dispatch and slicing ==\n";
    dispatch::Circle object(7, 2.0);
    const dispatch::Shape& base = object;
    std::cout << "direct non-virtual: " << object.label() << '\n';
    std::cout << "base non-virtual: " << base.label() << '\n';
    std::cout << "base virtual: " << base.name() << '\n';
    dispatch::printReference(object);
    dispatch::printValue(object);
    std::cout << "inherited virtual: " << base.category() << '\n';

    std::cout << "== copy base and field ==\n";
    dispatch::Circle copied = object;
    std::cout << "copy: id = " << copied.id()
              << ", radius = " << copied.radius() << '\n';

    std::cout << "== same pointer, different objects ==\n";
    dispatch::Rectangle rectangle(8, 3.0, 4.0);
    const dispatch::Shape* selected = &object;
    std::cout << "pointer to circle: " << selected->name() << '\n';
    selected = &rectangle;
    std::cout << "pointer to rectangle: " << selected->name() << '\n';

    std::cout << "== construction dispatch ==\n";
    {
        construction_dispatch::Derived observed;
    }

    std::cout << "== virtual destruction ==\n";
    std::unique_ptr<destruction::Base> owner =
        std::make_unique<destruction::Derived>();
    std::cout << "reset owner\n";
    owner.reset();
    std::cout << "object deleted\n";

    std::cout << "== polymorphic ownership ==\n";
    std::cout << std::fixed << std::setprecision(2);
    {
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<Circle>(7, 2.0));
        shapes.push_back(std::make_unique<Rectangle>(8, 3.0, 4.0));
        for (const auto& shape : shapes) {
            printArea(*shape);
        }

        // Удаляем явно, чтобы порядок строк не зависел от порядка очистки vector.
        std::cout << "reset first owner\n";
        shapes[0].reset();
        std::cout << "reset second owner\n";
        shapes[1].reset();
        std::cout << "owners are empty\n";
    }

    std::cout << "== specific exception handler ==\n";
    try {
        auto invalid = std::make_unique<Circle>(9, -1.0);
    } catch (const InvalidShape& error) {
        std::cout << "caught: " << error.what() << '\n';
    }

    std::cout << "== base exception handler ==\n";
    try {
        auto invalid = std::make_unique<Circle>(9, -1.0);
    } catch (const std::exception& error) {
        std::cout << "base: " << error.what() << '\n';
    }

    std::cout << "== test lifecycle model ==\n";
    test_lifecycle::PreparedTest test;
    test_lifecycle::Test& testBase = test;
    testBase.Run();
}
