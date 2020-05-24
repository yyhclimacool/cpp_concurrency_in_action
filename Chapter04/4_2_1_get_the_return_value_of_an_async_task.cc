#include <future>
#include <iostream>
#include <iostream>

#include <unistd.h> // sleep()

int find_the_answer_to_ltuae() {
  // thinking
  sleep(3);
  return 42;
}

void do_other_stuff() {
  std::cout << __func__ << std::endl;
}

struct X {
  void foo(int i, const std::string &val) {
    std::cout << __func__ << " int:" << i << " str:" << val << std::endl;
  }
  std::string bar(const std::string &val) {
    std::cout << __func__ << ":" <<  val << std::endl;
    return __func__ + val;
  }
};

struct Y {
  double operator()(double d) {
    std::cout << __func__ << " double:" << d << std::endl;
    return d + 42.424242;
  }
};

int main() {
  std::future<int> fut = std::async(find_the_answer_to_ltuae);
  do_other_stuff();
  std::cout << "The answer is " << fut.get() << std::endl;

  /*****************************************/
  X x;
  auto f1 = std::async(&X::foo, &x, 42, "hello");
  auto f2 = std::async(&X::bar, x, "goodbye");
  f1.wait();
  std::cout << "x.bar(\"goodbye\") = " << f2.get() << std::endl;

  /*****************************************/
  Y y;
  auto f3 = std::async(Y(), 3.14);
  auto f4 = std::async(std::ref(y), 2.718);
  std::cout << "Y().operator(3.14) = " << f3.get() << std::endl;
  std::cout << "std::ref(y).operator(2.718) = " << f4.get() << std::endl;
}
