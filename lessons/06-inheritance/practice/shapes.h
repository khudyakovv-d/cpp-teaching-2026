#pragma once

#include <stdexcept>
#include <string_view>

class InvalidShape : public std::runtime_error {
public:
    explicit InvalidShape(const char* message) : std::runtime_error(message) {}
};

class Shape {
public:
    explicit Shape(int id) : id_(id) {}
    virtual ~Shape() = default;

    int id() const { return id_; }
    virtual std::string_view name() const { return "shape"; }
    virtual std::string_view category() const { return "geometry"; }
    virtual double area() const = 0;

protected:
    void printId() const;

private:
    int id_;
};

class Circle final : public Shape {
public:
    Circle(int id, double radius);
    ~Circle() override;

    double radius() const { return radius_; }
    void describe() const;
    std::string_view name() const override;
    double area() const override;

private:
    double radius_;
};

class Rectangle final : public Shape {
public:
    Rectangle(int id, double width, double height);
    ~Rectangle() override;

    std::string_view name() const override;
    double area() const override;

private:
    double width_;
    double height_;
};
