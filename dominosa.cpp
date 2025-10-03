
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

std::string uint2bitstr(uint32_t u, int num_bits) {
  std::string str = "";  
  for (int i = 0; i < num_bits; i++) {
    str = (u & (1U << i)?"1":"0") + str;
  }
  return str;
}

std::vector<int> find_valid_combinations(int bit_length) {
 
  std::vector<int> valid_combinations;

  for (uint32_t u = 0; u < (1U << bit_length); u++) {
    bool result = true;
    int mask_len = 0;
    for (int i = 0; i < bit_length; i++) {
      if (u & (1U << i)) {
        if (mask_len % 2 != 0) {
          result = false;
          break;
        } else {
          mask_len = 0;
        }
      } else {
        mask_len++;
      }
    }
    if (mask_len % 2 == 0 && result == true) {
      valid_combinations.push_back(u);
    }
  }

  return valid_combinations;
}

class Segment {
  int offset;
  int bit_length;
  int curr;
  int inc() {
    cur++;
    if (LUT[bit_length][curr] == END_OF_VALIDS) {
      curr = 0;
      return 1;
    } else {
      return 0;
    }
  }
}

template <int N>
class RowState {
public:
  int mask;
  int curr;
  constexpr N_SEGS = (N + 1) / 2;
  Segement segs[N_SEGS];
  int inc() {
    int carry = 1;
    for (int s = 0; s < N_SEGS; s++) {
      if (carry == 1) {
        carry = segs[s].inc();
      } else {
        break;
      }
    }
    curr = mask;
    for (int s = 0; s < N_SEGS; s++) {
      curr |= (segs[s].curr << segs[s].offset)
    }
  }
}

int main() {
  const int M = 7; // rows
  const int N = 1; // cols
  int puzzle[M][N];

  for (int N = 8; N > 0; N--) {
    auto valid_combinations = find_valid_combinations(N);
    printf("Total combinations: %u
", 1U << N);
    printf("Valid combinations = %ld
", valid_combinations.size());
    for ( auto combination : valid_combinations) {
      std::string str = uint2bitstr(combination, N);
      printf("%u - %s
", combination, str.c_str());
    }
  }
  return 0;
}
