#include <parsing.h>

#include <iostream>
#include <cinttypes>
#include <string>

Options parse(int arguments_amount, char* arguments_array[]) {
  Options OptionsList;
  uint8_t i = 1;
  while (i + 1 < arguments_amount) {
	if (strcmp(arguments_array[i], "--length") == 0 || strcmp(arguments_array[i], "-l") == 0) {
	  i++;
	  OptionsList.length = std::atoi(arguments_array[i]);
	}
	else if (strcmp(arguments_array[i], "--width") == 0 || strcmp(arguments_array[i], "-w") == 0) {
	  i++;
	  OptionsList.width = std::atoi(arguments_array[i]);
	}
	else if (strcmp(arguments_array[i], "--input") == 0 || strcmp(arguments_array[i], "-i") == 0) {
	  i++;
	  OptionsList.input_path = arguments_array[i];
	}
	else if (strcmp(arguments_array[i], "--output") == 0 || strcmp(arguments_array[i], "-o") == 0) {
	  i++;
	  OptionsList.output_path = arguments_array[i];
	}
	else if (strcmp(arguments_array[i], "--max-iter") == 0 || strcmp(arguments_array[i], "-m") == 0) {
	  i++;
	  OptionsList.max_iter = std::atoi(arguments_array[i]);
	}
	else if (strcmp(arguments_array[i], "--freq") == 0 || strcmp(arguments_array[i], "-f") == 0) {
	  i++;
	  OptionsList.freq = std::atoi(arguments_array[i]);
	}

	i++;
  }

  return OptionsList;
}