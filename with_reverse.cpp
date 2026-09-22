#include <iostream>
#include <string>
#include <algorithm> // Нужен для функции std::reverse

// Функция для переворота строки
void reverseString(std::string &str) {
    std::reverse(str.begin(), str.end());
}

int main() {
    std::string text;

    std::cout << "Enter a string: ";
    std::getline(std::cin, text); // Считывает строку целиком (с пробелами)

    reverseString(text);

    std::cout << "Result: " << text << std::endl;

    return 0;
}
