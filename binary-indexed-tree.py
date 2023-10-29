# prefix max
class MaxBIT:
    def __init__(self, n):
        self.n = n
        self.arr = [0] * (n+1)
    def get(self, i):
        ans = 0
        i += 1
        while i > 0:
            ans = max(ans, self.arr[i])
            i -= i & -i
        return ans
    def set(self, i, x):
        i += 1
        while i <= self.n:
            self.arr[i] = max(self.arr[i], x)
            i += i & -i
    def unset(self, i):
        i += 1
        while i <= self.n:
            self.arr[i] = 0
            i += i & -i

# prefix sum
class BIT:
    def __init__(self, n):
        self.n = n
        self.arr = [0] * (n + 1)
    def sum(self, i):
        ans = 0
        i += 1
        while i > 0:
            ans += self.arr[i]
            i -= i & -i
        return ans
    def edit(self, i, x):
        i += 1
        while i <= self.n:
            self.arr[i] += x
            i += i & -i
    def __iter__(self):
        for i in range(self.n):
            yield self.sum(i) - self.sum(i-1)
    def __repr__(self):
        return repr(list(self))

class RangeBIT:
    def __init__(self, n):
        self.n = n
        self.bit1 = BIT(n)
        self.bit2 = BIT(n)
    def edit(self, l, r, x):
        self.bit1.edit(l, x)
        self.bit1.edit(r+1, -x)
        self.bit2.edit(l, x*(l-1))
        self.bit2.edit(r+1, -x*r)
    def sum(self, r):
        return self.bit1.sum(r) * r - self.bit2.sum(r)
    def range(self, l, r):
        return self.sum(r) - self.sum(l-1)
    def __iter__(self):
        for i in range(self.n):
            yield self.range(i, i)
    def __repr__(self):
        return repr(list(self))

