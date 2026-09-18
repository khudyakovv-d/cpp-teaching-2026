#include "vector.h"

#include <iostream>
#include <stdexcept>

void check(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

int main() {
    try {
        Vector empty;
        Vector zero(0);
        Vector emptyCopy = empty;
        check(empty.size() == 0 && zero.size() == 0 && emptyCopy.size() == 0,
              "Empty construction and copying");

        Vector a(2);
        check(a.size() == 2 && a[0] == 0 && a[1] == 0, "Zero initialization");
        a[0] = 10;
        a[1] = 20;
        Vector b = a;
        b[0] = 99;
        check(a[0] == 10 && b[0] == 99 && b[1] == 20, "Independent copy");

        Vector assigned(5);
        assigned = a;
        assigned[1] = 7;
        check(assigned.size() == 2 && a[1] == 20, "Independent assignment");
        Vector& alias = assigned;
        assigned = alias;
        check(assigned[0] == 10 && assigned[1] == 7, "Self-assignment");
        check(&(assigned = a) == &assigned, "Assignment returns recipient");

        {
            Vector source(1);
            source[0] = 42;
            assigned = source;
        }
        check(assigned.size() == 1 && assigned[0] == 42, "Copy outlives source");
        assigned = empty;
        check(assigned.size() == 0, "Assign empty to nonempty");
        empty = a;
        check(empty.size() == 2 && empty[1] == 20, "Assign nonempty to empty");

        const Vector& view = a;
        Vector sum = view + b;
        check(sum[0] == 109 && sum[1] == 40 && a[0] == 10 && b[0] == 99,
              "Sum preserves operands");
        Vector emptySum = zero + emptyCopy;
        check(emptySum.size() == 0, "Sum of empty vectors");

        bool rejected = false;
        try {
            Vector invalid(-1);
        } catch (const std::length_error&) {
            rejected = true;
        }
        check(rejected, "Reject negative size");

        rejected = false;
        try {
            Vector invalid = a + zero;
        } catch (const std::length_error&) {
            rejected = true;
        }
        check(rejected && a[0] == 10, "Reject mismatched sizes");
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    std::cout << "Vector checks passed\n";
}
