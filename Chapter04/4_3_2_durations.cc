#include <iostream>
#include <future>
#include <chrono>

int some_task() {
  return 42;
}

int main() {
  // many milliseconds in long
  // std::chrono::duration<long, std::ratio<1, 1000>> 10_milliseconds(10);
  auto ten_millisecs = std::chrono::milliseconds(10);

  using namespace std::chrono_literals;
  auto one_day = 24h;
  auto half_an_hour = 30min;
  auto max_time_between_messages = 30ms;

  std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(half_an_hour);

  std::future<int> f = std::async(std::launch::async, some_task);
  if(f.wait_for(std::chrono::milliseconds(10)) == std::future_status::ready)
    std::cout << f.get() << std::endl;
}
