#include <iostream>
#include <thread>
#include <string>

using namespace std;

class background_task {
public:
  void operator()() {
    std::cout << "Calling background_task.operator()" << std::endl;
  }
};

int main() {
  background_task f;
  std::thread my_thread(f);
  my_thread.join();

  std::thread t{background_task()};
  t.join();

  std::thread t2{[](){ cout << "Calling lambda..." << endl;}};
  t2.join();
}
