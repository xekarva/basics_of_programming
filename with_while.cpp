#include <iostream>
#include <string>
#include <clocale> // Библиотека для настройки локали (кириллицы)

// Функция переворота строки через цикл while
void reverseString(std::string &str) {
    int start = 0;
    int end = str.length() - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        start++;
        end--;
    }
}

int main() {
    // Включаем поддержку русского языка в консоли

    std::string text;

    std::cout << "Введите строку на русском: ";
    std::getline(std::cin, text);

    reverseString(text);

    std::cout << "Результат: " << text << std::endl;

    return 0;
}
