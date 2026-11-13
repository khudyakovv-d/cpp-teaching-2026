#include "check.h"

#include <iostream>
#include <stdexcept>

namespace ocp {
namespace bad {

enum class Delivery { Pickup, Courier };

// Стоимость заказа и все варианты доставки меняются в одном месте.
int orderTotal(int goods, Delivery delivery, int kilograms) {
    switch (delivery) {
    case Delivery::Pickup:
        return goods;
    case Delivery::Courier:
        return goods + 200 + 30 * kilograms;
    }
    throw std::invalid_argument("Unknown delivery");
}

} // namespace bad
namespace good {

class DeliveryFee {
public:
    virtual ~DeliveryFee() = default;
    virtual int calculate(int kilograms) const = 0;
};

class Pickup final : public DeliveryFee {
public:
    int calculate(int) const override { return 0; }
};

class Courier final : public DeliveryFee {
public:
    int calculate(int kilograms) const override { return 200 + 30 * kilograms; }
};

int orderTotal(int goods, const DeliveryFee& delivery, int kilograms) {
    return goods + delivery.calculate(kilograms);
}

// Новый тариф добавлен без изменения orderTotal, Pickup и Courier.
class Express final : public DeliveryFee {
public:
    int calculate(int kilograms) const override { return 500 + 50 * kilograms; }
};

} // namespace good

void run() {
    std::cout << "== OCP ==\n";
    const good::Pickup pickup;
    const good::Courier courier;
    const good::Express express;
    check(good::orderTotal(1000, pickup, 2)
        == bad::orderTotal(1000, bad::Delivery::Pickup, 2), "OCP: preserve pickup");
    check(good::orderTotal(1000, courier, 2)
        == bad::orderTotal(1000, bad::Delivery::Courier, 2), "OCP: preserve courier");
    check(good::orderTotal(1000, express, 2) == 1600, "OCP: new delivery");
    std::cout << "pickup: " << good::orderTotal(1000, pickup, 2) << '\n';
    std::cout << "courier: " << good::orderTotal(1000, courier, 2) << '\n';
    std::cout << "express: " << good::orderTotal(1000, express, 2) << '\n';
}

} // namespace ocp
