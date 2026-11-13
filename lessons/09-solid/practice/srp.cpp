#include "check.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <system_error>

namespace srp {
namespace bad {

class Library {
public:
    void lendBook(int readerId, int bookId, const std::string& filename) {
        if (readerId <= 0) {
            throw std::invalid_argument("Invalid reader");
        }
        const auto book = loans_.find(bookId);
        if (book == loans_.end()) {
            throw std::invalid_argument("Unknown book");
        }
        if (book->second != 0) {
            throw std::logic_error("Book is already on loan");
        }
        int borrowed = 0;
        for (const auto& loan : loans_) {
            if (loan.second == readerId) {
                ++borrowed;
            }
        }
        if (borrowed >= 3) {
            throw std::logic_error("Reader has reached the loan limit");
        }
        book->second = readerId;

        std::ofstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);
        file.open(filename);
        for (const auto& loan : loans_) {
            file << loan.first << ' ' << loan.second << '\n';
        }
        file.close();
    }

private:
    // Номер книги -> номер читателя; 0 означает, что книга свободна.
    std::map<int, int> loans_{{1, 0}, {2, 0}, {3, 0}, {4, 0}};
};

} // namespace bad
namespace good {

class Library {
public:
    void lendBook(int readerId, int bookId) {
        if (readerId <= 0) {
            throw std::invalid_argument("Invalid reader");
        }
        const auto book = loans_.find(bookId);
        if (book == loans_.end()) {
            throw std::invalid_argument("Unknown book");
        }
        if (book->second != 0) {
            throw std::logic_error("Book is already on loan");
        }
        int borrowed = 0;
        for (const auto& loan : loans_) {
            if (loan.second == readerId) {
                ++borrowed;
            }
        }
        if (borrowed >= 3) {
            throw std::logic_error("Reader has reached the loan limit");
        }
        book->second = readerId;
    }

    int borrowerOf(int bookId) const { return loans_.at(bookId); }
    std::map<int, int> loans() const { return loans_; }

private:
    std::map<int, int> loans_{{1, 0}, {2, 0}, {3, 0}, {4, 0}};
};

class LibraryStorage {
public:
    void save(const Library& library, const std::string& filename) const {
        std::ofstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);
        file.open(filename);
        for (const auto& loan : library.loans()) {
            file << loan.first << ' ' << loan.second << '\n';
        }
        file.close();
    }
};

} // namespace good

namespace {

// Инфраструктура демонстрации: файлы создаются только в новой временной папке.
class TemporaryDirectory {
public:
    TemporaryDirectory() {
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        for (int attempt = 0; attempt < 100; ++attempt) {
            const auto candidate = std::filesystem::temp_directory_path()
                / ("solid-library-" + std::to_string(stamp) + "-" + std::to_string(attempt));
            if (std::filesystem::create_directory(candidate)) {
                path_ = candidate;
                return;
            }
        }
        throw std::runtime_error("Cannot create demo directory");
    }

    ~TemporaryDirectory() {
        std::error_code ignored;
        std::filesystem::remove_all(path_, ignored);
    }

    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;

    std::filesystem::path file(const std::string& name) const { return path_ / name; }

private:
    std::filesystem::path path_;
};

std::string readFile(const std::filesystem::path& path) {
    std::ifstream file;
    file.exceptions(std::ios::failbit | std::ios::badbit);
    file.open(path);
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

} // namespace

void run() {
    std::cout << "== SRP ==\n";
    const TemporaryDirectory directory;
    const auto beforePath = directory.file("before.txt");
    const auto afterPath = directory.file("after.txt");

    bad::Library before;
    before.lendBook(7, 1, beforePath.string());
    good::Library after;
    after.lendBook(7, 1);
    check(after.borrowerOf(1) == 7, "SRP: record borrower in memory");
    check(!std::filesystem::exists(afterPath), "SRP: lending does not write a file");
    const good::LibraryStorage storage;
    storage.save(after, afterPath.string());
    const std::string expected = "1 7\n2 0\n3 0\n4 0\n";
    check(readFile(beforePath) == expected, "SRP: original persisted state");
    check(readFile(afterPath) == expected, "SRP: preserve persisted state");

    // Правила выдачи проверяются без LibraryStorage.
    bool unavailableRejected = false;
    try {
        after.lendBook(8, 1);
    } catch (const std::logic_error&) {
        unavailableRejected = true;
    }
    check(unavailableRejected && after.borrowerOf(1) == 7, "SRP: prevent double lending");
    after.lendBook(7, 2);
    after.lendBook(7, 3);
    bool limitRejected = false;
    try {
        after.lendBook(7, 4);
    } catch (const std::logic_error&) {
        limitRejected = true;
    }
    check(limitRejected && after.borrowerOf(4) == 0, "SRP: reject fourth loan without mutation");
    after.lendBook(8, 4);
    check(after.borrowerOf(4) == 8, "SRP: loan limit belongs to each reader");
    check(readFile(afterPath) == expected, "SRP: further lending remains in memory until save");

    bool storageFailureReported = false;
    try {
        storage.save(after, (directory.file("missing") / "loans.txt").string());
    } catch (const std::ios_base::failure&) {
        storageFailureReported = true;
    }
    check(storageFailureReported, "SRP: storage reports write errors");

    std::cout << "book 1 borrower: " << after.borrowerOf(1) << '\n';
    std::cout << "saved states match: yes\n";
    std::cout << "unavailable book rejected: yes\n";
    std::cout << "fourth loan rejected: yes\n";
}

} // namespace srp
