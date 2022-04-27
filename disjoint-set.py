class DisjointSet:
    def __init__(self, N):
        self.arr = [-1] * N
    def get(self, i):
        if self.arr[i] < 0:
            return i
        else:
            self.arr[i] = self.get(self.arr[i])
            return self.arr[i]
    def size(self, i):
        return -self.arr[self.get(i)]
    def union(self, i, j):
        i = self.get(i)
        j = self.get(j)
        if i != j:
            if self.size(i) < self.size(j):
                i, j = j, i
            self.arr[i] -= self.size(j)
            self.arr[j] = i
