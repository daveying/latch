#include <iostream>
#include <NORGate.hpp>

int main(int argc, const char** argv)
{
    gate::NORGate norGate0;
    gate::NORGate norGate1;
    norGate0.output(0)->connect(norGate1.input(0));
    norGate1.output(0)->connect(norGate0.input(1));
    norGate0.compute();
    return 0;
}
