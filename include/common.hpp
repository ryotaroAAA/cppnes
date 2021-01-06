#ifndef COMMON_H
#define COMMON_H

#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdlib> 
#include <cstdio>
#include <map> 
#include <string.h>
// #include <stdexcept>
using namespace std;

#define dprint(msg, ...) printf("[DEBUG] " msg " %s(%d) @ %s\n", ##__VA_ARGS__  ,__func__, __LINE__, __FILE__)
#define dump(msg, ...) printf("[DUMP] " msg "\n", ##__VA_ARGS__)

typedef enum {
    BYTE, WORD, NONE=0
} SIZE;

typedef union {
    uint8_t b_data;
    uint16_t w_data;
} DATA;

#endif //COMMON_H