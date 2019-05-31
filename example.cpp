#include "loadbar.h"

//For sleeping
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

int main() {
    const int size = 100;
    ProgressBar bar(size, std::cerr);
#pragma omp parallel for
    for (int i = 0; i < size; i++) {
        bar.print();

        std::this_thread::sleep_for(0.3s);
    }
}
