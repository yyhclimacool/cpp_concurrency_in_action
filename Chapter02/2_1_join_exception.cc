#include <thread>
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;

struct func {
private:
  int &ival;
public:
  func(int &i) : ival(i) {}
  void operator()() {
    long long local = 0;
    for (int j = 0; j != 10000000; ++j) {
      for (int i = 0; i != ival; ++i) {
        ++local;
      }
    }
    cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << " | " << local << endl;
  }
};

void do_something_in_current_thread() {
  throw std::runtime_error("test exception");
}

void f() {
  int some_local_state = 0;
  func myfunc(some_local_state);
  std::thread t(myfunc);
  try {
    do_something_in_current_thread();
  } catch (std::exception &e) {
    cout << "exception catched... join the thread " << e.what() << endl;
    t.join();
    return;
  }
  cout << "normaly joined the thread" << endl;
  t.join();
}

class thread_guard {
public:
  explicit thread_guard(std::thread &t_) : t(t_) {}
  ~thread_guard() {
    cout << "calling ~thread_guard() ..." << endl;
    if (t.joinable())
      t.join();
  }
  thread_guard(const thread_guard &) = delete;
  thread_guard &operator=(const thread_guard &) = delete;
private:
  std::thread &t;
};

void f2() {
  int some_local_state = 0;
  func myfunc(some_local_state);
  std::thread t{myfunc};
  thread_guard tg{t};
  do_something_in_current_thread();
}

int main() {
  f();
  try {
  f2();
  } catch (...) {
    cout << "catched an exception ..." << endl;
  }
}

