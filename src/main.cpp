#include "counters.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory>\n";
        return 1;
    }
    return work(argv[1]);
}
