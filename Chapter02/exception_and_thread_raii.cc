#include <thread>
#include <iostream>

using namespace std;

class thread_guard {
  std::thread &t_;
public:
  thread_guard(std::thread &t) : t_(t) {}
  ~thread_guard() {
    if (t_.joinable()) {
      t_.join();
    }
  }
  thread_guard(const thread_guard &) = delete;
  thread_guard &operator=(const thread_guard &) = delete;
};

struct func {
  int &i;
  func(int &i_) : i(i_) {}
  void operator()() {
    for (unsigned j = 0; j < 1000000; ++j)
      ++i;
    std::cout << "i = " << i << endl;
  }
};

void f(bool throw_or_not = false) {
  int some_local_state = 0;
  func my_func(some_local_state);
  std::thread t(my_func);
  thread_guard g(t);
  // do something in current thread
  {
    if (throw_or_not)
      throw std::runtime_error("Throwing exception in f()");
  }
}

int main() {
  f();
  f(true); // FIXME: 这里为什么没有打印出i的值呢？
}
