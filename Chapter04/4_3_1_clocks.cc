#include <iostream>
#include <chrono>

int main() {
  std::chrono::system_clock::time_point system_clock_now = std::chrono::system_clock::now();
  //std::cout << "std::chrono:system_clock::now() = " << now << std::endl;
  //std::cout << "period of std::chrono::system_clock = " << std::chrono::system_clock::period << std::endl;
  std::cout << "is std::chrono::system_clock steady ? " << std::chrono::system_clock::is_steady << std::endl;

  std::chrono::steady_clock::time_point steady_clock_now = std::chrono::steady_clock::now();
  //std::cout << "period of std::chrono::steady_clock = " << std::chrono::steady_clock::period << std::endl;
  std::cout << "is std::chrono::steady_clock steady ? " << std::chrono::steady_clock::is_steady << std::endl;

  std::chrono::high_resolution_clock::time_point high_rs_clock_now = std::chrono::high_resolution_clock::now();
  // std::cout << "period of std::chrono::high_resolution_clock = " << std::chrono::high_resolution_clock::period << std::endl;
  std::cout << "is std::chrono::high_resolution_clock steady ? " << std::chrono::high_resolution_clock::is_steady << std::endl;
}
