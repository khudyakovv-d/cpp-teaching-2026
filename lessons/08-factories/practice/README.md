# Демонстрация фабрик объектов

Проект показывает использование готового продукта, отдельные фабрики, одну фабрику с выбором типа и реестр настроенных фабрик. Дополнительный вариант разделяет настройки фабрики и параметры операции. Все примеры используют одни определения `Product`, `ProductA` и `ProductB` в [main.cpp](main.cpp).

Откройте эту папку в VS Code как CMake-проект и соберите цель `factories_demo`. Из терминала:

```sh
cmake -S . -B build
cmake --build build --config Debug
```

После сборки запустите программу:

- macOS и Linux с Make или Ninja: `./build/factories_demo`;
- Windows с генератором Visual Studio: `.\build\Debug\factories_demo.exe`;
- Windows с Ninja: `.\build\factories_demo.exe`.

Ожидаемый вывод:

```text
== existing product ==
ProductA(value=10)
ProductA(value=10)
== separate factories ==
operation 1: ProductA(value=10)
operation 2: ProductA(value=10)
operation 1: ProductB(text=demo, count=2)
operation 2: ProductB(text=demo, count=2)
== parameterized factory ==
ProductA(value=10)
ProductB(text=demo, count=2)
== registry of factories ==
ProductA(value=25)
ProductB(text=hello, count=3)
same product: no
old product: ProductA(value=25)
new product: ProductA(value=99)
== registry errors ==
Product type is not registered
Factory must not be null
== operation parameters ==
ProductA(value=10)
ProductA(value=20)
Value is outside the factory limit
```

`runExistingProduct()` дважды передаёт в `useProduct()` один готовый объект. Функции достаточно `Product&`, потому что создание остаётся у вызывающего кода.

`runSeparateFactories()` передаёт фабрики в `ProductClient`. В `run(2)` клиент сам создаёт отдельный продукт для каждой из двух операций и удаляет его в конце итерации. Конкретный класс выбирает фабрика. Фабрики объявлены раньше клиентов и живут дольше хранящихся у клиентов ссылок.

`runParameterizedFactory()` выбирает продукт аргументом `ProductType`. Внутри одной фабрики работает `switch`; начальные параметры продуктов заданы в его ветвях.

`runRegistryFactory()` регистрирует те же конкретные фабрики с другими настройками. Реестр передаёт создание найденной фабрике. Два одновременно живущих продукта A имеют разные адреса: каждый вызов создаёт новый объект. Затем повторная регистрация заменяет фабрику A, и следующий продукт получает значение `99`. Уже выданный продукт сохраняет `25`.

`runRegistryErrors()` показывает отказы при отсутствии регистрации B и попытке добавить пустую фабрику. Исключения перехвачены рядом с демонстрационными вызовами, поэтому оба сообщения видны за один запуск.

`runOperationParameters()` задаёт постоянный предел `100` для `OperationFactoryA`, затем создаёт продукты со значениями `10` и `20`. Значение `101` отклоняется исключением. Один настроенный создатель обслуживает несколько операций с разными данными.

Общий договор фабрик: успех возвращает непустой владеющий указатель на новый продукт, ошибка передаётся исключением. Реестр владеет фабриками, а выданные продукты имеют отдельных владельцев.

## Выбор продукта из командной строки

Без аргументов программа запускает все демонстрации. Один аргумент `a` или `b` запускает только создание выбранного продукта. На macOS или Linux:

```sh
./build/factories_demo a
./build/factories_demo b
```

Вывод первой команды:

```text
ProductA(value=10)
```

Вывод второй команды:

```text
ProductB(text=demo, count=2)
```

`main` проверяет число аргументов и передаёт `argv[1]` в `runSelectedProduct()`. Затем `parseProductType()` проверяет имя, фабрика создаёт продукт, а `useProduct()` использует общий интерфейс. В Windows можно передать те же аргументы после соответствующего пути к `.exe`.

Для `./build/factories_demo unknown` программа выводит в `std::cerr`:

```text
Unknown product name: unknown
```

Код завершения — `1`. Лишние аргументы также дают код `1` и сообщение `Usage: factories_demo [a|b]`. Успешные запуски возвращают `0`.

## Упражнение: добавить продукт C

Нужно поддержать `ProductC(bool enabled)`, описывающий себя как `ProductC(enabled=yes)` или `ProductC(enabled=no)`. Проведите его через все три основных варианта создания:

1. Добавьте `ProductC` и `FactoryC`, хранящую флаг. Передайте фабрику существующему `ProductClient` и выполните две операции.
2. Добавьте `ProductType::C` и ветвь параметризованной фабрики. Для неё выберите `enabled = true`.
3. Зарегистрируйте `FactoryC(false)` в реестре и создайте C через общий поиск.
4. Поддержите имя `c` в `parseProductType()` и обновите подсказку о допустимых аргументах. Проверьте запуск программы с аргументом `c` и отказ для неизвестного имени.

Для `./build/factories_demo c` ожидается:

```text
ProductC(enabled=yes)
```

Для создания через реестр с зарегистрированной `FactoryC(false)` ожидается `ProductC(enabled=no)`. Эти проверки относятся к результату выполнения упражнения; исходная демонстрация поддерживает только A и B.

**Проверка решения:** новый продукт и его конкретная фабрика добавляются отдельно. В параметризованной фабрике меняется `switch`, в реестре — набор регистраций. Общие `Product`, `ProductClient` и `RegistryFactory::create()` остаются прежними. Объясните, почему место подключения нового типа есть во всех вариантах, хотя код клиента может не меняться.
