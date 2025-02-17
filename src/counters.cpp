#include "counters.h"

constexpr size_t CHANNELS = 32;
constexpr uint32_t MAX_COUNTER = 0xFFFFFFFF;
std::mutex consoleMutex;

void processFile(const std::filesystem::path& filepath, std::atomic<int32_t>& totalErrors) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cerr << "Failed to open file: " << filepath << '\n';
        return;
    }

    std::vector<uint32_t> lastValues(CHANNELS, 0);
    std::bitset<CHANNELS> initialized(false);
    int32_t errors = 0;
    uint32_t value;
    size_t index = 0;

    //choose c-style (reinterpret_cast) only for read data
    while (file.read(reinterpret_cast<char*>(&value), sizeof(uint32_t))) {
        const size_t channel = index % CHANNELS;
        if (initialized[channel]) {
            if (const uint32_t expected = (lastValues[channel] == MAX_COUNTER) ?
                0 : (lastValues[channel] + 1);
                value != expected) {
                std::lock_guard<std::mutex> lock(consoleMutex);
                std::cout << "Count missing in: " << filepath.filename() << ", Channel: " << channel
                          << ", Expected: " << expected << ", Actual: " << value << '\n';
                ++errors;
            }
        } else {
            initialized[channel] = true;
        }
        lastValues[channel] = value;
        ++index;
    }

    totalErrors += errors;
    std::lock_guard<std::mutex> lock(consoleMutex);
    std::cout << "File: " << filepath.filename() << " - Errors: " << errors << '\n';
}

int32_t work(const std::string& argv) {
    const std::filesystem::path dirPath = argv;
    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) {
        std::cerr << "Invalid path to directory: " << dirPath << '\n';
        return 1;
    }

    std::vector<std::thread> threads;
    std::atomic<int32_t> totalErrors = 0;

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            threads.emplace_back(processFile, entry.path(), std::ref(totalErrors));
        }
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "Total errors count: " << totalErrors.load() << '\n';
    return 0;
}
