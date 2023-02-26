class Counter1(Counter):
    def __setitem__(self, value, key):
        if value == 0:
            del self[key]
        else:
            super().__setitem__(value, key)
