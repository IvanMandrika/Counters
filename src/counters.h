#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#include <bitset>
#include <cstdint>

using size_t = std::size_t;
void processFile(const std::filesystem::path& filepath, std::atomic<int32_t>& totalErrors);
int work(const std::string& argv);