#include "shapes.h"

#include <iostream>
#include <numbers>
#include <stdexcept>

void Shape::printId() const {
    std::cout << "shape " << id() << '\n';
}

void Circle::describe() const {
    printId();
    std::cout << "radius " << radius_ << '\n';
}

Circle::Circle(int id, double radius) : Shape(id), radius_(radius) {
    if (!(radius > 0.0)) {
        throw InvalidShape("radius must be positive");
    }
}

Circle::~Circle() {
    std::cout << "~Circle\n";
}

std::string_view Circle::name() const {
    return "circle";
}

double Circle::area() const {
    return std::numbers::pi * radius_ * radius_;
}

Rectangle::Rectangle(int id, double width, double height)
    : Shape(id), width_(width), height_(height) {
    if (!(width > 0.0) || !(height > 0.0)) {
        throw InvalidShape("sides must be positive");
    }
}

Rectangle::~Rectangle() {
    std::cout << "~Rectangle\n";
}

std::string_view Rectangle::name() const {
    return "rectangle";
}

double Rectangle::area() const {
    return width_ * height_;
}
