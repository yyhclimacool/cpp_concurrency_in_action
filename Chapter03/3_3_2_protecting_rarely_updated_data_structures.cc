#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <pthread.h>

using dns_entry = std::string;

class dns_cache {
  std::map<std::string, dns_entry> entries;
  mutable std::shared_mutex entry_mutex;
public:
  dns_entry find_entry(const std::string &domain) {
    std::shared_lock<std::shared_mutex> lk(entry_mutex);
    auto it = entries.find(domain);
    return it == entries.end() ? dns_entry(domain + " not_found") : it->second;
  }

  void update_or_add_entry(const std::string &domain, const dns_entry &entry) {
    std::lock_guard<std::shared_mutex> guard(entry_mutex);
    printf("\tadd [%s - %s]\n", domain.c_str(), entry.c_str());
    entries[domain] = entry;
  }
};

dns_cache cache;

void thread_a() {
  for (int i = 0; i < 10; ++i) {
    char buf[64];
    snprintf(buf, sizeof buf, "www.abc%d.com", i);
    std::string domain(buf);
    bzero(buf, sizeof buf);
    snprintf(buf, sizeof buf, "10.0.0.%d", i);
    cache.update_or_add_entry(domain, std::string(buf));
  }
}

void thread_b() {
  for (int i = 10; i < 23; ++i) {
    char buf[64];
    snprintf(buf, sizeof buf, "www.abc%d.com", i);
    std::string domain(buf);
    bzero(buf, sizeof buf);
    snprintf(buf, sizeof buf, "10.0.0.%d", i);
    cache.update_or_add_entry(domain, std::string(buf));
  }
}

int main() {
  std::thread t1(thread_a);
  std::thread t2(thread_b);

  t1.detach();
  t2.detach();

  std::cout << cache.find_entry("www.abc0.com") << std::endl;
  pthread_exit(NULL);
}
