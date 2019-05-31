#include "loadbar.h"

//For sleeping
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

int main() {
    const int size = 200;
    ProgressBar bar(size, std::cerr);
    for (int i = 0; i < size; i++) {
        bar.print(i, fmt::format("i={}", i));
        std::this_thread::sleep_for(0.1s);
    }
}
