#include "Processor.hpp"
#include <iostream>

int main() {
    Processor cpu = Processor{};

    cpu.reset();

    // cpu.step();

    cpu.state();

    return 0;
}
