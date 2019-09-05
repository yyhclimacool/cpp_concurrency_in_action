#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

void do_work(unsigned i) {
  sleep(1);
  printf("i = %d\n", i);
}

void f() {
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 20; ++i) {
    threads.push_back(std::thread(do_work, i));
  }

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main() {
  f();
}
