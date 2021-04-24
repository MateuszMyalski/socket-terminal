#include <stdio.h>
#include <stdlib.h>

#include "src/utils/logger.hpp"

int main(int argc, char* argv[]) {
    Logger::log("Hello", "Hello world", Logger::BLACK);
    return 1;
}