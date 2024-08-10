#include <algorithm>
#include <cassert>
#include <vector>


using Num = long long;

struct Line {
  Num m, b;

  Num operator()(Num x) const { return m * x + b; }

  double intersect(const Line &other) const {
    return static_cast<double>(other.b - b) / (m - other.m);
  }

  bool operator<(const Line &other) const { return m < other.m; }
};

class MonotonicLineSet {
public:
  MonotonicLineSet() {}

  Num operator()(Num x) const {
    auto it = std::lower_bound(xs.begin(), xs.end(), x);
    return lines[it - xs.begin()](x);
  }

  void add(const Line &line) {
    assert(lines.empty() || lines.back().m > line.m);
    while (lines.size() >= 2) {
      const Line &l1 = lines.back();
      const Line &l2 = lines[lines.size() - 2];
      double x = l1.intersect(l2);
      if (line(x) <= l1(x)) {
        lines.pop_back();
        xs.pop_back();
      } else {
        break;
      }
    }
    if (!lines.empty()) {
      xs.push_back(line.intersect(lines.back()));
    }
    lines.push_back(line);
  }

  void validate() const {
    int n = lines.size();
    std::vector<double> expect;
    for (int i = 0; i < n - 1; ++i) {
      double x = lines[i].intersect(lines[i + 1]);
      expect.push_back(x);
    }
    assert(xs == expect);
  }

private:
  std::vector<Line> lines;
  std::vector<double> xs;
};
