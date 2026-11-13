#include "check.h"

#include <iostream>

namespace lsp {
namespace bad {

class Rectangle {
public:
    virtual ~Rectangle() = default;
    // Договор: меняется только указанная сторона.
    virtual void setWidth(int value) { width_ = value; }
    virtual void setHeight(int value) { height_ = value; }
    int width() const { return width_; }
    int height() const { return height_; }
    int area() const { return width_ * height_; }

private:
    int width_ = 0;
    int height_ = 0;
};

class Square final : public Rectangle {
public:
    void setWidth(int value) override { setSide(value); }
    void setHeight(int value) override { setSide(value); }

private:
    void setSide(int value) {
        Rectangle::setWidth(value);
        Rectangle::setHeight(value);
    }
};

int resizeAndMeasure(Rectangle& rectangle) {
    rectangle.setWidth(4);
    rectangle.setHeight(5);
    return rectangle.area(); // По договору Rectangle ожидается 20.
}

} // namespace bad
namespace good {

class Shape {
public:
    virtual ~Shape() = default;
    virtual int area() const = 0;
};

class Rectangle final : public Shape {
public:
    Rectangle(int width, int height) : width_(width), height_(height) {}
    void setWidth(int value) { width_ = value; }
    void setHeight(int value) { height_ = value; }
    int area() const override { return width_ * height_; }

private:
    int width_;
    int height_;
};

class Square final : public Shape {
public:
    explicit Square(int side) : side_(side) {}
    void setSide(int value) { side_ = value; }
    int area() const override { return side_ * side_; }

private:
    int side_;
};

int measure(const Shape& shape) { return shape.area(); }

int resizeAndMeasure(Rectangle& rectangle) {
    rectangle.setWidth(4);
    rectangle.setHeight(5);
    return rectangle.area();
}

} // namespace good

void run() {
    std::cout << "== LSP ==\n";
    bad::Rectangle rectangle;
    bad::Square square;
    check(bad::resizeAndMeasure(rectangle) == 20, "LSP: rectangle contract");
    check(bad::resizeAndMeasure(square) == 25, "LSP: demonstrate violation");
    check(square.width() != 4, "LSP: second setter changed width");
    std::cout << "before rectangle: " << rectangle.area() << " (expected 20)\n";
    std::cout << "before square: " << square.area() << " (expected 20)\n";

    good::Rectangle fixedRectangle(1, 1);
    check(good::resizeAndMeasure(fixedRectangle) == 20, "LSP: corrected resize client");
    good::Square fixedSquare(1);
    fixedSquare.setSide(5);
    check(good::measure(fixedRectangle) == 20, "LSP: corrected rectangle");
    check(good::measure(fixedSquare) == 25, "LSP: corrected square");
    // good::resizeAndMeasure(fixedSquare); // Ошибка компиляции: Square не является Rectangle.
    std::cout << "after rectangle: " << good::measure(fixedRectangle) << '\n';
    std::cout << "after square: " << good::measure(fixedSquare) << '\n';
}

} // namespace lsp
