#include <iostream>
#include <thread>
#include <string>

using namespace std;

inline void do_somework() {
  cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << endl;
}

class background_task {
public:
  void operator()() const {
    cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << endl;
  }
};

struct func {
private:
  int &ival;
public:
  func(int &i) : ival(i) {}
  void operator()() {
    long long local = 0;
    for (int j = 0; j != 10000; ++j) {
      for (int i = 0; i != ival; ++i) {
        ++local;
      }
    }
    cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << " | " << local << endl;
  }
};

void oops() {
  int some_local_int = 2;
  func myfunc(some_local_int);
  std::thread mythread(myfunc);
  //mythread.detach();
  // waiting for thread to finish
  mythread.join();
}

int main() {
  std::thread t1(do_somework);
  t1.join();

  std::thread t2{background_task()};
  const background_task bgtask;
  std::thread t3{bgtask};
  t2.join();
  t3.join();

  std::thread t4([](){cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << endl;});
  t4.join();

  oops();
}
