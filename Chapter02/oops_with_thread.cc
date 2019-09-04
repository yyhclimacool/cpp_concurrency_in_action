#include <iostream>
#include <thread>

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

// This is bad
void oops() {
  int some_local_stat = 0;
  func my_func(some_local_stat);
  std::thread my_thread(my_func);
  my_thread.detach();
}

// This is OK
void oops_2() {
  int some_local_stat = 0;
  func my_func(some_local_stat);
  std::thread my_thread(my_func);
  my_thread.join();
  // my_thread.join(); // join two times is illegal
  if (!my_thread.joinable())
    cout << "oops_2 thread joined and is not joinable now" << endl;
}

int main() {
  oops_2();
  return 0;
}
