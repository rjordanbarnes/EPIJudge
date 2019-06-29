
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>

using std::condition_variable;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::lock_guard;
using std::mutex;
using std::random_device;
using std::string;
using std::thread;
using std::uniform_int_distribution;
using std::unique_lock;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

namespace ReaderWriter {
int data = 0;
mutex LRm;
condition_variable LR;
int read_count = 0;
mutex LW;
}  // namespace ReaderWriter

void DoSomethingElse() {
  static default_random_engine rnd((random_device())());
  uniform_int_distribution<> wait_time(0, 1000);
  sleep_for(milliseconds(wait_time(rnd)));
}

// LR and LW are variables in the ReaderWriter namespace.
// They serve as read and write locks. The integer
// variable read_count in ReaderWriter tracks the number of readers.
void Reader(string name) {
  while (true) {
    {
      lock_guard<mutex> lock(ReaderWriter::LRm);
      ++ReaderWriter::read_count;
    }

    cout << "Reader " << name << " is about to read" << endl;

    cout << ReaderWriter::data << endl;
    {
      lock_guard<mutex> lock(ReaderWriter::LRm);
      --ReaderWriter::read_count;
      ReaderWriter::LR.notify_one();
    }
    DoSomethingElse();
  }
}

void Writer(string name) {
  while (true) {
    {
      lock_guard<mutex> lock_w(ReaderWriter::LW);
      bool done = false;
      while (!done) {
        unique_lock<mutex> lock(ReaderWriter::LRm);
        if (ReaderWriter::read_count == 0) {
          cout << "Writer " << name << " is about to write" << endl;

          ++ReaderWriter::data;
          done = true;
        } else {
          // use wait/notify to avoid busy waiting
          while (ReaderWriter::read_count != 0) {
            ReaderWriter::LR.wait(lock);
          }
        }
      }
    }
    DoSomethingElse();
  }
}

int main(int argc, char* argv[]) {
  thread r0(Reader, "r0");
  thread r1(Reader, "r1");
  thread w0(Writer, "w0");
  thread w1(Writer, "w1");
  sleep_for(milliseconds(10000));
  return 0;
}
