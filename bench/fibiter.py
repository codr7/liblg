from timeit import default_timer as timer

def fib(n):
    a, b = 0, 1
    for _ in range(0, n): a, b = b, a+b
    return a

t = timer()

for i in range(100):
    assert fib(20) == 6765

print('{0}us'.format(round((timer() - t) * 1000000)))
