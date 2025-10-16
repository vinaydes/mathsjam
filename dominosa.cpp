
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

bool is_valid_layout(int layout, int N) {
 
  // layout is valid, if number of consecutive zeros (zero-segments)
  // int it are all of even length. For example, layout 10011100 is 
  // valid because both the 0-segments are of length 2 (even number).
  // whereas 1000100 is invalid because the first 0-segment has 3 zeros. 

  bool result = true;
  int zero_seg_len = 0; // length of current zero segment

  // iterate over the bits of code
  for (int b = 0; b < N; b++) { 

    if (layout & (1U << b)) {
      // the bit is 1 
      if (zero_seg_len % 2 != 0) {
        result = false;
        break;
      } else {
        zero_seg_len = 0;
      }
    } else {
      // the bit is zer
      zero_seg_len++;
    }
  }

  // Is last zero segment also even lengthed?
  if (zero_seg_len % 2 == 0 && result == true) {
    return true; 
  } else {
    return false;
  }
}

void compute_valid_code_pairs(std::vector<std::vector<int>>& pairs_lut, int N) {
  // A code is a binary code for row where 1 stands for
  // every tile that starts in that row and 0 stands
  // for horizontal tile. We ignore the tiles that end
  // on that rows as it is already encoded by the
  // previous row.
  // To check if the layout of current row is valid
  // we need to combine code for current row with
  // previous one row. This function populates 1 in
  // pairs_lut[i*N + j], if i and j result in a valid
  // layout
  // There are two conditions for a valid pair
  // 1. They should not have any overlapping 1s
  // 2. Current row layout should have all zero-segments
  //    of even length

  pairs_lut.resize(1 << N);
  for (int i = 0; i < (1 << N); i++) {
    pairs_lut[i].clear();
    for (int j = 0; j < (1 << N); j++) {
      int layout = i | j;
      if ( ((i & j) == 0) && is_valid_layout(layout, N) ) {
        pairs_lut[i].push_back(j);
      } 
    }
  }
}

  
const int INVALID = 1000000;

class RowState { 
  public:
  int code;
  // option pointer always points to the next available options
  int o_ptr;

  void set(int s_code) {
    code = s_code;
    o_ptr = 0;
  }

  void invalidate() {
    code = INVALID;
    o_ptr = INVALID;
  }

  int get_next_code(std::vector<std::vector<int>>& ref_pairs_lut) {
    if (o_ptr == ref_pairs_lut[code].size()) {
      return INVALID;
    } else {
      int ret = ref_pairs_lut[code][o_ptr];
      o_ptr++;
      return ret;
    }
  }

  void print(int d, std::vector<std::vector<int>>& ref_pairs_lut) {
    if (code == INVALID) return;
    printf("(%d) %d - ", d, code);
    for (int i = 0; i < ref_pairs_lut.size(); i++) {
      if (i == o_ptr) {
        printf("| %d |,", ref_pairs_lut[code][i]);
      } else {
        printf(" %d, ", ref_pairs_lut[code][i]);
      }
    }
    for (int r = 0; r < ref_pairs_lut.size(); r++) {
      printf("%d - ", r);
      for (int c = 0; c < ref_pairs_lut[r].size(); c++) {
        printf("%d, ", ref_pairs_lut[r][c]);
      }
      printf("\n");
    }
    return;
  }
};

void print_rows(RowState* rows, int len) {
  for (int i = 1; i < len; i++) {
    printf("%d, ", rows[i].code);
  }
  printf("\n");
}


