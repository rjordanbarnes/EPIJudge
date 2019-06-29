import concurrent.futures
import sys
import time


# Performs basic unit of work
def worker(lower, upper):
    for i in range(lower, upper + 1):
        assert collatz_check(i, set())
    print('(%d,%d)' % (lower, upper))


# Checks an individual number.
def collatz_check(x, visited):
    if x == 1:
        return True
    elif x in visited:
        return False
    visited.add(x)
    if x & 1:  # Odd number.
        return collatz_check(3 * x + 1, visited)
    # Divide by 2 for even number.
    return collatz_check(x >> 1, visited)


def main():
    n = 10000000
    range_size = 1000000
    num_threads = 4
    if len(sys.argv) > 1:
        n = int(sys.argv[1])
    if len(sys.argv) > 2:
        range_size = int(sys.argv[2])
    if len(sys.argv) > 3:
        num_threads = int(sys.argv[3])

    assert collatz_check(1, set())
    assert collatz_check(3, set())
    assert collatz_check(8, set())
    start_time = time.time()

    # Uses the library process pool for task assignment and load balancing
    with concurrent.futures.ThreadPoolExecutor(
            max_workers=num_threads) as executor:
        for i in range(n // range_size):
            executor.submit(worker, i * range_size + 1, (i + 1) * range_size)

    print('Finished all threads')
    running_time = (time.time() - start_time) * 1000
    print('time in milliseconds for checking to %d is %d (%d per ms)' %
          (n, running_time, n / running_time))


if __name__ == '__main__':
    main()
