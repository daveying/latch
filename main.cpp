#include <iostream>
#include <NORGate.hpp>

int main(int argc, const char** argv)
{
    gate::NORGate norGate0;
    gate::NORGate norGate1;
    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));
    norGate0.input(0)->value(gate::PinState::High);
    norGate1.input(1)->value(gate::PinState::Low);
    norGate0.compute();
    std::cout << static_cast<int>(norGate0.output(0)->value()) << std::endl;
    std::cout << static_cast<int>(norGate1.output(0)->value()) << std::endl;
    return 0;
}
