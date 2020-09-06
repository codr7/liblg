from timeit import default_timer as timer

def fib(n, a=0, b=1):
    if n == 0: return a
    if n == 1: return b

    return fib(n-1, b, a+b)

t = timer()

for i in range(100):
    assert fib(20) == 6765

print('{0}us'.format(round((timer() - t) * 1000000)))
