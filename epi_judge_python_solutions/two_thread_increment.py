import sys
import threading

counter = 0


def increment_thread(n):
    global counter
    for _ in range(n):
        counter = counter + 1


def two_thread_increment_driver(n):
    t1 = threading.Thread(target=increment_thread, args=(n, ))
    t2 = threading.Thread(target=increment_thread, args=(n, ))

    t1.start()
    t2.start()
    t1.join()
    t2.join()

    print(counter)


if __name__ == '__main__':
    n = int(sys.argv[1]) if len(sys.argv) == 2 else 1000000000
    two_thread_increment_driver(n)
