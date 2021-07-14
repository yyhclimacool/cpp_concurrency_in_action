/* 
 * brief : relaxed operations have few oerdering requirements
 */
#include <atomic>
#include <thread>
#include <assert.h>
#include <iostream>
#include <vector>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);
    // x sequenced before y, thus x happens before y
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
    while(!y.load(std::memory_order_relaxed));
    // no total order with relaxed, x could be false even though y returns true
    // the visibility of x in this thread is independent with visibility of y
    if (x.load(std::memory_order_relaxed))
        ++z;
}

int test() {
    x = false;
    y = false;
    z = 0;

    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    // z could be 0
    return z.load();
}

int main() {
    std::vector<int> counter(2, -1);
    for (int i = 0; i < 1000; ++i) {
        ++counter[test()];
    }
    std::cout << "0: " << counter[0]+1 << std::endl;
    std::cout << "1: " << counter[1]+1 << std::endl;
}
