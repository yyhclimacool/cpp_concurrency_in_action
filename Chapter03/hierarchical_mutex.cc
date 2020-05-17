#include <iostream>
#include <limits.h>
#include <mutex>
#include <thread>

class hierarchical_mutex {
    std::mutex internal_mutex_;
    const unsigned long hierarchy_value_;
    unsigned long previous_hierarchy_value_;
    static thread_local unsigned long this_thread_hierarchy_value_;

    // 欲尝试枷锁的mutex的层次值必须要小于当前线程的锁层次值
    void check_for_hierarchy_violation() {
        if (this_thread_hierarchy_value_ <= hierarchy_value_) {
          std::cout << "violdated this_thread_hierarchy_value_ <= hierarchy_value_" << std::endl;
          throw std::logic_error("mutex hierarchy violated");
        } else {
          std::cout << __func__ << "() passed with this_thread_hierarchy_value_ = " << this_thread_hierarchy_value_ << " hierarchy_value_ = " << hierarchy_value_ << std::endl;
        }
    }

    // 更新层次值
    void update_hierarchy_value() {
        previous_hierarchy_value_ = this_thread_hierarchy_value_;
        this_thread_hierarchy_value_ = hierarchy_value_;
    }
public:
    hierarchical_mutex(unsigned long h_val) 
        : hierarchy_value_(h_val),
          previous_hierarchy_value_(0) { }
    
    void lock() {
        check_for_hierarchy_violation();
        internal_mutex_.lock();
        update_hierarchy_value();
    }

    bool try_lock() {
        check_for_hierarchy_violation();
        if (!internal_mutex_.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }

    void unlock() {
        if (this_thread_hierarchy_value_ != hierarchy_value_) {
          std::cout << "violated this_thread_hierarchy_value_ != hierarchy_value_" << std::endl;
          throw std::logic_error("unlock mutex at the reverse order of lock");
        }
        this_thread_hierarchy_value_ = previous_hierarchy_value_;
        internal_mutex_.unlock();
    }
};

// 通过线程本地存储来保存当前锁住的mutex的层次值
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value_(ULONG_MAX);

hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
hierarchical_mutex other_level_mutex(6000);

int do_low_level_stuff() {
    // don't lock mutex
    std::cout << __func__ << std::endl;
    return 2;
}

int low_level_func() {
    std::lock_guard guard(low_level_mutex);
    return do_low_level_stuff();
}

void do_high_level_stuff(int val) {
    std::cout << __func__ << " int_val = " << val << std::endl;
}

void high_level_func() {
    // locks high_level first
    std::lock_guard guard(high_level_mutex);
    // locks low_level then
    do_high_level_stuff(low_level_func());
}

// OK
void thread_a() {
    high_level_func();
}

void do_other_stuff() {
    std::cout << __func__ << std::endl;
}

void other_level_func() {
    high_level_func();
    do_other_stuff();
}

// NOT OK!
void thread_b() {
    std::lock_guard guard(other_level_mutex);
    other_level_func();
}

/*************************************************************************
 * 以下代码测试正常情况下的死锁的发生
 ************************************************************************/

std::mutex high_mutex;
std::mutex low_mutex;
std::mutex other_mutex;

int low_lock_func() {
  std::cout << " --> " << __func__ << std::endl;
  std::lock_guard guard(low_mutex);
  return 2;
}

void high_lock_func() {
  std::cout << " --> " << __func__;
  std::lock_guard guard(high_mutex);
  low_lock_func();
}

void other_lock_func() {
  std::cout << " --> " << __func__;
  high_lock_func();
}

// 锁住high_mutex，然后再锁住low_mutex，这没有问题
void thread_1() {
  std::cout << "--> " << __func__ ;
  high_lock_func();
  std::lock_guard guard(other_mutex);
}

// 先锁住other_mutex，然后锁住high_mutex，然后锁住low_mutex
void thread_2() {
  std::cout << "--> " << __func__ ;
  std::lock_guard guard(other_mutex);
  other_lock_func();
}

int main() {
  // pass
  thread_a();
  // exception
  // thread_b();
  std::thread t1(thread_1);
  std::thread t2(thread_2);
  t1.join();
  t2.join();
}
