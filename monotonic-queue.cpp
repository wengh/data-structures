#include <deque>
#include <functional>

template <typename T = int, typename Comp = std::less<T>>
class MinMonotonicQueue {
private:
    struct Entry {
        T x;
        size_t count = 0;
    };

public:
    bool empty() const {
        return store.empty();
    }

    T min() const {
        return store.front().x;
    }

    void push(T x) {
        while (!store.empty() && lt(x, store.back().x)) {
            store.pop_back();
        }
        if (store.empty() || store.back().x != x) {
            store.push_back({x, 1});
        } else {
            ++store.back().count;
        }
    }

    void pop(T x) {
        if (!store.empty() && x == store.front().x) {
            if (--store.front().count == 0) {
                store.pop_front();
            }
        }
    }

private:
    std::deque<Entry> store;
    static constexpr inline Comp lt{};
};
