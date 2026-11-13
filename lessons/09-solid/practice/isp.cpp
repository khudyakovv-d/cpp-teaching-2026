#include "check.h"

#include <iostream>
#include <sstream>
#include <string>

namespace isp {
namespace bad {

class OfficeDevice {
public:
    virtual ~OfficeDevice() = default;
    virtual void print(const std::string& text) = 0;
    virtual std::string scan() = 0;
};

// Обе возможности устройства поддерживаются.
class AllInOne final : public OfficeDevice {
public:
    explicit AllInOne(std::ostream& output) : output_(output) {}
    void print(const std::string& text) override { output_ << text << '\n'; }
    std::string scan() override { return "Scanned document"; }

private:
    std::ostream& output_;
};

void printReport(OfficeDevice& device) { device.print("Report"); }
std::string scanDocument(OfficeDevice& device) { return device.scan(); }

// Попытка подключить устройство, которое умеет только печатать.
class SimplePrinter : public OfficeDevice {
public:
    explicit SimplePrinter(std::ostream& output) : output_(output) {}
    void print(const std::string& text) override { output_ << text << '\n'; }
    // Реализовать scan невозможно: в устройстве нет сканера.

private:
    std::ostream& output_;
}; // Класс остался абстрактным из-за OfficeDevice::scan().

} // namespace bad
namespace good {

class Printer {
public:
    virtual ~Printer() = default;
    virtual void print(const std::string& text) = 0;
};

class Scanner {
public:
    virtual ~Scanner() = default;
    virtual std::string scan() = 0;
};

class SimplePrinter final : public Printer {
public:
    explicit SimplePrinter(std::ostream& output) : output_(output) {}
    void print(const std::string& text) override { output_ << text << '\n'; }

private:
    std::ostream& output_;
};

// Учебная имитация устройства: отсканированное содержимое фиксировано.
class AllInOne final : public Printer, public Scanner {
public:
    explicit AllInOne(std::ostream& output) : output_(output) {}
    void print(const std::string& text) override { output_ << text << '\n'; }
    std::string scan() override { return "Scanned document"; }

private:
    std::ostream& output_;
};

void printReport(Printer& printer) { printer.print("Report"); }
std::string scanDocument(Scanner& scanner) { return scanner.scan(); }

} // namespace good

void run() {
    std::cout << "== ISP ==\n";
    std::ostringstream beforeOutput;
    bad::AllInOne before(beforeOutput);
    bad::printReport(before);
    check(beforeOutput.str() == "Report\n", "ISP: original printing");
    check(bad::scanDocument(before) == "Scanned document", "ISP: original scanning");
    std::cout << "before printing: " << beforeOutput.str();
    // bad::SimplePrinter unsupported(beforeOutput); // Не компилируется: абстрактный класс.

    std::ostringstream afterOutput;
    good::SimplePrinter printer(afterOutput);
    good::AllInOne combined(afterOutput);
    good::printReport(printer);
    check(afterOutput.str() == beforeOutput.str(), "ISP: preserve printing");
    std::cout << "after printing: " << afterOutput.str();
    good::printReport(combined);
    check(afterOutput.str() == "Report\nReport\n", "ISP: both devices print");
    check(good::scanDocument(combined) == "Scanned document", "ISP: scanner client");
    // good::scanDocument(printer); // Ошибка компиляции: нет интерфейса Scanner.
    std::cout << "after scanning: " << good::scanDocument(combined) << '\n';
}

} // namespace isp
