class SegmentTree:
    def __init__(self, arr, identity=0):
        self.identity = identity
        self.n = len(arr)
        self.store = [self.identity] * (2 * self.n)
        for i in range(len(arr)):
            self.store[self.n + i] = arr[i]
        for i in range(self.n - 1, 0, -1):
            self.store[i] = self.store[i << 1] + self.store[i << 1 | 1]

    def set(self, i, value):
        i += self.n
        self.store[i] = value
        while i > 1:
            i >>= 1
            self.store[i] = self.store[i << 1] + self.store[i << 1 | 1]

    def sum(self, l, r):  # [l, r)
        left = right = self.identity
        l += self.n
        r += self.n
        while l < r:
            if l & 1:
                left = left + self.store[l]
                l += 1
            if r & 1:
                r -= 1
                right = self.store[r] + right
            l >>= 1
            r >>= 1
        return left + right


class SegmentTree:
    def __init__(self, lo, hi):
        self.lo = lo
        self.hi = hi
        mi = (lo + hi) // 2
        if hi - lo > 1:
            self.left = SegmentTree(lo, mi)
            self.right = SegmentTree(mi, hi)
            self.lazy = False
        else:
            self.lazy = True
        self.x = 0
    
    def push(self):
        if self.lazy:
            self.left.set(self.lo, self.hi, self.x)
            self.right.set(self.lo, self.hi, self.x)
            self.x *= (self.hi - self.lo)
            self.lazy = False
    
    def get(self, lo, hi):
        if hi <= self.lo or self.hi <= lo:
            return 0
        if lo <= self.lo and self.hi <= hi:
            if self.lazy:
                return self.x * (self.hi - self.lo)
            else:
                return self.x
        else:
            self.push()
            result = 0
            result += self.left.get(lo, hi)
            result += self.right.get(lo, hi)
            return result
    
    def set(self, lo, hi, x):
        if hi <= self.lo or self.hi <= lo:
            return 0
        if lo <= self.lo and self.hi <= hi:
            old = self.get(self.lo, self.hi)
            self.x = x
            self.lazy = True
            return self.get(self.lo, self.hi) - old
        else:
            self.push()
            change = 0
            change += self.left.set(lo, hi, x)
            change += self.right.set(lo, hi, x)
            self.x += change
            return change
