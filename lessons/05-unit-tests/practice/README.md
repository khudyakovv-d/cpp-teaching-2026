# Демонстрация тестов Vector

Проект собирает примеры из теории в одну тестовую программу. Он использует исходники `Vector` из занятия 2: отдельную копию класса поддерживать не требуется. Есть обычные сценарии, фикстура с подготовкой данных и два набора параметризованных тестов. Полная конфигурация сборки — в [CMakeLists.txt](CMakeLists.txt), полный набор проверок — в [test.cpp](test.cpp).

Программа `vector_tests` имеет свой `main`: его предоставляет библиотека `GTest::gtest_main`. Эта функция инициализирует Google Test и запускает зарегистрированные сценарии. В `test.cpp` свой `main` добавлять не нужно.

Из корня репозитория:

```bash
cmake -S lessons/05-unit-tests/practice -B /tmp/cpp-unit-tests -DCMAKE_BUILD_TYPE=Debug
cmake --build /tmp/cpp-unit-tests
ctest --test-dir /tmp/cpp-unit-tests --output-on-failure
```

Для первого конфигурирования нужен доступ к GitHub: CMake скачает закреплённую ревизию Google Test. Конкретная ревизия обеспечивает воспроизводимость сборки. При последующих сборках используется уже полученный исходный код.

Для генераторов с несколькими конфигурациями, например Visual Studio, используйте `cmake --build /tmp/cpp-unit-tests --config Debug` и `ctest --test-dir /tmp/cpp-unit-tests -C Debug --output-on-failure`. Исполняемый файл обычно будет в подпапке `Debug`; в Windows он имеет расширение `.exe`.

Прямой запуск на macOS/Linux с обычным генератором:

```bash
/tmp/cpp-unit-tests/vector_tests
/tmp/cpp-unit-tests/vector_tests --gtest_filter='VectorTest.CopyConstructorCreatesIndependentCopy'
/tmp/cpp-unit-tests/vector_tests --gtest_shuffle --gtest_repeat=20
```

Успешный запуск выводит количество пройденных тестов и завершает программу с кодом ноль. Если ожидание нарушено, Google Test показывает имя сценария, строку и несовпавшие значения; CTest при `--output-on-failure` выводит этот отчёт.

В конце `test.cpp` есть дополнительный блок `static_assert` для типов константного и неконстантного доступа к элементу. Комментарии объясняют используемые средства проверки типов; этот блок можно пропустить при первом чтении. Проверки выполняются при компиляции и не отображаются как отдельные сценарии CTest.

Если исходники Google Test уже есть локально, можно конфигурировать без скачивания, добавив `-DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=/absolute/path/to/googletest`. Указывайте корень исходников с `CMakeLists.txt`, а не папку с собранными библиотеками.
