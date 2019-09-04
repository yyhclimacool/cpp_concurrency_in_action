#include <thread>
#include <iostream>

using namespace std;

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
  try {
    if (throw_or_not) {
      throw std::runtime_error("Throw exception while thread is running");
    }
  } catch (std::exception &e){
    t.join();
    cout << e.what() << endl;
    return; // 这里如果不返回会导致调用两次join，从而coredump
  }
  t.join();
}

int main() {
  f();
  f(true);
}
