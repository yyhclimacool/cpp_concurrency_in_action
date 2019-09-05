// See: https://stackoverflow.com/questions/57798868/how-stdthread-object-copies-argument
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

void f(int i, const std::string &str) {
  printf("i = %d, str = %s\n", i, str.c_str());
}

void oops(int ival) {
  char *buf = new char[32]{0};
  snprintf(buf, 32, "%i", ival);
  // maybe danling pointer
  // if it's danling pointer, it's undefined behaivor
  // the content of buf is undefined
  std::thread t(f, 3, buf); // FIXME: create a temporary std::string object
  t.detach();

  // 这里会有一个问题：对象t复制了buf指针，执行delete之后buf不再有效
  delete [] buf; 
}

void not_oops(int ival) {
  char *buf = new char[32]{0};
  snprintf(buf, 32, "%i", ival);
  std::thread t(f, 5, std::string(buf));
  t.detach();

  delete [] buf;
}

int main() {
  oops(1992); // This is buggy
  not_oops(1999);
  sleep(5);
}
