#pragma once

// https://gcc.gnu.org/onlinedocs/gcc-3.4.3/gcc/Function-Attributes.html
constexpr int getArrayPos(int x, int y) __attribute__((const));

constexpr int getArrayPos(int x, int y) {

    return 8 * x + y;

}