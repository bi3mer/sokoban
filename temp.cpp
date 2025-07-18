#include <iostream>
#include <fstream>

int main() {
    std::ifstream file("levels/00.txt", std::ios::binary);

    if (file.is_open()) {
        char byte;
        while (file.get(byte)) {
            std::cout << byte;
        }

        file.close();
    } else {
        std::cerr << "Error opening file" << std::endl;
    }

    return 0;
}