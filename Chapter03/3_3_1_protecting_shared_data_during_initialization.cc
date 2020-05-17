#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

std::shared_ptr<std::string> resource_ptr;
std::once_flag resource_flag;

void init_resource() {
  printf("thread_id:%lu calling %s\n", std::this_thread::get_id(), __func__);
  resource_ptr.reset(new std::string("fine"));
}

void thread_func() {
  printf("thread_id:%lu calling %s\n", std::this_thread::get_id(), __func__);
  std::call_once(resource_flag, init_resource);
}

int main() {
  for (int i = 0; i < 5; ++i) {
    std::thread(thread_func).detach();
  }
  pthread_exit(NULL);
}
