#include <cinttypes>
#include <string>

struct Options{
  uint16_t length = 0;
  uint16_t width = 0;
  uint64_t max_iter = 0;
  uint64_t freq = 0;
  std::string input_path;
  std::string output_path;
};

Options parse(int arguments_amount, char* arguments_array[]);
