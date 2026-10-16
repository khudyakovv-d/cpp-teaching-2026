# Демонстрация наследования и полиморфизма

Проект показывает порядок создания базовой части и поля, очистку при исключении из конструктора, выбор обычного и виртуального метода, срезку при передаче по значению, копирование базовой части, вызовы на стадиях жизни объекта, владение через `std::unique_ptr<Shape>`, собственное исключение и модель запуска теста.

Откройте эту папку в VS Code как CMake-проект и соберите цель `inheritance_demo`. Из терминала:

```sh
cmake -S . -B build
cmake --build build --config Debug
```

После сборки запустите программу:

- macOS и Linux с Make или Ninja: `./build/inheritance_demo`;
- Windows с генератором Visual Studio: `.\build\Debug\inheritance_demo.exe`;
- Windows с Ninja: `.\build\inheritance_demo.exe`.

Ожидаемый вывод:

```text
== lifetime ==
Base(7)
Member
Derived body
~Derived body
~Member
~Base
== constructor throws ==
Base(8)
Member
Derived body
~Member
~Base
caught: construction failed
== dispatch and slicing ==
direct non-virtual: circle
base non-virtual: shape
base virtual: circle
by reference: circle
by value: shape
inherited virtual: geometry
== copy base and field ==
copy: id = 7, radius = 2
== same pointer, different objects ==
pointer to circle: circle
pointer to rectangle: rectangle
== construction dispatch ==
Base
Derived
Derived
Base
== virtual destruction ==
reset owner
~Derived body
~Resource
~Base
object deleted
== polymorphic ownership ==
circle area = 12.57
rectangle area = 12.00
reset first owner
~Circle
reset second owner
~Rectangle
owners are empty
== specific exception handler ==
caught: radius must be positive
== base exception handler ==
base: radius must be positive
== test lifecycle model ==
prepare
value = 7
cleanup
```

Основной пример развивается так же, как в теории. В `dispatch` находится конкретная промежуточная версия `Shape` с идентификатором и `Circle` с радиусом: она нужна для сравнения вызовов и срезки. У круга переопределён `name`, а `category` остаётся базовым. Собственный конструктор копирования круга копирует и базу, и радиус. Итоговый абстрактный интерфейс добавляет `area()`; он объявлен в [shapes.h](shapes.h), реализации — в [shapes.cpp](shapes.cpp).

Отдельные небольшие классы в [main.cpp](main.cpp) показывают время жизни и модель тестовой библиотеки:

- `lifetime`: создание базы, поля и тела наследника; при исключении из тела очищаются готовые части, но деструктор недостроенного наследника не вызывается.
- `construction_dispatch`: виртуальные вызовы из конструкторов и деструкторов печатают `Base`, `Derived`, `Derived`, `Base`.
- `destruction`: удаление через `unique_ptr<Base>` вызывает тело деструктора наследника, деструктор его поля и базы.
- `test_lifecycle`: обычный `Test::Run` вызывает виртуальную подготовку и тело наследника, затем унаследованную очистку. Это модель успешного пути; зависимость от Google Test для её сборки не нужна.

При смене адреса в `selected` его статический тип остаётся прежним, а виртуальный `name()` выполняет реализацию нового объекта. Передача по значению создаёт отдельную базовую копию, поэтому выводит `shape`.

Владельцы фигур очищаются явными вызовами `reset()`, чтобы порядок вывода был фиксирован. Каждый вызов удаляет полную фигуру через `Shape*`; виртуальный деструктор обеспечивает выбор начала уничтожения. Без `reset()` фигуры также удалились бы при уничтожении контейнера. Печатающие деструкторы позволяют наблюдать этот момент; собственных ресурсов у фигур нет.

Конструкторы фигур бросают собственный `InvalidShape`, наследующий `std::runtime_error`. В одном примере его ловит конкретный обработчик, в другом — `std::exception&`, через который виртуально вызывается унаследованный `what()`.

Ошибочное удаление производного объекта через базовый указатель с невиртуальным деструктором не запускаем: его поведение не определено. Для локального объекта базовая ссылка ничего не меняет — он уничтожается по типу своего объявления.
