#include <iostream>
#include <NORGate.hpp>

int main(int argc, const char** argv)
{
    gate::NORGate norGate0;
    gate::NORGate norGate1;
    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));

    // S High
    norGate0.input(0)->value(gate::PinState::High);
    // R Low
    norGate1.input(1)->value(gate::PinState::Low);
    // S High, R Low
    std::cout << "S High, R Low" << std::endl;
    std::cout << static_cast<int>(norGate0.output(0)->value()) << std::endl;
    std::cout << static_cast<int>(norGate1.output(0)->value()) << std::endl;

    // S Low
    norGate0.input(0)->value(gate::PinState::Low);
    // S Low, R Low
    std::cout << "S Low, R Low" << std::endl;
    std::cout << static_cast<int>(norGate0.output(0)->value()) << std::endl;
    std::cout << static_cast<int>(norGate1.output(0)->value()) << std::endl;
    // R High
    norGate1.input(1)->value(gate::PinState::High);
    // S Low, R High
    std::cout << "S Low, R High" << std::endl;
    std::cout << static_cast<int>(norGate0.output(0)->value()) << std::endl;
    std::cout << static_cast<int>(norGate1.output(0)->value()) << std::endl;
    // S High
    norGate0.input(0)->value(gate::PinState::High);
    // S High, R High
    std::cout << "S High, R High" << std::endl;
    std::cout << static_cast<int>(norGate0.output(0)->value()) << std::endl;
    std::cout << static_cast<int>(norGate1.output(0)->value()) << std::endl;
    return 0;
}
