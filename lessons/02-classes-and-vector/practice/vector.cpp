#include "vector.h"

#include <stdexcept>

Vector::Vector() {
    arr_ = nullptr;
    size_ = 0;
}

Vector::Vector(int size) : arr_(nullptr), size_(0) {
    if (size < 0) {
        throw std::length_error("Vector size must be non-negative");
    }
    if (size > 0) {
        arr_ = new int[size]{};
    }
    size_ = size;
}

Vector::~Vector() {
    delete[] arr_;
}

int Vector::size() const {
    return size_;
}

int& Vector::operator[](int i) {
    return arr_[i];
}

const int& Vector::operator[](int i) const {
    return arr_[i];
}

Vector::Vector(const Vector& vec) : arr_(nullptr), size_(vec.size_) {
    if (size_ > 0) {
        arr_ = new int[size_];
    }
    for (int i = 0; i < size_; ++i) {
        arr_[i] = vec.arr_[i];
    }
}

Vector& Vector::operator=(const Vector& vec) {
    if (this == &vec) {
        return *this;
    }

    int* newArr = nullptr;
    if (vec.size_ > 0) {
        newArr = new int[vec.size_];
    }
    for (int i = 0; i < vec.size_; ++i) {
        newArr[i] = vec.arr_[i];
    }

    delete[] arr_;
    arr_ = newArr;
    size_ = vec.size_;
    return *this;
}

Vector Vector::operator+(const Vector& other) const {
    if (size_ != other.size_) {
        throw std::length_error("Vectors must be same size");
    }

    Vector result(size_);
    for (int i = 0; i < size_; ++i) {
        result[i] = arr_[i] + other[i];
    }
    return result;
}
