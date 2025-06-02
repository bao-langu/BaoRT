#include <iostream>
#include <baort/tests/print_test.h>
#include <baort/common/utils.h>
#include <asmjit/core/jitruntime.h>
#include <asmjit/arm/a64assembler.h>

int print_test() {
    asmjit::JitRuntime rt;
    asmjit::CodeHolder code;
    code.init(rt.environment());

    asmjit::a64::Assembler a(&code);

    const char* msg = "Hello from BaoRT ARM64!\n";
    const char* msg2 = "This is second message\n";

    baort::utils::eval(
        a.mov(asmjit::a64::x0, (uint64_t)msg), 
        "move x0 <- msg"
    );

    baort::utils::eval(
        a.mov(asmjit::a64::x19, asmjit::a64::x30),
        "move x19 <- x30"
    ); // Remember the return address

    // Load printf address into x16
    baort::utils::eval(
        a.mov(asmjit::a64::x16, (uint64_t)printf), 
        "move x16 <- printf"
    );

    // Align stack: subtract 16 to keep it 16-byte aligned before the call
    baort::utils::eval(
        a.sub(asmjit::a64::sp, asmjit::a64::sp, 16), 
        "sub sp, 16"
    );

    // Call printf (imported from libc)
    baort::utils::eval(
        a.blr(asmjit::a64::x16), 
        "blr x16 (printf)"
    );

    // Restore stack
    baort::utils::eval(
        a.add(asmjit::a64::sp, asmjit::a64::sp, 16), 
        "add sp, 16"
    );

    baort::utils::eval(
        a.mov(asmjit::a64::x0, (uint64_t)msg2), 
        "move x0 <- msg2"
    );

    // Load printf address into x16
    baort::utils::eval(
        a.mov(asmjit::a64::x16, (uint64_t)printf), 
        "move x16 <- printf"
    );

    // Align stack: subtract 16 to keep it 16-byte aligned before the call
    baort::utils::eval(
        a.sub(asmjit::a64::sp, asmjit::a64::sp, 16), 
        "sub sp, 16"
    );

    // Call printf (imported from libc)
    baort::utils::eval(
        a.blr(asmjit::a64::x16), 
        "blr x16 (printf)"
    );

    // Restore stack
    baort::utils::eval(
        a.add(asmjit::a64::sp, asmjit::a64::sp, 16), 
        "add sp, 16"
    );

    // Return
    baort::utils::eval(
        a.mov(asmjit::a64::x0, 0), 
        "move x0 <- 0"
    );
    baort::utils::eval(
        a.mov(asmjit::a64::x30, asmjit::a64::x19),
        "move x30 <- x19"
    ); // Get back the return address
    baort::utils::eval(
        a.ret(asmjit::a64::x30), 
        "return x30"
    ); // Use x30 (link register)

    // Finalize code
    void* BaoRT;
    baort::utils::eval(rt.add(&BaoRT, &code), "store code -> BaoRT");

    auto foo = asmjit::ptr_as_func<int(*)()>(BaoRT);
    int result = foo();
    std::cout << "BaoRT Result: " << result << std::endl;

    baort::utils::eval(rt.release(BaoRT)); // free memory
    return 0;
}