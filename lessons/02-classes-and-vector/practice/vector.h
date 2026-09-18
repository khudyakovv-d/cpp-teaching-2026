#pragma once

class Vector {
private:
    int* arr_;
    int size_;

public:
    Vector();
    explicit Vector(int size);
    ~Vector();

    int size() const;
    int& operator[](int i);
    const int& operator[](int i) const;

    Vector(const Vector& vec);
    Vector& operator=(const Vector& vec);
    Vector operator+(const Vector& other) const;
};
