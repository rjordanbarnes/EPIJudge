

#include <boost/thread.hpp>
#include <boost/thread/sync_bounded_queue.hpp>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>

using boost::queue_op_status;
using boost::sync_bounded_queue;
using boost::thread_group;
using std::bind;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::ref;
using std::stoi;
using std::unordered_set;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

typedef unsigned long long CollatzInt;
typedef sync_bounded_queue<pair<CollatzInt, CollatzInt>> QueueType;
void Worker(CollatzInt lower, CollatzInt upper);
bool CollatzCheck(CollatzInt x, unordered_set<CollatzInt>& visited);

void ThreadFunc(QueueType& q) {
  pair<CollatzInt, CollatzInt> args;
  while (q.wait_pull_front(args) == queue_op_status::success) {
    Worker(args.first, args.second);
  }
}

// Performs basic unit of work
void Worker(CollatzInt lower, CollatzInt upper) {
  for (CollatzInt i = lower; i <= upper; ++i) {
    assert(CollatzCheck(i, unordered_set<CollatzInt>()));
  }
  cout << '(' << lower << ',' << upper << ')' << endl;
}

// Checks an individual number
bool CollatzCheck(CollatzInt x, unordered_set<CollatzInt>& visited) {
  if (x == 1) {
    return true;
  }
  if (!visited.emplace(x).second) {
    return false;
  }
  if (x & 1) {  // Odd number.
    return CollatzCheck(3 * x + 1, visited);
  }
  // Divide by 2 for even number.
  return CollatzCheck(x >> 1, visited);
}

int main(int argc, char* argv[]) {
  CollatzInt N = 10000000;
  CollatzInt RANGE_SIZE = 1000000;
  int NUM_THREADS = 4;

  if (argc > 1) {
    N = stoi(argv[1]);
  }
  if (argc > 2) {
    RANGE_SIZE = stoi(argv[2]);
  }
  if (argc > 3) {
    NUM_THREADS = stoi(argv[3]);
  }

  assert(CollatzCheck(1, unordered_set<CollatzInt>()));
  assert(CollatzCheck(3, unordered_set<CollatzInt>()));
  assert(CollatzCheck(8, unordered_set<CollatzInt>()));
  auto start_time = system_clock::now();

  // Uses synchronized bounded queue for task assignment and load balancing
  QueueType q(NUM_THREADS);
  thread_group threads;
  for (int i = 0; i < NUM_THREADS; ++i) {
    threads.create_thread(bind(ThreadFunc, ref(q)));
  }
  for (CollatzInt i = 0; i < N / RANGE_SIZE; ++i) {
    q << make_pair(i * RANGE_SIZE + 1, (i + 1) * RANGE_SIZE);
  }
  q.close();
  threads.join_all();

  cout << "Finished all threads" << endl;
  auto running_time =
      duration_cast<milliseconds>(system_clock::now() - start_time).count();
  cout << "time in milliseconds for checking to " << N << " is " << running_time
       << '(' << N / running_time << " per ms)";

  return 0;
}
