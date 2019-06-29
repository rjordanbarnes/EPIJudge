import random


def random_iterator(limit):
    offset = 0
    while True:
        offset += random.random()
        if (offset > limit):
            raise StopIteration()
        yield offset


ri = random_iterator(10.0)
for i in ri:
    print(i)
