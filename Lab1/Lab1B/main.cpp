#include "TextIndex.h"
#include "Tests.h"
#include "Console.h"
#include <iostream>
#include <cstdlib>


void printIds(const std::vector<int>& ids) {
    if (ids.empty()) {
        std::cout << "ничего не найдено";
    }
    for (int id : ids) {
        std::cout << id << " ";
    }
    std::cout << "\n";
}

void printDocuments(const TextIndex& index) {
    std::cout << "\nДокументы из папки documents:\n";
    if (index.getDocuments().empty()) {
        std::cout << "Нет загруженных txt-файлов.\n";
    }
    for (const Document& document : index.getDocuments()) {
        std::cout << document.getId() << " — " << document.getFileName() << "\n";
    }
}

void printIndex(const TextIndex& index) {
    auto entries = index.getIndex();
    if (entries.empty()) {
        std::cout << "Индекс пуст.\n";
    }
    for (const auto& entry : entries) {
        std::cout << entry.first << ": ";
        printIds(entry.second);
    }
}

void runExample() {
    TextIndex index;
    index.loadDirectory("documents");
    printDocuments(index);
    std::cout << "\nИндекс: симметричный обход\n";
    printIndex(index);
    std::cout << "\nПоиск слова КОТ: ";
    printIds(index.searchWord("КОТ"));
    std::cout << "Поиск слова самолёт: ";
    printIds(index.searchWord("самолёт"));
    std::cout << "Все слова: кот код: ";
    printIds(index.searchAll("кот код"));
    if (!index.getDocuments().empty()) {
        int id = index.getDocuments().front().getId();
        index.removeDocument(id);
        std::cout << "После удаления документа " << id << ", поиск кот: ";
        printIds(index.searchWord("кот"));
        std::cout << "Индекс после удаления:\n";
        printIndex(index);
    }
}

void manualMode(TextIndex& index) {
    while (true) {
        std::cout << "\nИНДЕКС ДОКУМЕНТОВ\n1. Поиск одного слова\n"
            << "2. Поиск всех слов запроса\n3. Показать индекс\n"
            << "4. Показать документы\n5. Удалить документ из индекса\n"
            << "6. Перечитать папку documents\n0. Назад\n";
        int choice = readNumber("Выбор: ", 0, 6);
        if (choice == 0) {
            return;
        }
        try {
            if (choice == 1) {
                std::string word = readText("Слово: ");
                printIds(index.searchWord(word));
            }
            else if (choice == 2) {
                std::string query = readText("Запрос: ");
                printIds(index.searchAll(query));
            }
            else if (choice == 3) {
                printIndex(index);
            }
            else if (choice == 4) {
                printDocuments(index);
            }
            else if (choice == 5) {
                int id = readNumber("ID документа: ", 1, std::numeric_limits<int>::max());
                if (index.removeDocument(id)) {
                    std::cout << "Документ удалён из индекса.\n";
                }
                else {
                    std::cout << "Документ не найден.\n";
                }
            }
            else {
                index.loadDirectory("documents");
                printDocuments(index);
            }
        }
        catch (const std::exception& error) {
            if (std::cin.eof()) {
                return;
            }
            std::cout << "Ошибка: " << error.what() << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    std::system("chcp 65001 > nul");
    try {
        if (argc > 1 && std::string(argv[1]) == "--test") {
            runTests();
            return 0;
        }
        if (argc > 1 && std::string(argv[1]) == "--demo") {
            runExample();
            return 0;
        }
        TextIndex index;
        index.loadDirectory("documents");
        printDocuments(index);
        while (true) {
            std::cout << "\n1. Запустить тесты\n2. Ручное использование\n3. Готовый пример\n0. Выход\n";
            int choice = readNumber("Выбор режима: ", 0, 3);
            if (choice == 0) {
                return 0;
            }
            if (choice == 1) {
                runTests();
            }
            else if (choice == 2) {
                manualMode(index);
            }
            else {
                runExample();
            }
        }
    }
    catch (const std::exception& error) {
        std::cout << "Ошибка: " << error.what() << "\n";
        return 1;
    }
    return 0;
}
