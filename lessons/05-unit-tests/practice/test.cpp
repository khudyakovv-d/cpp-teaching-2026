#include "vector.h"

#include <gtest/gtest.h>
#include <stdexcept>
#include <type_traits>
#include <utility>

TEST(VectorTest, DefaultConstructorCreatesEmptyVector) {
    Vector values;
    EXPECT_EQ(values.size(), 0);
}

TEST(VectorTest, ElementCanBeChanged) {
    Vector values(2);        // Arrange
    ASSERT_EQ(values.size(), 2);

    values[1] = 42;          // Act

    EXPECT_EQ(values[1], 42); // Assert
    EXPECT_EQ(values[0], 0);
}

TEST(AssertionsTest, ComparesStringsAndFloatingPoint) {
    const char first[] = "vector";
    const char second[] = "vector";
    EXPECT_STREQ(first, second);
    EXPECT_NEAR(0.1 + 0.2, 0.3, 1e-12);
}

TEST(VectorTest, NewElementsAreZero) {
    Vector values(3);
    ASSERT_EQ(values.size(), 3);
    for (int i = 0; i < values.size(); ++i) {
        EXPECT_EQ(values[i], 0) << "element index = " << i;
    }
}

TEST(VectorTest, CopyConstructorCreatesIndependentCopy) {
    Vector original(2);
    ASSERT_EQ(original.size(), 2);
    original[0] = 7;
    original[1] = -3;

    Vector copy(original);

    ASSERT_EQ(copy.size(), 2);
    EXPECT_EQ(copy[0], 7);
    EXPECT_EQ(copy[1], -3);
    copy[0] = 99;
    EXPECT_EQ(original[0], 7);
}

TEST(VectorTest, AssignmentReplacesContentsAndReturnsDestination) {
    Vector source(2);
    ASSERT_EQ(source.size(), 2);
    source[0] = 7;
    source[1] = -3;
    Vector destination(1);
    ASSERT_EQ(destination.size(), 1);
    destination[0] = 100;

    Vector& returned = (destination = source);

    EXPECT_EQ(&returned, &destination);
    ASSERT_EQ(destination.size(), 2);
    EXPECT_EQ(destination[0], 7);
    EXPECT_EQ(destination[1], -3);
    destination[0] = 99;
    EXPECT_EQ(source[0], 7);
}

TEST(VectorTest, SelfAssignmentPreservesContents) {
    Vector values(1);
    ASSERT_EQ(values.size(), 1);
    values[0] = 7;
    Vector& alias = values;

    values = alias;

    ASSERT_EQ(values.size(), 1);
    EXPECT_EQ(values[0], 7);
}

TEST(VectorTest, AdditionProducesSumWithoutChangingOperands) {
    Vector left(2);
    Vector right(2);
    ASSERT_EQ(left.size(), 2);
    ASSERT_EQ(right.size(), 2);
    left[0] = 3;
    left[1] = -4;
    right[0] = 5;
    right[1] = 4;

    Vector result = left + right;

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 8);
    EXPECT_EQ(result[1], 0);
    EXPECT_EQ(left[0], 3);
    EXPECT_EQ(left[1], -4);
    EXPECT_EQ(right[0], 5);
    EXPECT_EQ(right[1], 4);
    result[0] = 100;
    EXPECT_EQ(left[0], 3);
    EXPECT_EQ(right[0], 5);
}

TEST(VectorTest, AdditionRejectsDifferentSizes) {
    Vector left(1);
    Vector right(2);
    ASSERT_EQ(left.size(), 1);
    ASSERT_EQ(right.size(), 2);
    left[0] = 7;
    right[0] = 8;
    right[1] = 9;

    EXPECT_THROW(left + right, std::length_error);

    ASSERT_EQ(left.size(), 1);
    ASSERT_EQ(right.size(), 2);
    EXPECT_EQ(left[0], 7);
    EXPECT_EQ(right[0], 8);
    EXPECT_EQ(right[1], 9);
}

TEST(VectorTest, ConstructorRejectsNegativeSize) {
    EXPECT_THROW(Vector(-1), std::length_error);
}

TEST(VectorTest, ConstReferenceAllowsReading) {
    Vector values(1);
    ASSERT_EQ(values.size(), 1);
    values[0] = 42;
    const Vector& readonly = values;

    EXPECT_EQ(readonly[0], 42);
}

class PreparedVectorTest : public ::testing::Test {
protected:
    Vector values{2};

    void SetUp() override {
        ASSERT_EQ(values.size(), 2);
        values[0] = 7;
        values[1] = -3;
    }
};

TEST_F(PreparedVectorTest, ReadsPreparedValues) {
    EXPECT_EQ(values[0], 7);
    EXPECT_EQ(values[1], -3);
}

TEST_F(PreparedVectorTest, CanChangeFirstElement) {
    values[0] = 99;
    EXPECT_EQ(values[0], 99);
    EXPECT_EQ(values[1], -3);
}

class VectorSizeTest : public ::testing::TestWithParam<int> {};

TEST_P(VectorSizeTest, PreservesRequestedSize) {
    const int requestedSize = GetParam();
    Vector values(requestedSize);
    EXPECT_EQ(values.size(), requestedSize);
}

INSTANTIATE_TEST_SUITE_P(
    ValidSizes,
    VectorSizeTest,
    ::testing::Values(0, 1, 2, 5)
);

struct AdditionCase {
    int left;
    int right;
    int expected;
};

class VectorAdditionTest
    : public ::testing::TestWithParam<AdditionCase> {};

TEST_P(VectorAdditionTest, AddsSingleElements) {
    const AdditionCase input = GetParam();
    Vector left(1);
    Vector right(1);
    ASSERT_EQ(left.size(), 1);
    ASSERT_EQ(right.size(), 1);
    left[0] = input.left;
    right[0] = input.right;

    Vector result = left + right;

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], input.expected)
        << "left = " << input.left << ", right = " << input.right;
}

INSTANTIATE_TEST_SUITE_P(
    SignedValues,
    VectorAdditionTest,
    ::testing::Values(
        AdditionCase{3, 5, 8},
        AdditionCase{-4, 4, 0},
        AdditionCase{-2, -3, -5}
    )
);

TEST(VectorTest, EmptyVectorCanBeCopied) {
    Vector original;
    Vector copy(original);
    EXPECT_EQ(copy.size(), 0);
}

TEST(VectorTest, AssignmentFromEmptyVectorClearsDestination) {
    Vector source;
    Vector destination(2);
    destination = source;
    EXPECT_EQ(destination.size(), 0);
    EXPECT_EQ(source.size(), 0);
}

TEST(VectorTest, EmptyVectorsCanBeAdded) {
    Vector left;
    Vector right;
    Vector result = left + right;
    EXPECT_EQ(result.size(), 0);
    EXPECT_EQ(left.size(), 0);
    EXPECT_EQ(right.size(), 0);
}

// Дополнительно: проверка типов при компиляции, без запуска Google Test.
// declval<T>() позволяет описать выражение с T, не создавая объект.
// Здесь он используется только внутри decltype: выражение не выполняется.
// decltype сохраняет тип результата, включая ссылку и const.
// is_same_v<A, B> равно true, если A и B — один и тот же тип.
// static_assert требует true; иначе компиляция завершится ошибкой.
static_assert(std::is_same_v<decltype(std::declval<Vector&>()[0]), int&>);
static_assert(std::is_same_v<decltype(std::declval<const Vector&>()[0]), const int&>);
