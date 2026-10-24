#include "prx/libc/include/general/VabiMacros.hpp"
#include <array>
#include <cstdlib>
#include <cstring>

extern "C" {
char* APS5_VABI getlogin_nid_postfix();
int APS5_VABI getlogin_r_nid_postfix(char* output, int size);
int* APS5_VABI __error_nid_postfix();
}

static void Require(bool value) { if (!value) std::abort(); }

int main() {
    std::array<char, 33> expected{};
    const int error = getlogin_r_nid_postfix(expected.data(), static_cast<int>(expected.size()));
    *__error_nid_postfix() = 12345;
    char* first = getlogin_nid_postfix();
    if (error != 0) {
        Require(first == nullptr);
        Require(*__error_nid_postfix() == error);
        return 0;
    }
    Require(first != nullptr);
    Require(*__error_nid_postfix() == 12345);
    Require(std::strcmp(first, expected.data()) == 0);
    char* second = getlogin_nid_postfix();
    Require(second == first);
    Require(std::strcmp(second, expected.data()) == 0);
    return 0;
}
