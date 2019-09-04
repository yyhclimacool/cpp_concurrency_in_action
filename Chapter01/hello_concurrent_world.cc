#include <iostream>
#include <thread>

using namespace std;

void hello() {
  cout << "Hello, Concurrent world!" << endl;
}

int main() {
  std::thread t(hell);
  t.join();
}
