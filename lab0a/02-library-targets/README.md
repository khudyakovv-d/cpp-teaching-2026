# Задача 0а. Версия с библиотечной целью

Это готовое продолжение [версии с одной исполняемой целью](../01-single-target/README.md). Код программы не изменился, но `module1` теперь собирается как отдельная статическая библиотека.

Перед разбором проекта прочитайте материал [«Цели и основные команды CMake»](../../lessons/01-compilation-and-build/03-cmake-targets.md).

## Структура проекта

```text
02-library-targets/
├── .vscode/
├── CMakeLists.txt
├── CMakePresets.json
├── main.cpp
└── src/
    ├── module1/
    │   ├── CMakeLists.txt
    │   ├── module1.cpp
    │   └── module1.h
    └── module2/
        ├── module2.cpp
        └── module2.h
```

Здесь две цели:

- `module1` — статическая библиотека из `module1.cpp`;
- `lab0a` — исполняемая программа из `main.cpp` и `module2.cpp`.

## Корневой CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.27)

project(lab0a LANGUAGES CXX)

add_subdirectory(src/module1)

add_executable(
    lab0a
    main.cpp
    src/module2/module2.cpp
    src/module2/module2.h
)

target_compile_features(lab0a PRIVATE cxx_std_20)

target_include_directories(
    lab0a
    PRIVATE
        src/module2
)

target_link_libraries(lab0a PRIVATE module1)
```

`add_subdirectory` подключает описание `module1`. Затем `target_link_libraries` сообщает, что программа `lab0a` зависит от этой библиотеки.

В `main.cpp` написано `#include "module2.h"`, но сам заголовок лежит в `src/module2`. Поэтому путь `src/module2` добавлен в каталоги поиска цели `lab0a` как `PRIVATE`.

Файл `module2.h` перечислен в `add_executable`, чтобы структура цели была видна целиком. Это не помогает компилятору найти заголовок: за поиск отвечает именно `target_include_directories`. Сам заголовок не компилируется отдельно, поэтому его можно убрать из списка без изменения результата сборки.

## CMakeLists.txt библиотеки

Файл `src/module1/CMakeLists.txt` создаёт библиотечную цель:

```cmake
add_library(module1 STATIC module1.cpp module1.h)

target_compile_features(module1 PRIVATE cxx_std_20)

target_include_directories(
    module1
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
```

`module1.h`, как и `module2.h` выше, добавлен в список для наглядности и отдельно не компилируется.

`${CMAKE_CURRENT_SOURCE_DIR}` здесь означает каталог `src/module1`. Путь объявлен как `PUBLIC`: он нужен самой библиотеке и автоматически передаётся цели `lab0a` при подключении через `target_link_libraries`.

В результате во время компиляции `main.cpp` компилятор получает оба каталога поиска: `src/module2` напрямую от `lab0a`, а `src/module1` — от библиотечной цели. Поэтому записи `#include "module1.h"` и `#include "module2.h"` работают, хотя заголовки лежат не рядом с `main.cpp`.

## Что показать после сборки

Откройте всю папку `02-library-targets`, выберите пресет `Build` и выполните `CMake: Configure`, затем `CMake: Build`.

В каталоге `build` появятся:

- объектный файл для `module1.cpp`;
- статическая библиотека `module1`: обычно `module1.lib` в Windows или `libmodule1.a` в macOS;
- объектные файлы для `main.cpp` и `module2.cpp`;
- исполняемая программа `lab0a`.

В окне **Output** будут видны основные действия:

1. компиляция `module1.cpp`, `main.cpp` и `module2.cpp`;
2. создание библиотеки `module1`;
3. линковка `lab0a` с этой библиотекой.

Исходные файлы могут компилироваться в другом порядке или параллельно. Важно не расположение строк в логе, а зависимость: линковка `lab0a` начнётся только после создания библиотеки `module1`.

Если изменить `module1.cpp`, пересоберутся библиотека `module1` и программа `lab0a`, но `main.cpp` и `module2.cpp` повторно компилироваться не будут. Если изменить `module2.cpp`, библиотека `module1` останется прежней.

Вывод программы остаётся тем же, что и в первой версии: изменилась организация сборки, а не поведение кода.
