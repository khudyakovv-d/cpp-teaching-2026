#include <iomanip>
#include <iostream>
#include <memory>
#include <numbers>
#include <string>
#include <string_view>
#include <typeinfo>

namespace access_modes {

class Counter {
public:
    int value() const { return value_; }

protected:
    void increment() { ++value_; }

private:
    int value_ = 0;
};

class VisitorCounter : public Counter {
public:
    void recordVisit() { increment(); }
};

class TicketMachine : private Counter {
public:
    int nextTicket() {
        increment();
        const Counter& counter = *this;
        return counter.value();
    }
};

class Meter : protected Counter {
public:
    int reading() const { return value(); }
};

class EventMeter : public Meter {
public:
    void recordEvent() { increment(); }
    int readBase() const {
        const Counter& counter = *this;
        return counter.value();
    }
};

void run() {
    VisitorCounter visitors;
    visitors.recordVisit();
    const Counter& counter = visitors;
    std::cout << "public value = " << visitors.value() << '\n';
    std::cout << "public base view = " << counter.value() << '\n';
    TicketMachine machine;
    std::cout << "private first ticket = " << machine.nextTicket() << '\n';
    std::cout << "private second ticket = " << machine.nextTicket() << '\n';
    // machine.value(); // Ошибка: унаследованный метод закрыт.
    // Counter& inaccessible = machine; // Ошибка: закрытая база снаружи.
    EventMeter meter;
    meter.recordEvent();
    std::cout << "protected reading = " << meter.reading() << '\n';
    std::cout << "protected descendant base view = " << meter.readBase() << '\n';
    // meter.value(); // Ошибка: унаследованный метод защищён.
    // Counter& inaccessible = meter; // Ошибка: защищённая база снаружи.
}

} // namespace access_modes

namespace university_roles {

struct Student {
    virtual ~Student() = default;
    void study() const { std::cout << "study\n"; }
    void printCard() const { std::cout << "student card\n"; }
};

struct Employee {
    virtual ~Employee() = default;
    void work() const { std::cout << "work\n"; }
    void printCard() const { std::cout << "employee card\n"; }
};

struct TeachingAssistant : public Student, public Employee {};

void startStudy(const Student& student) { student.study(); }
void startWork(const Employee& employee) { employee.work(); }

void run() {
    TeachingAssistant assistant;
    startStudy(assistant);
    startWork(assistant);
    // assistant.printCard(); // Ошибка: какую карточку печатать?
    assistant.Student::printCard();
    assistant.Employee::printCard();
}

} // namespace university_roles

namespace ordinary_diamond {

struct Person {
    explicit Person(const std::string& name) : name(name) {}
    virtual ~Person() = default;
    std::string name;
};

struct Student : public Person {
    explicit Student(const std::string& name) : Person(name) {}
};

struct Employee : public Person {
    explicit Employee(const std::string& name) : Person(name) {}
};

struct TeachingAssistant : public Student, public Employee {
    explicit TeachingAssistant(const std::string& name)
        : Student(name), Employee(name) {}
};

void run() {
    TeachingAssistant assistant("Анна Смирнова");
    // assistant.name; // Ошибка: две отдельные части Person.
    // Person* person = &assistant; // Ошибка: неоднозначный путь к базе.
    std::cout << "student name = " << assistant.Student::name << '\n';
    std::cout << "employee name = " << assistant.Employee::name << '\n';
    assistant.Student::name = "Анна Иванова";
    std::cout << "after student update:\n";
    std::cout << "student name = " << assistant.Student::name << '\n';
    std::cout << "employee name = " << assistant.Employee::name << '\n';
}

} // namespace ordinary_diamond

namespace virtual_diamond {

struct Person {
    explicit Person(const std::string& name) : name(name) {
        std::cout << "Person(" << name << ")\n";
    }
    virtual ~Person() { std::cout << "~Person\n"; }
    virtual std::string_view role() const { return "person"; }
    std::string name;
};

struct Student : virtual public Person {
    explicit Student(const std::string& name) : Person(name) {
        std::cout << "Student\n";
    }
    ~Student() override { std::cout << "~Student\n"; }
    std::string_view role() const override { return "student"; }
};

struct Employee : virtual public Person {
    explicit Employee(const std::string& name) : Person(name) {
        std::cout << "Employee\n";
    }
    ~Employee() override { std::cout << "~Employee\n"; }
    std::string_view role() const override { return "employee"; }
};

struct TeachingAssistant : public Student, public Employee {
    explicit TeachingAssistant(const std::string& name)
        : Person(name), Student(name), Employee(name) {
        std::cout << "TeachingAssistant\n";
    }
    ~TeachingAssistant() override { std::cout << "~TeachingAssistant\n"; }
    // Обе ветви переопределяют role(); ассистент задаёт итоговую реализацию.
    std::string_view role() const override { return "assistant"; }
};

void run() {
    std::cout << "standalone Student\n";
    {
        Student student("Борис");
        std::cout << "name = " << student.name << '\n';
    }
    std::cout << "complete TeachingAssistant\n";
    {
        TeachingAssistant assistant("Анна Смирнова");
        Student* student = &assistant;
        Employee* employee = &assistant;
        std::cout << "student name = " << student->name << '\n';
        std::cout << "employee name = " << employee->name << '\n';
        student->name = "Анна Иванова";
        std::cout << "after student update:\n";
        std::cout << "student name = " << student->name << '\n';
        std::cout << "employee name = " << employee->name << '\n';
        Person* person = &assistant;
        std::cout << "virtual role: " << person->role() << '\n';
    }
}

} // namespace virtual_diamond

