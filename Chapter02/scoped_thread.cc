#include <thread>
#include <iostream>
#include <string>

using namespace std;

class scoped_thread {
  std::thread t_;
public:
  explicit scoped_thread(std::thread t) : t_(std::move(t)) {
    if (!t_.joinable())
      throw std::logic_error("No thread");
  }

  ~scoped_thread() {
    t_.join();
  }
  scoped_thread(const scoped_thread &) = delete;
  scoped_thread &operator=(const scoped_thread &) = delete;
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

void f() {
  int some_local_state = 0;
  scoped_thread(std::thread(func(some_local_state))); // the thread will be joined
}

int main() {
  f();
}
