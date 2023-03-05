from collections import Counter


def getIsPrime(high):
    mask = [True] * high
    mask[0] = mask[1] = False
    for n in range(2, high):
        if mask[n]:
            for i in range(n * n, high, n):
                mask[i] = False
    return mask

high = 100000
isPrime = getIsPrime(high)
primes = [i for i, x in enumerate(isPrime) if x]


def factorize(n):
    i = 2
    ans = Counter()
    p = 2
    while p * p <= n:
        while n % p == 0:
            n //= p
            ans[p] += 1
        p += 1
    if n > 1:
        ans[n] += 1
    return ans

  
def divisors(factors):
    if len(factors) == 0:
        yield 1
    else:
        p, n = factors[0]
        a = 1
        for i in range(n+1):
            for b in divisors(factors[1:]):
                yield a * b
            a *= p
