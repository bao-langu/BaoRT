#include <baort/common/utils.h>
#include <iostream>

void baort::utils::eval(asmjit::Error err, std::string location) {
    if (err > 0) {
        std::cerr << "[" << location << "] JIT failed: " << asmjit::DebugUtils::errorAsString(err) << "\n";
        exit(1);
    }
}

void baort::utils::Logger::info(const std::string &msg) {
    std::cout << "[INFO] " << __FILE__ << ":" << __LINE__ << " in " << __func__ << "() -> " << msg << std::endl;
}