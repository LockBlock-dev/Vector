#include "Processor.hpp"
#include <iostream>

int main() {
    Processor cpu = Processor{};

    cpu.reset();

    cpu.load_elf("./tests/helloworld.out");

    for (int i = 0; i < 9; ++i)
    {
        cpu.step();
        // cpu.state();
    }

    cpu.state();

    return 0;
}
