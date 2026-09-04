# Настройка окружения для курса

Для работы понадобятся четыре компонента:

- Visual Studio Code — редактор;
- компилятор C++ — создаёт машинный код;
- CMake — читает описание проекта;
- расширения VS Code — добавляют команды сборки, запуска и отладки.

В Windows компилятор и системные инструменты сборки устанавливаются вместе с Visual Studio Build Tools. В macOS они входят в Command Line Tools for Xcode.

Visual Studio Code и Visual Studio Build Tools — разные программы. Код мы открываем в Visual Studio Code, а Build Tools предоставляет компилятор и другие инструменты для Windows. Команды в терминале ниже нужны только для установки и проверки окружения; проекты курса собираются через интерфейс Visual Studio Code.

## Windows

Сначала установите [Visual Studio Code](https://code.visualstudio.com/Download):

1. Скачайте **User Installer**. Для компьютеров с процессором Intel или AMD подходит `Windows x64`, для Windows on ARM — `Arm64`.
2. Запустите установщик.
3. Оставьте включёнными пункты добавления VS Code в контекстное меню и в `PATH`, если установщик их предлагает.

Затем установите компилятор:

1. Откройте страницу [Visual Studio Downloads](https://visualstudio.microsoft.com/downloads/).
2. В разделе **Tools for Visual Studio** скачайте **Build Tools for Visual Studio**.
3. Запустите Visual Studio Installer и выберите **Desktop development with C++**.
4. Проверьте, что выбраны MSVC C++ Build Tools и Windows SDK.
5. Нажмите **Install** и дождитесь завершения.

Полная среда Visual Studio не нужна. Из Build Tools нам требуются компилятор MSVC, линковщик, Windows SDK и MSBuild.

После этого откройте PowerShell и установите CMake:

```powershell
winget install --id Kitware.CMake -e
```

Если система запросит подтверждение источника или права администратора, подтвердите установку. Затем полностью закройте и снова откройте VS Code, чтобы он увидел новые программы.

## macOS

Сначала установите [Visual Studio Code](https://code.visualstudio.com/Download):

1. Скачайте версию для macOS.
2. Откройте `.dmg` и перенесите `Visual Studio Code.app` в `Applications`.
3. Запустите VS Code из `Applications` или через Spotlight.

Затем установите Command Line Tools for Xcode. Откройте Terminal и выполните:

```bash
xcode-select --install
```

Подтвердите установку в появившемся системном окне. Полный Xcode не нужен: Command Line Tools уже содержит компилятор Apple Clang, отладчик LLDB и Make.

Для установки CMake удобно использовать [Homebrew](https://brew.sh/). Если Homebrew ещё не установлен, сначала выполните инструкцию на его официальном сайте.

```bash
brew install cmake
```

После установки полностью закройте и снова откройте VS Code.

## Расширения Visual Studio Code

Откройте раздел **Extensions** и установите два расширения от Microsoft:

1. [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) — подсветка, подсказки и отладка C++.
2. [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) — подготовка, сборка, запуск и отладка CMake-проектов.

Расширение **Code Runner** не требуется. Оно может попытаться собрать только открытый `.cpp`, не учитывая остальные файлы проекта.

Расширение C/C++ также не устанавливает компилятор. Оно использует MSVC в Windows или Apple Clang в macOS, установленные на предыдущих шагах.

## Проверка установки

Откройте встроенный терминал через **Terminal → New Terminal**.

В Windows выполните:

```powershell
cmake --version
```

В macOS выполните:

```bash
clang++ --version
cmake --version
make --version
```

Каждая команда должна вывести информацию об установленной программе. Для проектов курса требуется CMake версии 3.27 или новее.

В обычном терминале Windows команда `cl` может быть недоступна, даже если MSVC установлен. Это нормально: CMake Tools подготавливает окружение Visual Studio самостоятельно.

Теперь откройте палитру команд:

- Windows: `Ctrl+Shift+P`;
- macOS: `Cmd+Shift+P`.

Введите `CMake:`. В списке должны появиться `CMake: Configure`, `CMake: Build` и другие действия. На левой панели также появится раздел CMake Tools.

## Если что-то не найдено

Сначала полностью перезапустите VS Code. Открытый редактор не всегда замечает программы, установленные после его запуска.

Если это не помогло:

- в Windows проверьте в Visual Studio Installer компонент **Desktop development with C++**, затем выполните `CMake: Scan for Compilers`;
- в macOS убедитесь, что установка Command Line Tools завершилась без ошибки;
- проверьте, что расширения C/C++ и CMake Tools установлены и включены;
- сохраните полный текст ошибки — по нему можно определить, какой компонент не найден.

После этих проверок окружение готово. Настройку и сборку первого проекта мы разберём на занятии.

Официальные инструкции: [Visual Studio Code](https://code.visualstudio.com/docs/getstarted/overview), [MSVC Build Tools](https://learn.microsoft.com/en-us/cpp/overview/acquire-msvc), [Apple Clang для VS Code](https://code.visualstudio.com/docs/cpp/config-clang-mac) и [CMake](https://cmake.org/download/).
