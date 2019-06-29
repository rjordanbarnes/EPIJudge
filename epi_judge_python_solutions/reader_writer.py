import random
import threading
import time


def do_something_else():
    time.sleep(random.random())


# LR and LW are class attributes in the ReaderWriter class.
# They serve as read and write locks. The integer
# variable read_count in ReaderWriter tracks the number of readers.
class Reader(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name, daemon=True)

    def run(self):
        while True:
            with ReaderWriter.LR:
                ReaderWriter.read_count += 1

            print('Reader', self.name, 'is about to read')

            print(ReaderWriter.data)
            with ReaderWriter.LR:
                ReaderWriter.read_count -= 1
                ReaderWriter.LR.notify()
            do_something_else()


class Writer(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name, daemon=True)

    def run(self):
        while True:
            with ReaderWriter.LW:
                done = False
                while not done:
                    with ReaderWriter.LR:
                        if ReaderWriter.read_count == 0:

                            print('Writer', self.name, 'is about to write')

                            ReaderWriter.data += 1
                            done = True
                        else:
                            # use wait/notify to avoid busy waiting
                            while ReaderWriter.read_count != 0:
                                ReaderWriter.LR.wait()
            do_something_else()


class ReaderWriter:
    data = 0
    LR = threading.Condition()
    read_count = 0
    LW = threading.Lock()


def main():
    r0 = Reader('r0')
    r1 = Reader('r1')
    w0 = Writer('w0')
    w1 = Writer('w1')
    r0.start()
    r1.start()
    w0.start()
    w1.start()
    time.sleep(10)


if __name__ == '__main__':
    main()
