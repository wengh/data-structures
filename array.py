import collections

class Array(collections.abc.Sequence):
    def __init__(self, arr, index_range=None):
        self.arr = arr
        if not index_range:
            index_range = range(len(arr))
        self.range = index_range
    def __getitem__(self, key):
        if isinstance(key, int):
            return self.arr[self.range[key]]
        return Array(self.arr, self.range[key])
    def __setitem__(self, key, value):
        if isinstance(key, slice):
            assert(len(value) == len(self.range[key]))
        self.arr[self.range[key]] = value
    def __len__(self):
        return len(self.range)
    def __reversed__(self):
        return self[::-1]
    def __str__(self):
        return str(list(self))
    def __repr__(self):
        return f'Array({self.arr}, {self.range})'
