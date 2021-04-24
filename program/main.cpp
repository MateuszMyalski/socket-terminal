#include <iostream>
#include <sstream>

#include "src/utils/logger.hpp"

int main(int argc, char* argv[]) {
    Utils::log("ABCD CHUJ CHUJ CHUuj", Utils::colors::blue);

    std::stringstream ss("asdsad");
    Utils::log(ss, Utils::colors::green);
    return 1;
}