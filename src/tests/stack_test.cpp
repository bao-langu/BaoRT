#include <iostream>
#include <cstdio>

#include <asmjit/core/globals.h>
#include <asmjit/core/operand.h>
#include <baort/common/utils.h>
#include <asmjit/arm/a64assembler.h>
#include <asmjit/core/codeholder.h>
#include <baort/tests/stack_test.h>
#include <asmjit/core/jitruntime.h>
#include <asmjit/core/logger.h>

using baort::utils::Logger;

int stack_test() {
    Logger::info("Starting stack test...");
    
    asmjit::JitRuntime rt;
    asmjit::CodeHolder code;
    code.init(rt.environment());
    asmjit::StringLogger logger;
    code.setLogger(&logger);

    asmjit::a64::Assembler a(&code);

    const char* msg = "BaoRT is printing\n";

    // Set up main stack frame - ensure 16-byte alignment
    baort::utils::eval(
        a.sub(asmjit::a64::sp, asmjit::a64::sp, 16)
    );
    baort::utils::eval(
        a.stp(asmjit::a64::x29, asmjit::a64::x30, asmjit::a64::Mem(asmjit::a64::sp))
    );
    baort::utils::eval(
        a.mov(asmjit::a64::x29, asmjit::a64::sp)
    );

    // Save callee-saved registers we'll use
    baort::utils::eval(
        a.sub(asmjit::a64::sp, asmjit::a64::sp, 16)
    );
    baort::utils::eval(
        a.str(asmjit::a64::x16, asmjit::a64::Mem(asmjit::a64::sp, 8))
    );

    // Set up printf arguments
    baort::utils::eval(
        a.mov(asmjit::a64::x16, (uint64_t)printf)  // Store printf address in callee-saved register
    );
    baort::utils::eval(
        a.mov(asmjit::a64::x0, (uint64_t)msg)
    );

    // Call printf
    baort::utils::eval(
        a.blr(asmjit::a64::x16)
    );

    // Restore callee-saved registers
    baort::utils::eval(
        a.ldr(asmjit::a64::x16, asmjit::a64::Mem(asmjit::a64::sp, 8))
    );
    baort::utils::eval(
        a.add(asmjit::a64::sp, asmjit::a64::sp, 16)
    );

    // Set return value
    baort::utils::eval(
        a.mov(asmjit::a64::x0, 0)
    );

    // Restore frame and return
    baort::utils::eval(
        a.ldp(asmjit::a64::x29, asmjit::a64::x30, asmjit::a64::Mem(asmjit::a64::sp))
    );
    baort::utils::eval(
        a.add(asmjit::a64::sp, asmjit::a64::sp, 16)
    );
    baort::utils::eval(
        a.ret(asmjit::a64::x30)
    );

    void* bao_func;
    auto err = rt.add(&bao_func, &code);
    if (err) {
        std::cerr << "Failed to add function: " << asmjit::DebugUtils::errorAsString(err) << std::endl;
        return 1;
    }

    // std::cout << "Function assembled at: " << bao_func << std::endl;
    // std::cout << "Generated assembly:\n" << logger.data() << std::endl;

    auto func = asmjit::ptr_as_func<int(*)()>(bao_func);
    int result = func();
    Logger::info(std::format("BaoRT Result: {}", result));

    baort::utils::eval(rt.release(bao_func));
    return 0;
}