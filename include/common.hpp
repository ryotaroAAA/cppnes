#ifndef COMMON
#define COMMON

#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdint>
using namespace std;

struct reg {
    uint8_t A;
    uint8_t X;
    uint8_t Y;
};

class cpu {
private:
    reg reg;
public:
    cpu();
};

#endif //COMMON