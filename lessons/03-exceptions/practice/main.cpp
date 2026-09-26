#include "vector.h"

#include <iostream>
#include <stdexcept>

struct Trace {
    const char* name;

    ~Trace() {
        std::cout << "destroy " << name << '\n';
    }
};

void fail() {
    Trace first{"first"};
    Trace second{"second"};
    Vector bad(-1);
    std::cout << bad.size() << '\n'; // Не выполнится.
}

struct Holder {
    Vector data;
    Trace field{"field"};

    explicit Holder(int size) : data(size) {
        if (size > 100) {
            throw std::length_error("Holder size is too large");
        }
    }

    ~Holder() {
        std::cout << "Holder destructor\n"; // При ошибке конструктора не выполнится.
    }
};

void copy_checked(Vector& target, const Vector& source) {
    if (target.size() != source.size()) {
        throw std::length_error("different sizes");
    }
    for (int i = 0; i < source.size(); ++i) {
        if (source[i] < 0) {
            throw std::invalid_argument("negative element");
        }
        target[i] = source[i];
    }
}

void copy_checked_strong(Vector& target, const Vector& source) {
    if (target.size() != source.size()) {
        throw std::length_error("different sizes");
    }
    for (int i = 0; i < source.size(); ++i) {
        if (source[i] < 0) {
            throw std::invalid_argument("negative element");
        }
    }
    for (int i = 0; i < source.size(); ++i) {
        target[i] = source[i];
    }
}

int main() {
    try {
        fail();
    } catch (const std::length_error& error) {
        std::cout << "catch: " << error.what() << '\n';
    }

    try {
        Holder holder(101);
    } catch (const std::length_error& error) {
        std::cout << "catch: " << error.what() << '\n';
    }

    Vector source(2);
    source[0] = 7;
    source[1] = -1;

    Vector partial(2);
    partial[0] = 1;
    partial[1] = 2;
    Vector preserved = partial;

    try {
        copy_checked(partial, source);
    } catch (const std::invalid_argument&) {
        std::cout << "partial: " << partial[0] << ' ' << partial[1] << '\n';
    }

    try {
        copy_checked_strong(preserved, source);
    } catch (const std::invalid_argument&) {
        std::cout << "preserved: " << preserved[0] << ' ' << preserved[1] << '\n';
    }
}
