def foo(x=[]):
    x.append(1)
    return x


result = foo()
print(result)
result = foo()
print(result)
