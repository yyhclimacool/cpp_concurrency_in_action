#include <thread>
#include <mutex>

bool flag = false;
std::mutex flag_mutex;

void wait_for_flag() {
  std::unique_lock<std::mutex> lk(flag_mutex);
  while (!flag) {
    lk.unlock();
    printf("flag is false...\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    lk.lock();
  }
  printf("now flag is true!\n");
}

void set_flag_to_true() {
  std::this_thread::sleep_for(std::chrono::seconds(3));
  std::lock_guard<std::mutex> guard(flag_mutex);
  flag = true;
}

int main() {
  std::thread t1(set_flag_to_true);
  t1.detach();
  wait_for_flag();
}
