#include <thread>
#include <iostream>
#include <future>

std::shared_future<int> *global_sfp;

void thread_func(std::shared_future<int> sf) {
  int val = sf.get();
  std::cout << "thread_id:" << std::this_thread::get_id() << " val:" << val << std::endl;
}

int main() {
  std::promise<int> p;
  std::future<int> f = p.get_future();
  std::shared_future<int> sf(std::move(f));
  for (int i = 0; i < 3; ++i) {
    std::thread(thread_func, sf).detach();
  }
  p.set_value(42);
  pthread_exit(NULL);
}
