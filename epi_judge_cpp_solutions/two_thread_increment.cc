
#include <iostream>
#include <string>
#include <thread>

using std::cout;
using std::endl;
using std::stoi;
using std::thread;

static int counter = 0;

void IncrementThread(int n) {
  for (int i = 0; i < n; ++i) {
    ++counter;
  }
}

void TwoThreadIncrementDriver(int n) {
  thread T1(IncrementThread, n);
  thread T2(IncrementThread, n);
  T1.join();
  T2.join();

  cout << counter << endl;
}

int main(int argc, char* argv[]) {
  int n = argc == 2 ? stoi(argv[1]) : 1000000000;
  TwoThreadIncrementDriver(n);
  return 0;
}
