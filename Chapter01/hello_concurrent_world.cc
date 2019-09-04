#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void hello() {
  cout << "Hello, Concurrent world!" << endl;
}

int main() {
  std::thread t(hello);
  t.join();
  // 对象t析构前需要设置让该std::thread对象join还是detach
  // 否则t的析构函数会调用terminate，进而coredump
}
