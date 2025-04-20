from sortedcontainers import SortedList
from dataclasses import dataclass
from fractions import Fraction
from bisect import bisect_left


Num = int


@dataclass(frozen=True, order=True, slots=True)
class Line:
    m: Num
    b: Num

    def __call__(self, x):
        return self.m * x + self.b

    def intersect(self, other: 'Line'):
        if self.m == other.m:
            return None
        return Fraction((other.b - self.b), (self.m - other.m))


class LineSet:
    def __init__(self):
        self.lines = SortedList(key=lambda l: -l.m)
        self.xs = SortedList()

    def __remove(self, i):
        del self.lines[i]
        if i < len(self.xs):
            del self.xs[i]
        if i > 0:
            del self.xs[i - 1]
        if 0 < i < len(self.lines):
            self.xs.add(self.lines[i].intersect(self.lines[i - 1]))
    
    def __len__(self):
        return len(self.lines)

    def __call__(self, x):
        '''
        Get the minimum y value at x among all lines.
        '''
        i = self.xs.bisect_left(x)
        return self.lines[i](x)

    def add(self, line: Line):
        i = self.lines.bisect_left(line)

        if i < len(self.lines):
            l1 = self.lines[i]
            if l1.m == line.m:
                if l1.b <= line.b:
                    return
                else:
                    self.__remove(i)

        if 0 < i < len(self.lines):
            l1 = self.lines[i - 1]
            l2 = self.lines[i]
            x = l1.intersect(l2)
            if line(x) >= l1(x):
                return

        while i + 1 < len(self.lines):
            l1 = self.lines[i]
            l2 = self.lines[i + 1]
            x = l1.intersect(l2)
            if line(x) <= l1(x):
                self.__remove(i)
            else:
                break

        while i - 2 >= 0:
            l1 = self.lines[i - 1]
            l2 = self.lines[i - 2]
            x = l1.intersect(l2)
            if line(x) <= l1(x):
                self.__remove(i - 1)
                i -= 1
            else:
                break

        if 0 < i <= len(self.xs):
            del self.xs[i-1]
        if 0 < i:
            self.xs.add(line.intersect(self.lines[i - 1]))
        if i < len(self.lines):
            self.xs.add(line.intersect(self.lines[i]))

        self.lines.add(line)

    def validate(self):
        '''
        Check that invariants hold.
        '''
        n = len(self.lines)
        expect = []
        for i in range(n - 1):
            l1 = self.lines[i]
            l2 = self.lines[i + 1]
            x = l1.intersect(l2)
            expect.append(x)
        assert self.xs == expect, f'\n{list(self.xs)}\nexpect:\n{expect}'


class MonotonicLineSet:
    '''
    A line set that requires the lines to be added in decreasing order of slope.
    '''

    def __init__(self):
        self.lines = []
        self.xs = []

    def __len__(self):
        return len(self.lines)

    def __call__(self, x):
        '''
        Get the minimum y value at x among all lines.
        '''
        i = bisect_left(self.xs, x)
        return self.lines[i](x)

    def add(self, line: Line):
        assert(not self.lines or self.lines[-1].m > line.m)
        while len(self) >= 2:
            l1 = self.lines[-1]
            l2 = self.lines[-2]
            x = l1.intersect(l2)
            if line(x) <= l1(x):
                self.lines.pop()
                self.xs.pop()
            else:
                break
        if len(self) >= 1:
            self.xs.append(line.intersect(self.lines[-1]))
        self.lines.append(line)

    def validate(self):
        '''
        Check that invariants hold.
        '''
        n = len(self.lines)
        expect = []
        for i in range(n - 1):
            l1 = self.lines[i]
            l2 = self.lines[i + 1]
            x = l1.intersect(l2)
            expect.append(x)
        assert self.xs == expect, f'\n{self.xs}\nexpect:\n{expect}'
