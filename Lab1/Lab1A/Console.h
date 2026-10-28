#pragma once
#include <limits>
#include <stdexcept>
#include <string>


inline int readNumber(const std::string& prompt, int minimum, int maximum) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (value >= minimum && value <= maximum) {
                return value;
            }
        }
        else {
            if (std::cin.eof()) {
                throw std::runtime_error("Ввод завершён");
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Введите целое число от " << minimum << " до " << maximum << ".\n";
    }
}

inline std::string readText(const std::string& prompt) {
    std::cout << prompt;
    std::string text;
    if (!std::getline(std::cin, text)) {
        throw std::runtime_error("Ввод завершён");
    }
    return text;
}
