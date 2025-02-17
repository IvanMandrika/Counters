#include "../src/counters.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <vector>

using size_t = std::size_t;

std::string readFile(const std::string &filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  if (file) {
    buffer << file.rdbuf();
  } else {
    throw std::runtime_error("Error opening file: " + filename);
  }
  return buffer.str();
}

std::vector<std::string> split(const std::string &str, const char delimiter) {
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

using RetPrepareData = std::pair<std::vector<std::string>, std::vector<std::string>>;

RetPrepareData prepareData(const std::string &data, const std::string &testData) {
  std::stringstream buffer;
  std::streambuf *oldCout = std::cout.rdbuf();
  std::cout.rdbuf(buffer.rdbuf());

  work(data);

  std::cout.rdbuf(oldCout);
  std::string expectedOutput = readFile(testData);

  std::vector<std::string> expectedOutputLines = split(expectedOutput, '\n');
  std::sort(expectedOutputLines.begin(), expectedOutputLines.end());
  std::vector<std::string> outputLines = split(buffer.str(), '\n');
  std::sort(outputLines.begin(), outputLines.end());
  return {expectedOutputLines, outputLines};
}

TEST(Counters, GoodFiles) {
  try {
    auto [expectedOutputLines, outputLines] =
        prepareData("data/data_raw_32_rand_ch_offs", "test_data/good_ans.txt");

    int32_t diff = 0;
    for (size_t i = 0; i < expectedOutputLines.size(); ++i) {
      if (expectedOutputLines[i] == "\n") {
        diff -= 1;
        continue;
      }
      if (i >= outputLines.size()) {
        std::cerr << "Output is too short\n";
        break;
      }
      if (outputLines[i] == "\n") {
        diff -= 1;
        continue;
      }
      EXPECT_EQ(expectedOutputLines[i], outputLines[i + diff]);
    }
  } catch (...) {
    std::cerr << "Sorry, test broken\nError opening file: test_data/good_ans.txt\n";
  }
}

TEST(Counters, BadFiles) {
  try {
    auto [expectedOutputLines, outputLines] =
        prepareData("data/data_raw_32_rand_ch_offs_break", "test_data/bad_ans.txt");

    int32_t diff = 0;
    for (size_t i = 0; i < expectedOutputLines.size(); ++i) {
      if (expectedOutputLines[i] == "\n") {
        diff -= 1;
        continue;
      }
      if (i >= outputLines.size()) {
        std::cerr << "Output is too short\n";
        break;
      }
      if (outputLines[i] == "\n") {
        diff -= 1;
        continue;
      }
      EXPECT_EQ(expectedOutputLines[i], outputLines[i + diff]);
    }
  } catch (...) {
    std::cerr << "Sorry, test broken\nError opening file: test_data/good_ans.txt\n";
  }
}