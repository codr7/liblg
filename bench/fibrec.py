from timeit import default_timer as timer

def fib(n):
    if n < 2:
        return n

    return fib(n-1) + fib(n-2)


t = timer()

for i in range(100):
    assert fib(20) == 6765

print('{0}us'.format(round((timer() - t) * 1000000)))
