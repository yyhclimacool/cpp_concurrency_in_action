#include <iostream>
#include <thread>

using namespace std;

void hello() {
  cout << "Hello, concurrency world!" << endl;
}

int main() {
  std::thread t(hello);
  t.join();
}
