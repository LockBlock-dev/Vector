#include <print>
#include <Emulator.hpp>

int main()
{
    Emulator emulator;

    emulator.reset();

    emulator.load_elf("./tests/helloworld.out");

    emulator.run();

    emulator.state();

    return 0;
}
