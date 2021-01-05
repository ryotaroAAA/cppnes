#ifndef COMMON_H
#define COMMON_H

#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdlib> 
#include <cstdio>
#include <map> 
#include <string.h>
using namespace std;

#define dprint(msg, ...) printf("[DEBUG] " msg " %s(%d) @ %s\n", ##__VA_ARGS__  ,__func__, __LINE__, __FILE__)
#define dump(msg, ...) printf("[DUMP] " msg "\n", ##__VA_ARGS__)

#endif //COMMON_H