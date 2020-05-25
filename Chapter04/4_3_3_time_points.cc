#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <condition_variable>
#include <mutex>

void do_something() {
  sleep(2);
}

bool done;
std::condition_variable cv;
std::mutex mut;

void make_it_done() {
  sleep(3);
  {
  std::lock_guard<std::mutex> guard(mut);
  done = true;
  }
  cv.notify_one();
}

bool wait_for_done() {
  std::unique_lock<std::mutex> lk(mut);
  // wait for 2 seconds.
  auto end_time_point = std::chrono::steady_clock::now() + std::chrono::milliseconds(2000);
  // always using a while loop for spurious wakeups when you're not specifing a predicate.
  while (!done) {
    // use wait_until for bounded length of loop, not wait_for.
    if(cv.wait_until(lk, end_time_point) == std::cv_status::timeout) {
      std::cout << "wait for done timed out!" << std::endl;
      break;
    }
  }
  return done;
}

int main() {
  //std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes> now = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now(); // std::chrono::time_point<>
  auto cnt = now.time_since_epoch(); // returns a std::chrono::duration<>
  std::cout << "time since epoch : " << cnt.count() << " nano seconds." << std::endl;

  auto start = std::chrono::high_resolution_clock::now();
  do_something();
  auto stop = std::chrono::high_resolution_clock::now();
  std::cout << "do_something took "
    << std::chrono::duration<double, std::ratio<1, 1>>(stop - start).count() 
    << " seconds" << std::endl;
  std::cout << "do_something took "
    << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() 
    << " seconds" << std::endl;

  std::thread t1(make_it_done);
  t1.detach();
  
  if (wait_for_done()) {
    std::cout << "DONE!" << std::endl;
  }
}
