#ifndef UTILS_H
#define UTILS_H
#include <asmjit/core/errorhandler.h>
#include <string>
namespace baort::utils {
    void eval(asmjit::Error err, std::string location = "not specified");

    class Logger {
    public:
        static void info(const std::string& msg);
    };
}
#endif