namespace shapes {

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle final : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}
    double radius() const { return radius_; }
    double area() const override {
        return std::numbers::pi * radius_ * radius_;
    }

private:
    double radius_;
};

class Rectangle final : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double area() const override { return width_ * height_; }

private:
    double width_;
    double height_;
};

void printRadius(const Shape* shape) {
    const Circle* circle = dynamic_cast<const Circle*>(shape);
    if (circle != nullptr) {
        std::cout << "radius = " << circle->radius() << '\n';
    } else {
        std::cout << "not a circle\n";
    }
}

void requireCircle(const Shape& shape) {
    try {
        const Circle& circle = dynamic_cast<const Circle&>(shape);
        std::cout << "required radius = " << circle.radius() << '\n';
    } catch (const std::bad_cast&) {
        std::cout << "circle required\n";
    }
}

void run() {
    Circle circle(2.0);
    Rectangle rectangle(3.0, 4.0);
    Shape* shape = &circle;
    std::cout << "virtual area = " << shape->area() << '\n';
    printRadius(shape);
    shape = &rectangle;
    printRadius(shape);
    printRadius(nullptr);
    requireCircle(circle);
    requireCircle(rectangle);

    Shape& knownCircle = circle;
    Circle& recovered = static_cast<Circle&>(knownCircle);
    std::cout << "static radius = " << recovered.radius() << '\n';
    // static_cast<Circle&>(static_cast<Shape&>(rectangle)); // Не выполнять: UB.

}

} // namespace shapes

namespace university_roles {

void workIfEmployed(const Student& student) {
    const Employee* employee = dynamic_cast<const Employee*>(&student);
    if (employee != nullptr) {
        employee->work();
    } else {
        std::cout << "not employed\n";
    }
    // static_cast<const Employee*>(&student); // Ошибка: соседние базы.
}

void runCrossCast() {
    TeachingAssistant assistant;
    workIfEmployed(assistant);
    Student onlyStudent;
    workIfEmployed(onlyStudent);
}

} // namespace university_roles

namespace virtual_diamond {

bool isAssistant(Person* person) {
    return dynamic_cast<TeachingAssistant*>(person) != nullptr;
}

void runCasts() {
    TeachingAssistant assistant("Анна Смирнова");
    Person* person = &assistant;
    TeachingAssistant* recovered = dynamic_cast<TeachingAssistant*>(person);
    std::cout << "recover assistant: " << (recovered == &assistant ? "yes" : "no") << '\n';
    // static_cast<TeachingAssistant*>(person); // Ошибка: виртуальная база.
    Student onlyStudent("Борис");
    std::cout << "standalone is assistant: " << (isAssistant(&onlyStudent) ? "yes" : "no") << '\n';
}

} // namespace virtual_diamond

namespace shapes {

void runConstAndOwnership() {
    const Circle circle(2.0);
    const Shape& readOnly = circle;
    const Circle* found = dynamic_cast<const Circle*>(&readOnly);
    std::cout << "const radius = " << found->radius() << '\n';
    // dynamic_cast<Circle*>(&readOnly); // Ошибка: снятие const.

    std::unique_ptr<Shape> owner = std::make_unique<Circle>(5.0);
    Circle* observer = dynamic_cast<Circle*>(owner.get());
    if (observer != nullptr) {
        std::cout << "owned radius = " << observer->radius() << '\n';
    }
    owner.reset();
    // observer теперь висячий; его не разыменовываем и не передаём в cast.
    observer = nullptr;
    std::cout << "owner empty: " << (owner == nullptr ? "yes" : "no") << '\n';
}

} // namespace shapes

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "== inheritance access ==\n";
    access_modes::run();
    std::cout << "== two base interfaces ==\n";
    university_roles::run();
    std::cout << "== ordinary diamond ==\n";
    ordinary_diamond::run();
    std::cout << "== virtual diamond ==\n";
    virtual_diamond::run();
    std::cout << "== upcast and downcast ==\n";
    shapes::run();
    std::cout << "== cross-cast ==\n";
    university_roles::runCrossCast();
    std::cout << "== downcast through virtual base ==\n";
    virtual_diamond::runCasts();
    std::cout << "== const and ownership ==\n";
    shapes::runConstAndOwnership();
}
