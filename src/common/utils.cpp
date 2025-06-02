#include <baort/common/utils.h>
#include <iostream>

void baort::utils::eval(asmjit::Error err, std::string location) {
    if (err > 0) {
        std::cerr << "[" << location << "] JIT failed: " << asmjit::DebugUtils::errorAsString(err) << "\n";
        exit(1);
    }
}