#include <exception>
#include <iostream>

namespace srp { void run(); }
namespace ocp { void run(); }
namespace lsp { void run(); }
namespace isp { void run(); }
namespace dip { void run(); }

int main() {
    try {
        srp::run();
        ocp::run();
        lsp::run();
        isp::run();
        dip::run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
