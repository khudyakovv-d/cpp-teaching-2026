#include "vector.h"

#include <iostream>
#include <stdexcept>

void print(const Vector& v) {
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << ' ';
    }
    std::cout << '\n';
}

void replaceCopy(Vector target, const Vector& source) {
    std::cout << "Parameter before assignment: ";
    print(target);
    target = source; // Параметр уже создан: копирующее присваивание.
    std::cout << "Parameter after assignment: ";
    print(target);
}

int main() {
    // Подготовим исходные измерения.
    Vector measurements(2);
    measurements[0] = 10;
    measurements[1] = 20;

    // Создадим независимую рабочую копию и попробуем исправление.
    Vector corrected = measurements; // Конструктор копирования.
    corrected[0] = 12;
    std::cout << "Original measurements: ";
    print(measurements);
    std::cout << "Working copy: ";
    print(corrected);

    // Сохраним исходные данные в заранее созданном объекте.
    Vector saved;          // Конструктор без аргументов.
    saved = measurements;  // Копирующее присваивание.
    std::cout << "Saved original: ";
    print(saved);

    // Примем исправление: объект measurements уже существует.
    measurements = corrected;
    std::cout << "Accepted measurements: ";
    print(measurements);

    // Ссылка обозначает тот же объект; указатель можно перенаправить.
    Vector& alias = measurements;
    alias[1] = 21;
    const Vector& view = measurements;
    std::cout << "Measurements through const reference: ";
    print(view);

    Vector* pointer = &measurements;
    pointer = &saved;       // Меняется адрес, присваивания Vector нет.
    (*pointer)[1] = 25;     // Меняется элемент saved.
    std::cout << "Saved data through pointer: ";
    print(saved);

    // При вызове копируется параметр target; внутри он получает новые данные.
    replaceCopy(measurements, saved);
    std::cout << "Measurements after function: ";
    print(measurements);

    // Указатель локальный, а объект и его массив выделены динамически.
    {
        Vector* dynamicCopy = new Vector(measurements);
        std::cout << "Dynamic copy: ";
        print(*dynamicCopy);
        delete dynamicCopy;
    }

    Vector sum = measurements + corrected;
    std::cout << "Sum: ";
    print(sum);

    try {
        Vector wrongSize(3);
        Vector invalidSum = measurements + wrongSize;
        print(invalidSum);
    } catch (const std::length_error& error) {
        std::cout << "Error: " << error.what() << '\n';
    }
}
