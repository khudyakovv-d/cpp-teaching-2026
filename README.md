# Материалы курса C++

Перед первым занятием подготовьте инструменты по инструкции: [настройка окружения](environment-setup.md).

## Занятия

1. [Компиляция и сборка проекта](lessons/01-compilation-and-build/README.md) — путь от `.cpp` до программы, системы сборки, цели CMake и демонстрация задачи 0а.
2. [От структуры в C к классу в C++](lessons/02-classes-and-vector/README.md) — собственный `Vector`, время жизни, ссылки и `const`, готовая реализация для демонстрации.
3. [Исключения и владение ресурсами](lessons/03-exceptions/README.md) — передача ошибок, раскрутка стека, утечки памяти и безопасность присваивания.
4. [Перемещение и умные указатели](lessons/04-move-and-smart-pointers/README.md) — конструктор перемещения, перемещающее присваивание и владение через `unique_ptr`, `shared_ptr` и `weak_ptr`.
5. [Юнит-тесты с Google Test](lessons/05-unit-tests/README.md) — проверки поведения `Vector`, фикстуры, параметры и запуск через CTest.
6. [Наследование и полиморфизм](lessons/06-inheritance/README.md) — отношения между объектами, базовый интерфейс, виртуальные функции и владение фигурами.

## Как устроен репозиторий

Каждое занятие — отдельная папка в `lessons`. Внутри всегда рядом находятся `theory` с конспектами и `practice` с проектом к этой теории. Начинайте с `README.md` занятия: он задаёт порядок чтения и ведёт к практике.

```text
lessons/
├── 01-compilation-and-build/
│   ├── README.md
│   ├── theory/
│   │   └── 01-…md, 02-…md, 03-…md
│   └── practice/
│       ├── README.md
│       ├── 01-single-target/
│       └── 02-library-targets/
├── 02-classes-and-vector/
│   ├── README.md
│   ├── theory/
│   │   └── 01-…md, 02-…md, 03-…md, 04-…md, 05-…md
│   └── practice/
│       ├── README.md
│       ├── CMakeLists.txt
│       └── исходники Vector
├── 03-exceptions/
│   ├── README.md
│   ├── theory/
│   │   └── 01-…md, 02-…md, 03-…md
│   └── practice/
│       ├── README.md
│       ├── CMakeLists.txt
│       └── main.cpp
├── 04-move-and-smart-pointers/
│   ├── README.md
│   ├── theory/
│   │   └── 01-…md, 02-…md, 03-…md
│   └── practice/
│       ├── README.md
│       ├── CMakeLists.txt
│       └── исходники Vector и main.cpp
├── 05-unit-tests/
│   ├── README.md
│   ├── theory/
│   │   └── 01-…md, 02-…md, 03-…md
│   └── practice/
│       ├── README.md
│       ├── CMakeLists.txt
│       └── test.cpp
└── 06-inheritance/
    ├── README.md
    ├── theory/
    │   └── 01-…md, 02-…md, 03-…md
    └── practice/
        ├── README.md
        ├── CMakeLists.txt
        └── shapes.h, shapes.cpp и main.cpp
```

Для сборки открывайте в VS Code папку конкретного проекта с `CMakeLists.txt`. В первом занятии это одна из двух версий внутри `practice`, в остальных — сама папка `practice`.

Общая [настройка окружения](environment-setup.md) остаётся в корне курса.
