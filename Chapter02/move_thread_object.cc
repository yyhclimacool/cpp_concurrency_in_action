#include <thread>
#include <iostream>
#include <string>

using namespace std;

void some_function() {
  printf("some_function\n");
}

std::thread f() {
  return std::thread(some_function);
}

void some_other_function() {
  printf("some_other_function\n");
}

std::thread g() {
  std::thread t(some_other_function);
  return t; // it's movable
}

void f1(std::thread t) {
  cout << std::this_thread::get_id() << endl;
  t.join();
}

void g1() {
  f1(std::thread(some_function));
  std::thread t(some_function);
  f1(std::move(t));
}

int main() {
  f().join();
  g().join();
  g1();
}
