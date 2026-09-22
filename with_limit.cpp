#include <iostream>
#include <string>
#include <windows.h>

// Функция принимает исходную строку и ВОЗВРАЩАЕТ новое развернутое слово
std::string getReversedString(std::string str) {
    int start = 0;
    int end = str.length() - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        start++;
        end--;
    }

    return str; // Возвращаем измененную копию строки
}

int main() {

    SetConsoleCP(1251);       // Устанавливаем кодировку ввода UTF-8
    SetConsoleOutputCP(1251); // Устанавливаем кодировку вывода UTF-8
    int userLimit;
    std::string originalText; // Переменная для исходного текста

    // 1. Установка лимита символов
    std::cout << "Enter character limit: ";
    std::cin >> userLimit;
    std::cin.ignore();

    // 2. Ввод текста
    std::cout << "Введите ваше ФИО: ";
    std::getline(std::cin, originalText);

    // 3. Обрезаем исходный текст по лимиту, если он длиннее
    if (originalText.length() > userLimit) {
        originalText = originalText.substr(0, userLimit);
    }

    // 4. СОХРАНЯЕМ результат в отдельную новую переменную
    std::string reversedText = getReversedString(originalText);

    // Выводим обе переменные, чтобы убедиться, что исходное слово не испортилось
    std::cout << "\n--- Result ---" << std::endl;
    std::cout << "Original variable (limited): " << originalText << std::endl;
    std::cout << "New variable (reversed):     " << reversedText << std::endl;

    return 0;
}
