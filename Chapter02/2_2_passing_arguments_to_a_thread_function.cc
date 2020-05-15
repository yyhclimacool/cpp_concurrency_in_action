#include <thread>
#include <iostream>
#include <string>

void f(int i, const std::string &s) {
  std::cout << "int:" << i << " string:" << s << std::endl;
}

int main() {
  std::thread t(f, 42, "hello");
  t.join();
}
