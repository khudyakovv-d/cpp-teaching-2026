# Демонстрация перемещения и умных указателей

Проект продолжает `Vector` из занятия 2: в локальную версию добавлены конструктор перемещения и оператор перемещающего присваивания. Копирование и остальные операции сохранены, поэтому можно сравнить независимую копию с передачей того же массива. Предыдущие занятия используют свою прежнюю версию класса.

Откройте эту папку в VS Code как CMake-проект и соберите цель `move_and_smart_pointers_demo`. Из терминала:

```sh
cmake -S . -B build
cmake --build build --config Debug
```

После сборки запустите программу:

- macOS и Linux с Make или Ninja: `./build/move_and_smart_pointers_demo`;
- Windows с генератором Visual Studio: `.\build\Debug\move_and_smart_pointers_demo.exe`;
- Windows с Ninja: `.\build\move_and_smart_pointers_demo.exe`.

Ожидаемый вывод:

```text
independent copy: 10 99
move constructor: source size = 0, same array = true
move assignment: source size = 0, same array = true
reused source: 7
unique move: source empty = true, same object = true
consumed: 42
after consume: empty = true
observer while owned: true
destroy node 1
observer after reset: false
destroy node 2
parent still alive: false
destroy node 3
```

`same array` сравнивает адрес элемента до и после перемещения `Vector`; `same object` — адрес самого `Vector` при передаче `unique_ptr`. Это две разные операции. Строки `destroy node` показывают момент уничтожения объектов: последний владелец удаляет объект, а слабая обратная связь не удерживает родителя.

Реализация новых операций находится в [vector.cpp](vector.cpp), объявления — в [vector.h](vector.h). Программа не рассчитывает на число конструкторов при возврате по значению: оно может зависеть от NRVO.
