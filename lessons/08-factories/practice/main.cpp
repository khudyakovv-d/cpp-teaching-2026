#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

enum class ProductType { A, B };

class Product {
public:
    virtual ~Product() = default;
    virtual std::string describe() const = 0;
};

class ProductA final : public Product {
public:
    explicit ProductA(int value) : value_(value) {}

    std::string describe() const override {
        return "ProductA(value=" + std::to_string(value_) + ")";
    }

private:
    int value_;
};

class ProductB final : public Product {
public:
    ProductB(std::string text, int count)
        : text_(std::move(text)), count_(count) {}

    std::string describe() const override {
        return "ProductB(text=" + text_ + ", count="
            + std::to_string(count_) + ")";
    }

private:
    std::string text_;
    int count_;
};

class Factory {
public:
    virtual ~Factory() = default;
    // Успех: новый продукт и непустой владеющий указатель.
    // Ошибка создания: исключение; nullptr не является результатом отказа.
    virtual std::unique_ptr<Product> create() const = 0;
};

class FactoryA final : public Factory {
public:
    explicit FactoryA(int value) : value_(value) {}

    std::unique_ptr<Product> create() const override {
        return std::make_unique<ProductA>(value_);
    }

private:
    int value_;
};

class FactoryB final : public Factory {
public:
    FactoryB(std::string text, int count)
        : text_(std::move(text)), count_(count) {}

    std::unique_ptr<Product> create() const override {
        return std::make_unique<ProductB>(text_, count_);
    }

private:
    std::string text_;
    int count_;
};

void useProduct(const Product& product) {
    std::cout << product.describe() << '\n';
}

class ProductClient {
public:
    explicit ProductClient(const Factory& factory) : factory_(factory) {}

    void run(int operations) const {
        for (int index = 0; index < operations; ++index) {
            auto product = factory_.create();
            std::cout << "operation " << index + 1 << ": ";
            useProduct(*product);
        }
    }

private:
    // Фабрика живёт дольше клиента; продуктом владеет отдельный unique_ptr.
    const Factory& factory_;
};

class ParameterizedFactory {
public:
    std::unique_ptr<Product> create(ProductType type) const {
        switch (type) {
        case ProductType::A:
            return std::make_unique<ProductA>(10);
        case ProductType::B:
            return std::make_unique<ProductB>("demo", 2);
        }
        throw std::invalid_argument("Unknown product type");
    }
};

ProductType parseProductType(const std::string& name) {
    if (name == "a") {
        return ProductType::A;
    }
    if (name == "b") {
        return ProductType::B;
    }
    throw std::invalid_argument("Unknown product name: " + name);
}

void runSelectedProduct(const std::string& name) {
    const ProductType type = parseProductType(name);
    ParameterizedFactory factory;
    auto product = factory.create(type);
    useProduct(*product);
}

class RegistryFactory {
public:
    void add(ProductType type, std::unique_ptr<Factory> factory) {
        if (!factory) {
            throw std::invalid_argument("Factory must not be null");
        }

        // Повторная регистрация заменяет фабрику для этого типа.
        factories_.insert_or_assign(type, std::move(factory));
    }

    std::unique_ptr<Product> create(ProductType type) const {
        const auto found = factories_.find(type);
        if (found == factories_.end()) {
            throw std::invalid_argument("Product type is not registered");
        }

        return found->second->create();
    }

private:
    std::map<ProductType, std::unique_ptr<Factory>> factories_;
};

// Отдельный вариант: limit_ постоянен, value меняется при каждой операции.
// create(int) имеет другой интерфейс и не переопределяет Factory::create().
class OperationFactoryA {
public:
    explicit OperationFactoryA(int limit) : limit_(limit) {
        if (limit < 0) {
            throw std::invalid_argument("Factory limit must be non-negative");
        }
    }

    std::unique_ptr<Product> create(int value) const {
        if (value < 0 || value > limit_) {
            throw std::invalid_argument("Value is outside the factory limit");
        }
        return std::make_unique<ProductA>(value);
    }

private:
    int limit_;
};

void runExistingProduct() {
    ProductA product(10);
    useProduct(product);
    useProduct(product);
}

void runSeparateFactories() {
    FactoryA factoryA(10);
    FactoryB factoryB("demo", 2);
    ProductClient clientA(factoryA);
    ProductClient clientB(factoryB);
    clientA.run(2);
    clientB.run(2);
}

void runParameterizedFactory() {
    ParameterizedFactory factory;
    auto first = factory.create(ProductType::A);
    auto second = factory.create(ProductType::B);
    std::cout << first->describe() << '\n';
    std::cout << second->describe() << '\n';
}

void runRegistryFactory() {
    RegistryFactory factory;
    factory.add(ProductType::A, std::make_unique<FactoryA>(25));
    factory.add(ProductType::B, std::make_unique<FactoryB>("hello", 3));

    auto first = factory.create(ProductType::A);
    auto second = factory.create(ProductType::B);
    std::cout << first->describe() << '\n';
    std::cout << second->describe() << '\n';

    auto another = factory.create(ProductType::A);
    std::cout << "same product: "
              << (first.get() == another.get() ? "yes" : "no") << '\n';

    factory.add(ProductType::A, std::make_unique<FactoryA>(99));
    auto updated = factory.create(ProductType::A);
    std::cout << "old product: " << first->describe() << '\n';
    std::cout << "new product: " << updated->describe() << '\n';
}

void runRegistryErrors() {
    RegistryFactory factory;
    factory.add(ProductType::A, std::make_unique<FactoryA>(10));

    try {
        auto product = factory.create(ProductType::B);
        std::cout << product->describe() << '\n';
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << '\n';
    }

    try {
        factory.add(ProductType::B, nullptr);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << '\n';
    }
}

void runOperationParameters() {
    OperationFactoryA factory(100);
    auto first = factory.create(10);
    auto second = factory.create(20);
    useProduct(*first);
    useProduct(*second);

    try {
        auto product = factory.create(101);
        useProduct(*product);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << '\n';
    }
}

int main(int argc, char** argv) {
    try {
        if (argc == 1) {
            std::cout << "== existing product ==\n";
            runExistingProduct();
            std::cout << "== separate factories ==\n";
            runSeparateFactories();
            std::cout << "== parameterized factory ==\n";
            runParameterizedFactory();
            std::cout << "== registry of factories ==\n";
            runRegistryFactory();
            std::cout << "== registry errors ==\n";
            runRegistryErrors();
            std::cout << "== operation parameters ==\n";
            runOperationParameters();
            return 0;
        }
        if (argc != 2) {
            throw std::invalid_argument("Usage: factories_demo [a|b]");
        }
        runSelectedProduct(argv[1]);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
