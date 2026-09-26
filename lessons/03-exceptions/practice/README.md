# Демонстрация исключений

Программа использует `Vector` из занятия 2. Откройте эту папку в VS Code как CMake-проект и соберите цель `exceptions_demo`. Из терминала проект собирается так:

```sh
cmake -S . -B build
cmake --build build --config Debug
```

После сборки запустите программу:

- macOS и Linux с Make или Ninja: `./build/exceptions_demo`;
- Windows с генератором Visual Studio: `.\build\Debug\exceptions_demo.exe`;
- Windows с Ninja: `.\build\exceptions_demo.exe`.

В выводе сначала идут `destroy second`, `destroy first`, затем `catch`: локальные объекты уничтожаются до входа в обработчик. При ошибке в `Holder` появится `destroy field`, но не `Holder destructor`. В конце `partial: 7 2` и `preserved: 1 2` показывают разницу между двумя гарантиями из третьей части.

Вывод не показывает освобождение массива внутри `Vector`. Этот вывод следует из [его деструктора](../../02-classes-and-vector/practice/vector.cpp), который выполняет `delete[] arr_`.
