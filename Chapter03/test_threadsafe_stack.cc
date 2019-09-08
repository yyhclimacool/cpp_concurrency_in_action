#include "threadsafe_stack.hpp"
#include <vector>
#include <memory>
#include <unistd.h>

int main() {
  threadsafe_stack<int> sss;
  std::vector<std::thread> threads;
  for (int i = 0; i < 100; ++i) {
    threads.push_back(std::thread([&](){sss.push(i * 7);}));
  }
  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::detach)); 
  std::vector<std::thread> dones;
  for (int i = 0; i < 10; ++i) {
    dones.push_back(std::thread([&](){auto it = sss.pop(); printf("value = %d\n", *(it.get()));}));
  }
  std::for_each(dones.begin(), dones.end(), std::mem_fn(&std::thread::join));
  sleep(10); // waiting for detached threads to finish
}
