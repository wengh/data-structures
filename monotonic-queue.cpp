#include <deque>
#include <functional>

template <typename T = int, typename Comp = std::less<T>>
class MinMonotonicQueue {
public:
    bool empty() const {
        return store.empty();
    }

    T min() const {
        return store.front();
    }

    void push(T x) {
        while (!store.empty() && lt(x, store.back())) {
            store.pop_back();
        }
        store.push_back(x);
    }

    void pop(T x) {
        if (!store.empty() && x == store.front()) {
            store.pop_front();
        }
    }

private:
    std::deque<T> store;
    static constexpr inline Comp lt{};
};
