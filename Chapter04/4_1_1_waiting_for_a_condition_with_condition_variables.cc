#include <thread>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <string>
#include <queue>

using namespace std;

std::mutex mut;
std::queue<std::string> data_queue;
std::condition_variable data_cond;

bool stop = false;

void data_preparation_thread() {
  for (int i = 0; i < 100; ++i) {
    char buf[32]{'\0'};
    snprintf(buf, sizeof buf, "data_%3d", i);
    {
      std::lock_guard<std::mutex> guard(mut);
      data_queue.push(std::string(buf));
    }
    data_cond.notify_one();
  }
}

void data_processing_thread() {
  while(true) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [] {return !data_queue.empty(); });
    std::string data = data_queue.front();
    data_queue.pop();
    lk.unlock();
    printf("\tdata from queue: %s\n", data.c_str());
    if (stop)
      break;
  }
}

int main() {
  std::thread(data_preparation_thread).detach();
  std::thread(data_processing_thread).detach();
  pthread_exit(NULL);
}
