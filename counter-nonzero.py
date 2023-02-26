class Counter1(Counter):
    def __setitem__(self, key, value):
        if value == 0:
            del self[key]
        else:
            super().__setitem__(key, value)
