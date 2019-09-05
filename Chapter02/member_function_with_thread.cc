#include <thread>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

struct X {
  void do_lengthy_work() {
    cout << "do_lengthy_work..." << endl;
  }
};

int main() {
  X x;
  std::thread t(&X::do_lengthy_work, &x);
  t.join();
}
