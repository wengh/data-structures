# prefix max
class BinaryIndexedTree:
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
class BinaryIndexedTree:
    def __init__(self, n):
        self.n = n
        self.arr = [0] * (n+1)
    def get(self, i):
        ans = 0
        i += 1
        while i > 0:
            ans += self.arr[i]
            i -= i & -i
        return ans
    def set(self, i, x):
        i += 1
        while i <= self.n:
            self.arr[i] += x
            i += i & -i
