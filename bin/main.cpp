#include "libs/parsing.h"
#include "libs/tobmp.h"

#include <iostream>
#include <deque>
#include <fstream>
#include <sstream>
#include <string>


void showStanding(const std::deque<std::deque<uint64_t>>& deq, Options& nowOptions) {
  std::cout << "--------------------------------------" << std::endl;
  std::cout << "standing" << std::endl;
  for (int i = 0; i < nowOptions.length; ++i) {
    for (int j = 0; j < nowOptions.width; ++j) {
      std::cout << deq[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "--------------------------------------" << std::endl;
}

void showNeeds(const std::deque<std::pair<uint16_t, uint16_t>>& need_topple) {
  std::cout << "--------------------------------------" << std::endl;
  std::cout << "need topple" << std::endl;
  for (int i = 0; i < need_topple.size(); ++i) {
    std::cout << "(" << need_topple[i].first << ", " << need_topple[i].second << ")" << std::endl;
  }
  std::cout << "--------------------------------------" << std::endl;
}

void fillDeq(std::deque<std::deque<uint64_t>>& deq,
             std::deque<std::pair<uint16_t, uint16_t>>& need_topple,
             const std::string& file_name, Options& nowOptions) {
  std::ifstream work_file(file_name);
  if (!(work_file.is_open())) {
    std::cout << "cannot find file: " << file_name << std::endl
              << "ended with error";
    exit(1);
  }
  std::string line;
  char delimiter = '\t';
  while (getline(work_file, line)) {
    std::stringstream stream(line);
    std::string x;
    std::string y;
    std::string amount;
    getline(stream, x, delimiter);
    getline(stream, y, delimiter);
    getline(stream, amount, delimiter);
    uint16_t uint_x = std::stoi(x);
    uint16_t uint_y = std::stoi(y);
    uint16_t uint_amount = std::stoi(amount);
    deq[uint_y][uint_x] = uint_amount;
    if (uint_amount > 3) {
      need_topple.push_back({uint_y, uint_x});
    }
  }
}

void createImage(std::deque<std::deque<uint64_t>>& deq,
                 const std::string& image_number,
                 const Options& nowOptions) {
  Img image(nowOptions.width, nowOptions.length);
  for (int y = 0; y < nowOptions.length; ++y) {
    for (int x = 0; x < nowOptions.width; ++x) {
      if (deq[y][x] == 0) image.SetColor(Color(1.0f, 1.0f, 1.0f), y, x);
      if (deq[y][x] == 1) image.SetColor(Color(0, 0.5f, 0.3f), y, x);
      if (deq[y][x] == 2) image.SetColor(Color(0.5f, 0, 0.5f), y, x);
      if (deq[y][x] == 3) image.SetColor(Color(1.0f, 1.0f, 0), y, x);
      if (deq[y][x] > 3) image.SetColor(Color(0, 0, 0), y, x);
    }
  }
  std::string path = nowOptions.output_path;
  std::string extension = ".bmp";
  image.Export(path + image_number + extension);
}

void topple(std::deque<std::deque<uint64_t>>& deq,
            std::deque<std::pair<uint16_t, uint16_t>>& need_topple, Options& nowOptions) {
  uint64_t cnt = 0;
  while (need_topple.size() > 0) {
    uint16_t shift_i = 0;
    uint16_t shift_j = 0;
    uint64_t size_before_iter = need_topple.size();
    if (nowOptions.freq != 0 && cnt%nowOptions.freq == 0) {
      createImage(deq, std::to_string(cnt/nowOptions.freq), nowOptions);
    }
    if (cnt == nowOptions.max_iter) break;
    cnt++;
    uint16_t changed_i_ind = 0;
    uint16_t changed_j_ind = 0;
    for (int k = size_before_iter - 1; k >= 0; --k) {
      uint16_t i = need_topple[k].first + shift_i;
      uint16_t j = need_topple[k].second + shift_j;
      if (j + 1 >= nowOptions.width) {
        for (int l = 0; l < nowOptions.length; ++l) {
          deq[l].push_back(0);
        }
        nowOptions.width++;
      }
      if (j - 1 < 0) {
        for (int l = 0; l < nowOptions.length; ++l) {
          deq[l].push_front(0);
        }
        shift_j++;
        j++;
        changed_i_ind = need_topple.size() - size_before_iter;
        nowOptions.width++;
      }
      std::deque<uint64_t> temp_deque(nowOptions.width);
      for (int l = 0; l < nowOptions.width; ++l) {
        temp_deque[l] = 0;
      }
      if (i + 1 >= nowOptions.length) {
        deq.push_back(temp_deque);
        nowOptions.length++;
      }
      if (i - 1 < 0) {
        deq.push_front(temp_deque);
        nowOptions.length++;
        changed_j_ind = need_topple.size() - size_before_iter;
        shift_i++;
        i++;
      }
      deq[i][j + 1] += 1;
      if (deq[i][j + 1] == 4) {
        need_topple.push_back({i, j + 1});
      }
      deq[i][j - 1] += 1;
      if (deq[i][j - 1] == 4) {
        need_topple.push_back({i, j - 1});
      }
      deq[i + 1][j] += 1;
      if (deq[i + 1][j] == 4) {
        need_topple.push_back({i + 1, j});
      }
      deq[i - 1][j] += 1;
      if (deq[i - 1][j] == 4) {
        need_topple.push_back({i - 1, j});
      }
      deq[i][j] -= 4;
      if (deq[i][j] > 3) need_topple.push_back({i, j});
    }
    for (int i = 0; i < size_before_iter; ++i) need_topple.pop_front();
    for (int i = 0; i < changed_j_ind; ++i) {
      need_topple[i].first += 1;
    }
    for (int i = 0; i < changed_i_ind; ++i) {
      need_topple[i].second += 1;
    }
  }
}

int main(int argc, char* argv[]) {
  Options nowOptions = parse(argc, argv);
  std::deque<std::deque<uint64_t>> deq(nowOptions.length, std::deque<uint64_t>(nowOptions.width));
  std::deque<std::pair<uint16_t, uint16_t>> need_topple;
  fillDeq(deq, need_topple, nowOptions.input_path, nowOptions);
  topple(deq, need_topple, nowOptions);
  createImage(deq, "image_final", nowOptions);

  return 0;
}
