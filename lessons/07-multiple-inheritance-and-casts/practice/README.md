# Демонстрация наследования и приведений по иерархии

Проект следует порядку теории: доступность базы, несколько базовых частей, затем преобразования по иерархии. Разные версии классов разделены пространствами имён в [main.cpp](main.cpp).

Откройте эту папку в VS Code как CMake-проект и соберите цель `casts_demo`. Из терминала:

```sh
cmake -S . -B build
cmake --build build --config Debug
```

После сборки запустите программу:

- macOS и Linux с Make или Ninja: `./build/casts_demo`;
- Windows с генератором Visual Studio: `.\build\Debug\casts_demo.exe`;
- Windows с Ninja: `.\build\casts_demo.exe`.

Ожидаемый вывод:

```text
== inheritance access ==
public value = 1
public base view = 1
private first ticket = 1
private second ticket = 2
protected reading = 1
protected descendant base view = 1
== two base interfaces ==
study
work
student card
employee card
== ordinary diamond ==
student name = Анна Смирнова
employee name = Анна Смирнова
after student update:
student name = Анна Иванова
employee name = Анна Смирнова
== virtual diamond ==
standalone Student
Person(Борис)
Student
name = Борис
~Student
~Person
complete TeachingAssistant
Person(Анна Смирнова)
Student
Employee
TeachingAssistant
student name = Анна Смирнова
employee name = Анна Смирнова
after student update:
student name = Анна Иванова
employee name = Анна Иванова
virtual role: assistant
~TeachingAssistant
~Employee
~Student
~Person
== upcast and downcast ==
virtual area = 12.57
radius = 2.00
not a circle
not a circle
required radius = 2.00
circle required
static radius = 2.00
== cross-cast ==
work
not employed
== downcast through virtual base ==
Person(Анна Смирнова)
Student
Employee
TeachingAssistant
recover assistant: yes
Person(Борис)
Student
standalone is assistant: no
~Student
~Person
~TeachingAssistant
~Employee
~Student
~Person
== const and ownership ==
const radius = 2.00
owned radius = 5.00
owner empty: yes
```

В `access_modes` счётчик посетителей доступен через публичную базу `Counter`. Аппарат выдачи талонов использует её закрыто, а измеритель событий получает доступ к ней через защищённое наследование промежуточного класса. Запрещённые внешние обращения оставлены в комментариях.

В `university_roles` ассистент одновременно подходит к интерфейсам `Student` и `Employee`. Два метода `printCard()` требуют выбрать студенческую карточку или карточку сотрудника. Затем обычный и виртуальный ромб продолжают пример с общим классом `Person`.

При обычном наследовании обе части сначала содержат одинаковое имя «Анна Смирнова». После изменения через `Student` эта ветвь хранит «Анна Иванова», а `Employee` — прежнее имя. При виртуальном наследовании строка одна, поэтому изменение видно через обе роли. В теории этому соответствуют отдельная диаграмма ромба и сравнение частей объекта.

Печатающие конструкторы и деструкторы показывают, что самостоятельный `Student` создаёт свою `Person`, а в полном ассистенте её создаёт `TeachingAssistant`. Общая база создаётся первой и уничтожается последней, один раз. Дополнительный виртуальный `role()` переопределён в обеих ветвях и в самом ассистенте, который задаёт единственную итоговую реализацию.

После разбора частей объекта блок `shapes` показывает успешный и неуспешный `dynamic_cast` указателя и ссылки, нулевой указатель и статический спуск при известном типе.

Затем функция с параметром `const Student&` проверяет наличие роли `Employee`. У ассистента проверка успешна, у самостоятельного студента — нет. Следующий блок выполняет спуск из общей виртуальной `Person` к `TeachingAssistant`; прямой статический спуск через такую базу был бы ошибкой компиляции.

Завершающий блок показывает сохранение `const` и владения. Приведение результата `owner.get()` получает невладеющий указатель. После удаления объекта этот указатель не используется. Порядок этих блоков совпадает с порядком третьей части теории.

Строки с ошибками компиляции закомментированы с указанием причины. Ошибочный статический спуск также не запускается: неопределённое поведение не имеет ожидаемого результата. Ограничения `const_cast`, `reinterpret_cast` и записи в стиле C кратко разобраны в конце теории; для выполнения демонстрации эти формы не требуются.
