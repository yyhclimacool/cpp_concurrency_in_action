#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <algorithm>
#include <list>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value) {
  std::lock_guard<std::mutex> guard(some_mutex);
  some_list.push_back(new_value);
}

bool list_contains(int value_to_look) {
  // C++17 suppoert class template parameter deduction
  //   std::lock_guard guard(some_mutex);
  // you can also use enhanced version of lock guard
  //   std::scoped_lock guard(some_mutex);
  std::lock_guard<std::mutex> guard(some_mutex);
  return std::find(some_list.begin(), some_list.end(), value_to_look) != some_list.end();
}

int main() {
  std::thread t1([] {int i = 100; while (i-- > 0) { add_to_list(i); }});
  std::thread t2([] {auto res = list_contains(55); if(res) std::cout << "found 55" << std::endl; else std::cout << "55 not found" << std::endl; });
  t2.join();
  t1.join();
}

