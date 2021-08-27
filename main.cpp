#include <iostream>
#include <IGate.hpp>

int main(int argc, const char** argv)
{
    uint8_t high = 11;
    uint8_t low  = 0;
    std::cout << !high << std::endl;
    std::cout << !low << std::endl;
    return 0;
}