void write_svg(RowState* rows, int M, int N, std::string filename) {
std::string prefix_t = R"(<?xml version="1.0" encoding="UTF-8"?>
<svg width="1000" height="1000" xmlns="http://www.w3.org/2000/svg">
  <rect width="100%" height="100%" fill="#f0f0f0"/>
  <rect x="10" y="10" width="800" height="700" fill="none" stroke="gray" stroke-width="1" />
  <line x1="10" y1="110" x2="810" y2="110" stroke="gray" stroke-width="1" /> 
  <line x1="10" y1="210" x2="810" y2="210" stroke="gray" stroke-width="1" /> 
  <line x1="10" y1="310" x2="810" y2="310" stroke="gray" stroke-width="1" /> 
  <line x1="10" y1="410" x2="810" y2="410" stroke="gray" stroke-width="1" /> 
  <line x1="10" y1="510" x2="810" y2="510" stroke="gray" stroke-width="1" /> 
  <line x1="10" y1="610" x2="810" y2="610" stroke="gray" stroke-width="1" /> 

  <line x1="110" y1="10" x2="110" y2="710" stroke="gray" stroke-width="1" /> 
  <line x1="210" y1="10" x2="210" y2="710" stroke="gray" stroke-width="1" /> 
  <line x1="310" y1="10" x2="310" y2="710" stroke="gray" stroke-width="1" /> 
  <line x1="410" y1="10" x2="410" y2="710" stroke="gray" stroke-width="1" /> 
  <line x1="510" y1="10" x2="510" y2="710" stroke="gray" stroke-width="1" /> 
  <line x1="610" y1="10" x2="610" y2="710" stroke="gray" stroke-width="1" /> 
  <line x1="710" y1="10" x2="710" y2="710" stroke="gray" stroke-width="1" /> 
)";

  std::string tiles;
  for (int i = 1; i < M; i++) {
    for (int j = 0; j < N; j++) {
      std::string tile;
      if ((rows[i-0].code & (1 << j)) == 0) { 
         if( (rows[i-1].code & (1 << j)) == 0 ) {
          // draw horizontal tile and skip one j
          tile = "  <rect x=\"" + std::to_string(20+j*100) + "\" y=\"" + std::to_string(20+(i-1)*100) + 
            "\" width=\"180\" height=\"80\" fill=\"greenyellow\" stroke=\"black\" stroke-width=\"2\" rx=\"10\" />\n";
          j++;
         }
      } else {
        // draw a verticle tile
        tile = "  <rect x=\"" + std::to_string(20+j*100) + "\" y=\"" + std::to_string(20+(i-1)*100) + 
          "\" width=\"80\" height=\"180\" fill=\"skyblue\" stroke=\"black\" stroke-width=\"2\" rx=\"10\" />\n";
      }
      tiles += tile;
    }
  }
  /*std::string path = "<text x=\"10\" y=\"750\" fill=\"blue\" font-size=\"20\">";

  for (int i = 1; i < M; i++) {
    path = (std::string(rows[i].code) + ", ");
  }
  
  path = "</text>";*/

  std::string complete_svg = prefix_t + tiles + std::string("</svg>");
  //printf("%s\n", complete_svg.c_str());
  FILE* fp;

  fp = fopen(filename.c_str(), "w");
  fprintf(fp, "%s\n", complete_svg.c_str());
  fclose(fp);
  return;
}

int main() {
  const int M = 7; // Rows
  const int N = 8; // Cols

  std::vector<std::vector<int>> pairs_lut;
  compute_valid_code_pairs(pairs_lut, N);




  RowState rows[M+1];
  int depth = 0;

  for (int i = 0; i < M; i++) {
    rows[i].invalidate();
  }

  rows[depth].set(0);

  int visited[256];

  for (int i = 0; i < 256; i++) {
    visited[i] = 0;
  }

  int64_t count = 0;
  while (depth != -1) {
    int next_code = rows[depth].get_next_code(pairs_lut);
    if (next_code == INVALID) {
      depth--;
      continue;
    } else {
      depth++;
      rows[depth].set(next_code);
    }
    if (depth == M - 1) {
      next_code = rows[depth].get_next_code(pairs_lut);
      if (next_code == 0) {
        count++;
        rows[depth + 1].set(next_code);
        if (count == 534874) {
          write_svg(rows, M + 1, N, "box.svg");
          print_rows(rows, M + 1);
        }
        for (int i = 1; i < M+1; i++) {
          visited[rows[i].code] = 1;
        }
      }
      depth--;
    }
  }
  //for (int i = 0; i < 256; i++) {
  //  printf("%d ", visited[i]);
  //}
  //printf("\n");
  FILE* fp;
  fp = fopen("graph.dot", "w");
  fprintf(fp, "digraph code_map {\n");
  for (int r = 0; r < (1 << N); r++) {
    //printf("%d - ", r);
    for (int c = 0; c < pairs_lut[r].size(); c++) {
      if ( r < pairs_lut[r][c] ) {
        if (visited[r] == 1) {
          fprintf(fp, "    %d -> %d;\n", r, pairs_lut[r][c]);
        }
      }
    }
    //printf("\n");
  }
  fprintf(fp, "}\n");
  fclose(fp);
  printf("total enumerations: %ld\n", count);
  return 0;
}
